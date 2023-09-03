// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include "client/timeout.h"
#include "net/endpoint.h"

namespace Express::Net {
    #ifndef _WIN32
        using SOCKET = int;
        constexpr auto INVALID_SOCKET = -1;
    #endif

    enum class EventType {kToRead, kToWrite};

    class Socket {
    public:
        explicit Socket(Endpoint endpoint);

        // delete copy/move constructor and assignment
        Socket(Socket&& src) = delete;
        auto operator=(Socket&& rhs) = delete;
        Socket(const Socket&) = delete;
        auto operator=(const Socket&) -> Socket& = delete;

        auto Connect(const Timeout& timeout) const -> void;
        auto Send(std::string_view buffer, const Timeout& timeout) const -> size_t;
        auto Recv(unsigned char* buffer, const size_t size, const Timeout& timeout) const -> size_t;

        [[nodiscard]] int Get() const { return sock_; };

        ~Socket();

    private:
        Endpoint ep_;
        SOCKET sock_ = INVALID_SOCKET;

        auto MakeNonBlocking() const -> void;
        auto GetPendingError() const -> int;
        auto Select(EventType event, const Timeout& timeout) const -> int;
    };
}