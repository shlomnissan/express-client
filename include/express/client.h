// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <string_view>
#include <stdexcept>

#include <express/request.h>
#include <express/body.h>
#include <express/http_defs.h>

namespace Express {
    class ExpressClient {
    public:
        static auto request(const Http::RequestConfig& config) -> void;
    };

    struct ExpressClientError : public std::logic_error {
        using std::logic_error::logic_error;
    };
}