// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include "socket.h"

#include <fcntl.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "client/error.h"

namespace Express::Net {
    Socket::Socket(Endpoint endpoint) : ep_(std::move(endpoint)) {
        sock_ = socket(ep_.family(), ep_.socket_type(), ep_.protocol());
        if (sock_ < 0) {
            Error::System("Socket error");
        }
        MakeNonBlocking();
    }

    auto Socket::MakeNonBlocking() const -> void {
        const auto flags = fcntl(sock_, F_GETFL, 0);
        if (flags < 0) {
            close(sock_);
            Error::System("Socket flags error");
        }
        if (fcntl(sock_, F_SETFL, flags | O_NONBLOCK) < 0) {
            close(sock_);
            Error::System("Socket flags error");
        }
    }

    auto Socket::GetPendingError() const -> int {
        auto option_value = 0;
        socklen_t option_length = sizeof(option_value);
        if (getsockopt(sock_, SOL_SOCKET, SO_ERROR, &option_value, &option_length) < 0) {
           Error::System("Socket options error");
        }
        return option_value;
    }

    auto Socket::Connect(const Timeout& timeout) const -> void {
        auto result = connect(sock_, ep_.address(), ep_.address_length());

        if (result < 0) {
            if (errno == EINPROGRESS) {
                auto select_result = Select(EventType::kToWrite, timeout);
                if (select_result == 0) {
                    Error::Runtime("Timeout error", "Failed to connect");
                }

                // Check for any pending errors.
                // If there are none, the connection was successful
                auto pending_error = GetPendingError();
                if (pending_error != 0) {
                    Error::System("Socket connect error");
                }
            } else {
                Error::System("Socket connect error");
            }
        }
    }

    auto Socket::Send(std::string_view buffer, const Timeout& timeout) const -> size_t {
        if (Select(EventType::kToWrite, timeout) == 0) {
            Error::Runtime("Timeout error", "Failed to send data to the server");
        }

        auto data_ptr = buffer.data();
        auto bytes_left = buffer.size();
        while (bytes_left > 1) {
            auto bytes_written = send(sock_, data_ptr, bytes_left, 0);
            if (bytes_written < 0) {
                Error::System("Socket send error");
            }

            bytes_left -= bytes_written;
            data_ptr += bytes_written;
        }

        return buffer.size();
    }

    auto Socket::Recv(unsigned char* buffer, const size_t size, const Timeout& timeout) const -> size_t {
        if (Select(EventType::kToRead, timeout) == 0) {
            Error::Runtime("Timeout error", "Failed to receive data from the server");
        }

        auto bytes_read = recv(sock_, buffer, size, 0);
        if (bytes_read < 0) {
            Error::System("Socket recv error");
        }

        return bytes_read;
    }

    auto Socket::Select(EventType event, const Timeout& timeout) const -> int {
        fd_set fdset;
        FD_ZERO(&fdset);
        FD_SET(sock_, &fdset);

        timeval select_timeout {
            // Casts are needed for portability. timeout.get() returns a 64-bit
            // value, but time_t and suseconds_t are smaller on some platforms
            .tv_sec = static_cast<time_t>(timeout.Get() / 1000),
            .tv_usec = static_cast<suseconds_t>((timeout.Get() % 1000) * 1000),
        };

        auto result = select(
            sock_ + 1,
            event == EventType::kToRead ? &fdset : nullptr,
            event == EventType::kToWrite ? &fdset : nullptr,
            nullptr,
            timeout.has_timeout() ? &select_timeout : nullptr
        );

        if (result < 0) {
            Error::System("Socket select error");
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