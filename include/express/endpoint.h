// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <stdexcept>
#include <netdb.h>
#include <string_view>

namespace Express::Net {
    class Endpoint {
    public:
        Endpoint(std::string_view host, std::string_view port);

        Endpoint(const Endpoint&) = delete;
        auto operator=(const Endpoint&) -> Endpoint& = delete;

        Endpoint(Endpoint&&) = delete;
        auto operator=(Endpoint&&) -> Endpoint& = delete;

        [[nodiscard]] auto getFamily() const { return address_->ai_family; }
        [[nodiscard]] auto getSocketType() const { return address_->ai_socktype; }
        [[nodiscard]] auto getProtocol() const { return address_->ai_protocol; }
        [[nodiscard]] auto getAddress() const { return address_->ai_addr; }
        [[nodiscard]] auto getAddressLength() const { return address_->ai_addrlen; }

        ~Endpoint();
    private:
        addrinfo* address_ = nullptr;
    };

    struct InvalidAddress : public std::runtime_error {
        InvalidAddress()
            : std::runtime_error(
                "Failed to initialize an endpoint. Check your hostname and "
                "ensure the port you're requesting is free."
            ) {}
    };
}