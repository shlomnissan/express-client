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

    using ssize_t = SSIZE_T;

    #define SYS_EINTR WSAEINTR
    #define CLOSE(s) closesocket(s)
    #define ERRNO() (WSAGetLastError())
#else
    #include <cerrno>
    #include <unistd.h>
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

        // implements move constructor and assignment
        Socket(Socket&& src) noexcept;
        auto operator=(Socket&& rhs) noexcept -> Socket&;

        // deletes copy constructor and assignment
        Socket(const Socket&) = delete;
        auto operator=(const Socket&) -> Socket& = delete;

        auto connect() const -> void;
        auto send(std::string_view buffer, milliseconds timeout) const -> ssize_t;
        auto recv(uint8_t* buffer, milliseconds timeout) const -> ssize_t;

        [[nodiscard]] SOCKET get() const { return fd_socket_; };

        ~Socket();

    private:
        SOCKET fd_socket_ = INVALID_SOCKET;
        Endpoint endpoint_;

        auto wait(EventType event, milliseconds timeout) const -> void;
        auto close_() -> void;
    };

    struct SocketError : public std::runtime_error {
        using std::runtime_error::runtime_error;
    };
}