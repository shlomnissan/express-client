// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <express/endpoint.h>

#include <chrono>
#include <string_view>
#include <stdexcept>

#if defined(_WIN32)
    #ifndef _WIN32_WINNT
        #define _WIN32_WINNT 0x0600
    #endif
    #include <winsock2.h>
    #include <BaseTsd.h>

    #pragma comment(lib, "ws2_32.lib")

    using ssize_t = SSIZE_T;

    #define SYS_EINTR WSAEINTR
    #define CLOSE(s) closesocket(s)
    #define ERRNO() (WSAGetLastError())
#else
    #include <cerrno>
    #include <sys/socket.h>
    #include <sys/select.h>
    #include <sys/types.h>

    using SOCKET = int;
    constexpr auto INVALID_SOCKET = -1;

    #define SYS_EINTR EINTR
    #define CLOSE(s) close(s)
    #define ERRNO() (errno)
#endif

namespace Express::Net {
    using namespace std::chrono;

    enum class EventType {ToRead, ToWrite};

    class Socket {
    public:
        explicit Socket(Endpoint endpoint);

        auto connect() const -> void;
        auto send(std::string_view buffer, milliseconds timeout) const -> ssize_t;
        auto recv(uint8_t* buffer, milliseconds timeout) const -> ssize_t;

        [[nodiscard]] int get() const { return fd_socket; };

        ~Socket();

    private:
        SOCKET fd_socket = 0;
        Endpoint endpoint_;

        auto wait(EventType event, milliseconds timeout) const -> void;
    };

    struct SocketError : public std::runtime_error {
        using std::runtime_error::runtime_error;
    };
}