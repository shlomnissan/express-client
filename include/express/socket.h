// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <express/socket_defs.h>
#include <express/endpoint.h>
#include <express/timeout.h>

namespace Express::Net {
    enum class EventType {ToRead, ToWrite};

    class Socket {
    public:
        explicit Socket(Endpoint endpoint);

        // delete copy/move constructor and assignment
        Socket(Socket&& src) = delete;
        auto operator=(Socket&& rhs) = delete;
        Socket(const Socket&) = delete;
        auto operator=(const Socket&) -> Socket& = delete;

        virtual auto connect() -> void;
        virtual auto send(std::string_view buffer, const Timeout& timeout) const -> ssize_t;
        virtual auto recv(uint8_t* buffer, const Timeout& timeout) const -> ssize_t;

        auto sendAll(std::string_view buffer, const Timeout& timeout) const -> void;

        [[nodiscard]] SOCKET get() const { return fd_socket_; };

        virtual ~Socket();

    protected:
        SOCKET fd_socket_ = INVALID_SOCKET;
        Endpoint endpoint_;

        auto wait(EventType event, const Timeout& timeout) const -> void;
    };

    struct SocketError : public std::runtime_error {
        using std::runtime_error::runtime_error;
    };
}