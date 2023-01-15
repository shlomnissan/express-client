// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <simple_http/endpoint.h>

#include <cstring>
#include <netdb.h>
#include <sys/socket.h>

#include <iostream>

namespace simplehttp {
    Endpoint::Endpoint(std::string_view host, std::string_view port) {
        struct addrinfo hints;
        memset(&hints, 0, sizeof(hints));
        hints.ai_socktype = SOCK_STREAM;
        if (getaddrinfo(host.data(), port.data(), &hints, &address)) {
            throw InvalidAddress();
        }
    }

    Endpoint::~Endpoint() {
        if (address != nullptr) {
            freeaddrinfo(address);
        }
    }
}

