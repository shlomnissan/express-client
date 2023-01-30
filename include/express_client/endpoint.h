// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#ifndef EXPRESS_ENDPOINT_H
#define EXPRESS_ENDPOINT_H

#include <stdexcept>
#include <netdb.h>
#include <string_view>

namespace Express::Net {
    class Endpoint {
    public:
        Endpoint(std::string_view host, std::string_view port);

        // Delete copy and move operations
        Endpoint(const Endpoint&) = delete;
        auto operator=(const Endpoint&) -> Endpoint& = delete;
        Endpoint(Endpoint&& rhs) = delete;
        auto operator=(Endpoint&& rhs) -> Endpoint& = delete;

        [[nodiscard]] auto getFamily() const { return address->ai_family; }
        [[nodiscard]] auto getSocketType() const { return address->ai_socktype; }
        [[nodiscard]] auto getProtocol() const { return address->ai_protocol; }
        [[nodiscard]] auto getAddress() const { return address->ai_addr; }
        [[nodiscard]] auto getAddressLength() const { return address->ai_addrlen; }

        ~Endpoint();
    private:
        addrinfo* address = nullptr;
    };

    struct InvalidAddress : public std::runtime_error {
        InvalidAddress()
            : std::runtime_error(
                "Failed to initialize an endpoint. Check your hostname and "
                "ensure the port you're requesting is free."
            ) {}
    };
}

#endif  // EXPRESS_ENDPOINT_H