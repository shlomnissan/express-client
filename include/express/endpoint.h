// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <stdexcept>
#include <memory>
#include <string>

#include <express/socket_defs.h>

namespace Express::Net {
    struct addrinfo_deleter {
        void operator()(addrinfo* address) const { freeaddrinfo(address); }
    };

    class Endpoint {
    public:
        Endpoint(const std::string& host, const std::string& port);

        [[nodiscard]] auto family() const { return address_->ai_family; }
        [[nodiscard]] auto socketType() const { return address_->ai_socktype; }
        [[nodiscard]] auto protocol() const { return address_->ai_protocol; }
        [[nodiscard]] auto address() const { return address_->ai_addr; }
        [[nodiscard]] auto addressLength() const { return address_->ai_addrlen; }
        [[nodiscard]] auto host() const { return host_; }

    private:
        std::string host_;
        std::string port_;
        std::unique_ptr<addrinfo, addrinfo_deleter> address_ {nullptr};
    };

    struct AddressError : public std::runtime_error {
        using std::runtime_error::runtime_error;
    };
}