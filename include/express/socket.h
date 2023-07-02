// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

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
        virtual auto send(std::string_view buffer, const Timeout& timeout) const -> size_t;
        virtual auto recv(uint8_t* buffer, const size_t size, const Timeout& timeout) const -> size_t;

        [[nodiscard]] int get() const { return sock_; };

        virtual ~Socket();

    protected:
        Endpoint ep_;
        int sock_ = -1;

        auto select(EventType event, const Timeout& timeout) const -> int;
    };

    struct SocketError : public std::runtime_error {
        using std::runtime_error::runtime_error;
    };
}