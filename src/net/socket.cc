// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <express/socket.h>

#include <cerrno>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>

namespace Express::Net {
    constexpr auto InterruptedBySystemSignal = EINTR; 

    Socket::Socket(Endpoint endpoint) : endpoint_(std::move(endpoint)) {
        fd_socket = socket(
            endpoint_.family(),
            endpoint_.socketType(),
            endpoint_.protocol()
        );

        if (!fd_socket) {
            throw SocketError {"Failed to initialize socket."};
        }
    }

    auto Socket::connect() const -> void {
        auto result = ::connect(
            fd_socket,
            endpoint_.address(),
            endpoint_.addressLength()
        );

        while (result == -1 && errno == InterruptedBySystemSignal) {
            result = ::connect(
                fd_socket,
                endpoint_.address(),
                endpoint_.addressLength()
            ); 
        }

        if (result == -1) {
            throw SocketError {"Failed to connect."};
        }
    }

    auto Socket::send(std::string_view buffer, uint64_t timeout) const -> ssize_t {
        wait(EventType::ToWrite, timeout);
        return ::send(fd_socket, buffer.data(), buffer.size(), 0);
    }

    auto Socket::recv(uint8_t* buffer, uint64_t timeout) const -> ssize_t {
        wait(EventType::ToRead, timeout);
        return ::recv(fd_socket, buffer, BUFSIZ, 0);
    }

    auto Socket::wait(EventType event, const std::uint64_t timeout) const -> void {
        fd_set fdset;
        FD_ZERO(&fdset);
        FD_SET(fd_socket, &fdset);

        timeval select_timeout {
            .tv_sec = static_cast<time_t>(timeout / 1000),
            .tv_usec = static_cast<suseconds_t>((timeout % 1000) * 1000),
        };

        auto result = ::select(
            fd_socket + 1,
            event == EventType::ToRead ? &fdset : nullptr,
            event == EventType::ToWrite ? &fdset : nullptr,
            nullptr,
            timeout > 0 ? &select_timeout : nullptr
        );

        while (result == -1 && errno == InterruptedBySystemSignal) {
            result = ::select(
                fd_socket + 1,
                event == EventType::ToRead ? &fdset : nullptr,
                event == EventType::ToWrite ? &fdset : nullptr,
                nullptr,
                timeout > 0 ? &select_timeout : nullptr
            );
        }

        if (result == -1) throw SocketError {"Failed to select socket."};
        if (result == 0) throw SocketError {"Request timed out."};
    }

    Socket::~Socket() {
        if (fd_socket) {
            close(fd_socket);
        }
    }
}
