// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <express/endpoint.h>

#include <cstdint>
#include <string_view>
#include <stdexcept>

namespace Express::Net {
    class Socket {
    public:
        explicit Socket(Endpoint endpoint);

        auto connect() const -> void;
        auto send(std::string_view buffer) const -> long;
        auto recv(uint8_t* buffer) const -> long;

        [[nodiscard]] int get() const { return socket_fd; };

        ~Socket();

    private:
        int socket_fd = 0;
        Endpoint endpoint_;
    };

    struct SocketError : public std::runtime_error {
        using std::runtime_error::runtime_error;
    };
}