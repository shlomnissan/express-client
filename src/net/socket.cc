// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <express/socket.h>
#include <express/endpoint.h>

#include <cerrno>
#include <unistd.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>

namespace Express::Net {
    Socket::Socket(Endpoint endpoint) : ep_(std::move(endpoint)) {
        sock_ = socket(ep_.family(), ep_.socketType(), ep_.protocol());

        if (sock_ < 0) {
            throw SocketError {"Failed to initialize socket."};
        }
    }

    auto Socket::connect() -> void {
        auto result = ::connect(sock_, ep_.address(), ep_.addressLength());

        if (result < 0) {
            throw SocketError {"Failed to connect."};
        }
    }

    auto Socket::send(std::string_view buffer, const Timeout& timeout) const -> size_t {
        wait(EventType::ToWrite, timeout);

        auto ptr = buffer.data();
        auto bytes_left = buffer.size();

        while (bytes_left > 0) {
            auto bytes_written = ::send(sock_, ptr, bytes_left, 0);
            if (bytes_written < 0) {
                throw SocketError {"Failed to send data to the server."};
            }

            bytes_left -= bytes_written;
            ptr += bytes_written;
        }

        return buffer.size();
    }

    auto Socket::recv(uint8_t* buffer, const Timeout& timeout) const -> ssize_t {
        wait(EventType::ToRead, timeout);

        auto result = ::recv(
            sock_,
            buffer,
            BUFSIZ,
            0
        );

        if (result < 0) {
            throw SocketError {"Failed to receive data from the server."};
        }

        return result;
    }

    auto Socket::wait(EventType event, const Timeout& timeout) const -> void {
        fd_set fdset;
        FD_ZERO(&fdset);
        FD_SET(sock_, &fdset);

        timeval select_timeout {
            // casts are needed for portability. timeout.get() returns a 64-bit
            // value, but time_t and suseconds_t are smaller on some platforms
            .tv_sec = static_cast<time_t>(timeout.get() / 1000),
            .tv_usec = static_cast<suseconds_t>((timeout.get() % 1000) * 1000),
        };

        auto count = ::select(
            sock_ + 1,
            event == EventType::ToRead ? &fdset : nullptr,
            event == EventType::ToWrite ? &fdset : nullptr,
            nullptr,
            timeout.hasTimeout() ? &select_timeout : nullptr
        );

        if (count == 0) {
            throw SocketError {"Request timed out."};
        }

        if (count < 0) {
            throw SocketError {"Failed to select socket."};
        }
    }

    Socket::~Socket() {
        if (sock_ != -1) {
            close(sock_);
            sock_ = -1;
        }
    }
}
