// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <memory>
#include <string_view>

#if defined(_WIN32)
    #include "net/winsock.h"
#else
    #include <netdb.h>
#endif

namespace Express::Net {
    struct addrinfo_deleter {
        void operator()(addrinfo* address) const { freeaddrinfo(address); }
    };

    class Endpoint {
    public:
        Endpoint(std::string_view host, std::string_view port);

        [[nodiscard]] auto family() const { return address_->ai_family; }
        [[nodiscard]] auto socket_type() const { return address_->ai_socktype; }
        [[nodiscard]] auto protocol() const { return address_->ai_protocol; }
        [[nodiscard]] auto address() const { return address_->ai_addr; }
        [[nodiscard]] auto address_length() const { return address_->ai_addrlen; }

    private:
        std::unique_ptr<addrinfo, addrinfo_deleter> address_ {nullptr};
    };
}