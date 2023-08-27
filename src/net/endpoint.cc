// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include "endpoint.h"

#include <cstring>

#include "client/error.h"

namespace Express::Net {
    using std::string_view;

    Endpoint::Endpoint(string_view host, string_view port) {
        addrinfo hints;
        memset(&hints, 0, sizeof(hints));
        hints.ai_socktype = SOCK_STREAM;

        addrinfo *address_info;
        if (getaddrinfo(host.data(), port.data(), &hints, &address_info)) {
            Error::System("Endpoint error");
        }

        address_.reset(address_info);
    }
}