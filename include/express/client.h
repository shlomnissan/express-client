// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <future>
#include <stdexcept>
#include <memory>

#include <express/socket.h>
#include <express/request.h>
#include <express/response.h>
#include <express/url.h>

namespace Express {
    using namespace Net;
    using namespace Http;

    class ExpressClient {
    public:
        static auto request(const RequestConfig& config) -> std::future<Response>;

    private:
        static auto makeRequest(const RequestConfig& config) -> Response;
        static auto getSocket(const URL& url) -> std::unique_ptr<Socket>; 
    };

    struct ClientError : public std::logic_error {
        using logic_error::logic_error;
    };
}