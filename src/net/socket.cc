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
    Socket::Socket(Endpoint endpoint) : endpoint_(std::move(endpoint)) {
        fd_socket_ = socket(
            endpoint_.family(),
            endpoint_.socketType(),
            endpoint_.protocol()
        );

        if (fd_socket_ < 0) {
            throw SocketError {"Failed to initialize socket."};
        }
    }

    auto Socket::connect() -> void {
        auto result = ::connect(
            fd_socket_,
            endpoint_.address(),
            endpoint_.addressLength()
        );

        if (result < 0) {
            throw SocketError {"Failed to connect."};
        }
    }

    auto Socket::send(std::string_view buffer, const Timeout& timeout) const -> ssize_t {
        wait(EventType::ToWrite, timeout);
        auto result = ::send(
            fd_socket_,
            buffer.data(),
            static_cast<int>(buffer.size()),
            0
        );

        while (result == -1 && errno == EINTR) {
            result = ::send(
                fd_socket_,
                buffer.data(),
                static_cast<int>(buffer.size()),
                0
            );
        }

        if (result == -1) {
            throw SocketError {"Failed to send data to the server."};
        }

        return result;
    }

    auto Socket::sendAll(std::string_view buffer, const Timeout& timeout) const -> void {
        auto data_ptr = buffer.data();
        ssize_t bytes_remaining = buffer.size();
        while (bytes_remaining > 0) {
            auto bytes_sent = this->send(data_ptr, timeout);
            data_ptr += bytes_sent;
            bytes_remaining -= bytes_sent;
        }
    }

    auto Socket::recv(uint8_t* buffer, const Timeout& timeout) const -> ssize_t {
        wait(EventType::ToRead, timeout);
        auto result = ::recv(
            fd_socket_,
            reinterpret_cast<char*>(buffer),
            BUFSIZ,
            0
        );

        while (result == -1 && errno == EINTR) {
            result = ::recv(
                fd_socket_,
                reinterpret_cast<char*>(buffer),
                BUFSIZ,
                0
            );
        }

        if (result == -1) {
            throw SocketError {"Failed to receive data from the server."};
        }

        return result;
    }

    auto Socket::wait(EventType event, const Timeout& timeout) const -> void {
        fd_set fdset;
        FD_ZERO(&fdset);
        FD_SET(fd_socket_, &fdset);

        timeval select_timeout {
            .tv_sec = timeout.get() / 1000,
            .tv_usec = (timeout.get() % 1000) * 1000,
        };

        auto count = ::select(
            fd_socket_ + 1,
            event == EventType::ToRead ? &fdset : nullptr,
            event == EventType::ToWrite ? &fdset : nullptr,
            nullptr,
            timeout.hasTimeout() ? &select_timeout : nullptr
        );

        while (count == -1 && errno == EINTR) {
            count = ::select(
                fd_socket_ + 1,
                event == EventType::ToRead ? &fdset : nullptr,
                event == EventType::ToWrite ? &fdset : nullptr,
                nullptr,
                timeout.hasTimeout() ? &select_timeout : nullptr
            );
        }

        if (count == -1) throw SocketError {"Failed to select socket."};
        if (count == 0) throw SocketError {"Request timed out."};
    }

    Socket::~Socket() {
        if (fd_socket_ != -1) {
            close(fd_socket_);
            fd_socket_ = -1;
        }
    }
}
