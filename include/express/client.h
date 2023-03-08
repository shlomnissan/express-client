// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <future>
#include <string_view>
#include <stdexcept>

#include <express/request.h>
#include <express/url.h>
#include <express/response.h>

namespace Express {
    using namespace Net;
    using namespace Http;

    class ExpressClient {
    public:
        static auto request(const RequestConfig& config) -> std::future<Response>;

    private:
        static auto makeRequest(const RequestConfig& config) -> Response;
    };
}