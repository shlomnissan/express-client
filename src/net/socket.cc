// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <express/socket.h>

namespace Express::Net {
    constexpr auto InterruptedBySystemSignal = EINTR; 

    Socket::Socket(Endpoint endpoint) : endpoint_(std::move(endpoint)) {
        fd_socket = socket(
            endpoint_.family(),
            endpoint_.socketType(),
            endpoint_.protocol()
        );

        if (fd_socket == INVALID_SOCKET) {
            throw SocketError {"Failed to initialize socket."};
        }
    }

    auto Socket::connect() const -> void {
        auto result = ::connect(
            fd_socket,
            endpoint_.address(),
            endpoint_.addressLength()
        );

        while (result == -1 && ERRNO() == SYS_EINTR) {
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

    auto Socket::send(std::string_view buffer, milliseconds timeout) const -> ssize_t {
        wait(EventType::ToWrite, timeout);
        return ::send(fd_socket, buffer.data(), buffer.size(), 0);
    }

    auto Socket::recv(uint8_t* buffer, milliseconds timeout) const -> ssize_t {
        wait(EventType::ToRead, timeout);
        return ::recv(fd_socket, reinterpret_cast<char*>(buffer), BUFSIZ, 0);
    }

    auto Socket::wait(EventType event, milliseconds timeout) const -> void {
        fd_set fdset;
        FD_ZERO(&fdset);
        FD_SET(fd_socket, &fdset);

        #if defined(_WIN32)
            TIMEVAL select_timeout {
                static_cast<LONG>(timeout.count() / 1000),
                static_cast<LONG>((timeout.count() % 1000) * 1000)
            }; 
        #else
            timeval select_timeout {
                .tv_sec = timeout.count() / 1000,
                .tv_usec = (timeout.count() % 1000) * 1000,
            };
        #endif

        auto count = ::select(
            fd_socket + 1,
            event == EventType::ToRead ? &fdset : nullptr,
            event == EventType::ToWrite ? &fdset : nullptr,
            nullptr,
            timeout.count() > 0 ? &select_timeout : nullptr
        );

        while (count == -1 && errno == InterruptedBySystemSignal) {
            count = ::select(
                fd_socket + 1,
                event == EventType::ToRead ? &fdset : nullptr,
                event == EventType::ToWrite ? &fdset : nullptr,
                nullptr,
                timeout.count() > 0 ? &select_timeout : nullptr
            );
        }

        if (count == -1) throw SocketError {"Failed to select socket."};
        if (count == 0) throw SocketError {"Request timed out."};
    }

    Socket::~Socket() {
        if (fd_socket != INVALID_SOCKET) {
            CLOSE(fd_socket);
        }
    }
}
