// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include "express/endpoint.h"
#include <express/socket.h>

#ifdef BUILD_SSL
    #include <iostream>
    #include <openssl/crypto.h>
    #include <openssl/x509.h>
    #include <openssl/ssl.h>
    #include <openssl/err.h>
#endif

namespace Express::Net {
    constexpr auto InterruptedBySystemSignal = EINTR; 

    Socket::Socket(Endpoint endpoint) : endpoint_(std::move(endpoint)) {
        fd_socket_ = socket(
            endpoint_.family(),
            endpoint_.socketType(),
            endpoint_.protocol()
        );

        std::cout << OpenSSL_version(SSLEAY_VERSION) << '\n';

        if (fd_socket_ == INVALID_SOCKET) {
            throw SocketError {"Failed to initialize socket."};
        }
    }

    Socket::Socket(Socket&& src) noexcept :
      fd_socket_{src.fd_socket_},
      endpoint_{std::move(src.endpoint_)} {
        src.fd_socket_ = INVALID_SOCKET;
    }

    auto Socket::operator=(Socket&& rhs) noexcept -> Socket& {
        if (this != &rhs) {
            std::swap(fd_socket_, rhs.fd_socket_);
            std::swap(endpoint_, rhs.endpoint_);
            CLOSE(rhs.fd_socket_);
            rhs.fd_socket_ = INVALID_SOCKET;
        }
        return *this;
    }

    auto Socket::connect() const -> void {
        auto result = ::connect(
            fd_socket_,
            endpoint_.address(),
            static_cast<int>(endpoint_.addressLength())
        );

        while (result == -1 && ERRNO() == SYS_EINTR) {
            result = ::connect(
                fd_socket_,
                endpoint_.address(),
                static_cast<int>(endpoint_.addressLength())
            ); 
        }

        if (result == -1) {
            throw SocketError {"Failed to connect."};
        }
    }

    auto Socket::send(std::string_view buffer, milliseconds timeout) const -> ssize_t {
        wait(EventType::ToWrite, timeout);
        return ::send(fd_socket_, buffer.data(), static_cast<int>(buffer.size()), 0);
    }

    auto Socket::recv(uint8_t* buffer, milliseconds timeout) const -> ssize_t {
        wait(EventType::ToRead, timeout);
        return ::recv(fd_socket_, reinterpret_cast<char*>(buffer), BUFSIZ, 0);
    }

    auto Socket::wait(EventType event, milliseconds timeout) const -> void {
        fd_set fdset;
        FD_ZERO(&fdset);
        FD_SET(fd_socket_, &fdset);

        #if defined(_WIN32)
            TIMEVAL select_timeout {
                static_cast<LONG>(timeout.count() / 1000),
                static_cast<LONG>((timeout.count() % 1000) * 1000)
            }; 
        #else
            timeval select_timeout {
                .tv_sec = static_cast<time_t>(timeout.count() / 1000),
                .tv_usec = static_cast<suseconds_t>((timeout.count() % 1000) * 1000),
            };
        #endif

        auto count = ::select(
            static_cast<int>(fd_socket_ + 1),
            event == EventType::ToRead ? &fdset : nullptr,
            event == EventType::ToWrite ? &fdset : nullptr,
            nullptr,
            timeout.count() > 0 ? &select_timeout : nullptr
        );

        while (count == -1 && errno == InterruptedBySystemSignal) {
            count = ::select(
                static_cast<int>(fd_socket_ + 1),
                event == EventType::ToRead ? &fdset : nullptr,
                event == EventType::ToWrite ? &fdset : nullptr,
                nullptr,
                timeout.count() > 0 ? &select_timeout : nullptr
            );
        }

        if (count == -1) throw SocketError {"Failed to select socket."};
        if (count == 0) throw SocketError {"Request timed out."};
    }

    auto Socket::close_() -> void {
        if (fd_socket_ != INVALID_SOCKET) {
            CLOSE(fd_socket_);
            fd_socket_ = INVALID_SOCKET;
        }
    }

    Socket::~Socket() {
        close_();
    }
}
