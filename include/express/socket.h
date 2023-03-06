// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <express/endpoint.h>

#include <chrono>
#include <string_view>
#include <stdexcept>

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
        int fd_socket = 0;
        Endpoint endpoint_;

        auto wait(EventType event, milliseconds timeout) const -> void;
    };

    struct SocketError : public std::runtime_error {
        using std::runtime_error::runtime_error;
    };
}