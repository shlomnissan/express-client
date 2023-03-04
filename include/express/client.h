// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <string_view>
#include <stdexcept>

#include <express/request.h>
#include <express/response.h>

namespace Express {
    class ExpressClient {
    public:
        static auto request(const Http::RequestConfig& config) -> Http::Response;
    };
}