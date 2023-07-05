// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <express/socket.h>
#include <express/endpoint.h>
#include <express/error.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>

namespace Express::Net {
    Socket::Socket(Endpoint endpoint) : ep_(std::move(endpoint)) {
        sock_ = socket(ep_.family(), ep_.socketType(), ep_.protocol());
        if (sock_ < 0) {
            Error::system("Socket error");
        }

        makeNonBlocking(); 
    }

    auto Socket::makeNonBlocking() const -> void {
        const auto flags = fcntl(sock_, F_GETFL, 0);
        if (flags < 0) {
            close(sock_);
            Error::system("Socket flags error");
        }
        if (fcntl(sock_, F_SETFL, flags | O_NONBLOCK) < 0) {
            close(sock_);
            Error::system("Socket flags error");
        }
    }

    auto Socket::getPendingError() const -> int {
        auto option_value = 0;
        socklen_t option_length = sizeof(option_value);
        if (getsockopt(sock_, SOL_SOCKET, SO_ERROR, &option_value, &option_length) < 0) {
           Error::system("Socket options error");
        }
        return option_value;
    }

    auto Socket::connect(const Timeout& timeout) -> void {
        auto result = ::connect(sock_, ep_.address(), ep_.addressLength());

        if (result < 0) {
            if (errno == EINPROGRESS) {
                auto select_result = select(EventType::ToWrite, timeout);
                if (select_result == 0) {
                    Error::runtime("Timeout error", "Failed to connect");
                }

                // Check for any pending errors.
                // If there are none, the connection was successful
                auto pending_error = getPendingError();
                if (pending_error != 0) {
                    Error::system("Socket connect error");
                }
            } else {
                Error::system("Socket connect error");
            }
        }
    }

    auto Socket::send(std::string_view buffer, const Timeout& timeout) const -> size_t {
        if (select(EventType::ToWrite, timeout) == 0) {
            Error::runtime("Timeout error", "Failed to send data to the server");
        }

        auto data_ptr = buffer.data();
        auto bytes_left = buffer.size();

        while (bytes_left > 0) {
            auto bytes_written = ::send(sock_, data_ptr, bytes_left, 0);
            if (bytes_written < 0) {
                Error::system("Socket send error");
            }

            bytes_left -= bytes_written;
            data_ptr += bytes_written;
        }

        return buffer.size();
    }

    auto Socket::recv(uint8_t* buffer, const size_t size, const Timeout& timeout) const -> size_t {
        if (select(EventType::ToRead, timeout) == 0) {
            Error::runtime("Timeout error", "Failed to receive data from the server");
        }

        auto bytes_read = ::recv(sock_, buffer, size, 0);
        if (bytes_read < 0) {
            Error::system("Socket recv error");
        }

        return bytes_read;
    }

    auto Socket::select(EventType event, const Timeout& timeout) const -> int {
        fd_set fdset;
        FD_ZERO(&fdset);
        FD_SET(sock_, &fdset);

        timeval select_timeout {
            // Casts are needed for portability. timeout.get() returns a 64-bit
            // value, but time_t and suseconds_t are smaller on some platforms
            .tv_sec = static_cast<time_t>(timeout.get() / 1000),
            .tv_usec = static_cast<suseconds_t>((timeout.get() % 1000) * 1000),
        };

        auto result = ::select(
            sock_ + 1,
            event == EventType::ToRead ? &fdset : nullptr,
            event == EventType::ToWrite ? &fdset : nullptr,
            nullptr,
            timeout.hasTimeout() ? &select_timeout : nullptr
        );

        if (result < 0) {
            Error::system("Socket select error");
        }

        return result;
    }

    Socket::~Socket() {
        if (sock_ != -1) {
            close(sock_);
            sock_ = -1;
        }
    }
}
