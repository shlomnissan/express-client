// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <express/endpoint.h>

#include <cstring>

namespace Express::Net {
    Endpoint::Endpoint(std::string_view host, std::string_view port) {
        addrinfo hints;
        memset(&hints, 0, sizeof(hints));
        hints.ai_socktype = SOCK_STREAM;

        addrinfo *address_info;
        if (getaddrinfo(host.data(), port.data(), &hints, &address_info)) {
            throw AddressError {"Failed to initialize an endpoint."};
        }

        address_.reset(address_info);
    }
}