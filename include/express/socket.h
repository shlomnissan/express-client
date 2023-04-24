// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <chrono>

#include <express/socket_defs.h>
#include <express/endpoint.h>

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
        auto sendAll(std::string_view buffer, milliseconds timeout) const -> void;
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