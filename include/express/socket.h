// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <cstdint>
#include <express/endpoint.h>

#include <string_view>
#include <stdexcept>

namespace Express::Net {
    class Socket {
    public:
        explicit Socket(const Endpoint& endpoint);

        Socket(const Socket&) = delete;
        auto operator=(const Socket&) -> Socket& = delete;

        Socket(Socket&&) = delete;
        auto operator=(Socket&&) -> Socket& = delete;

        auto connect() const -> void;
        auto send(std::string_view buffer) const -> long;
        auto recv(uint8_t* buffer) const -> long;

        ~Socket();

    private:
        int fd_socket_ = 0;
        int address_len_ = 0;
        sockaddr* address_ = nullptr;
    };

    struct SocketError : public std::runtime_error {
        using std::runtime_error::runtime_error;
    };
}