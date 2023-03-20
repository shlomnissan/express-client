// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <stdexcept>
#include <memory>
#include <string_view>

#if defined(_WIN32)
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <netdb.h>
    #include <sys/socket.h>
#endif

namespace Express::Net {
    class Endpoint {
    public:
        Endpoint(std::string_view host, std::string_view port);

        [[nodiscard]] auto family() const { return address_->ai_family; }
        [[nodiscard]] auto socketType() const { return address_->ai_socktype; }
        [[nodiscard]] auto protocol() const { return address_->ai_protocol; }
        [[nodiscard]] auto address() const { return address_->ai_addr; }
        [[nodiscard]] auto addressLength() const { return address_->ai_addrlen; }

    private:
        struct addrinfo_deleter {
            void operator()(addrinfo* address) const {
                freeaddrinfo(address);
            }
        };
        std::unique_ptr<addrinfo, addrinfo_deleter> address_ {nullptr};
    };

    struct AddressError : public std::runtime_error {
        using std::runtime_error::runtime_error;
    };
}