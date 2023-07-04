// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <express/endpoint.h>

#include <cstring>
#include <system_error>

namespace Express::Net {
    Endpoint::Endpoint(std::string_view host, std::string_view port)
      : host_(host), port_(port) {
        addrinfo hints;
        memset(&hints, 0, sizeof(hints));
        hints.ai_socktype = SOCK_STREAM;

        addrinfo *address_info;
        if (getaddrinfo(host_.c_str(), port_.c_str(), &hints, &address_info)) {
            throw std::system_error {errno, std::system_category(),
                "Endpoint error"
            };
        }

        address_.reset(address_info);
    }
}