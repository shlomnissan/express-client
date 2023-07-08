// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <future>
#include <stdexcept>
#include <memory>

#include <express/error.h>
#include <express/socket.h>
#include <express/request.h>
#include <express/response_parser.h>
#include <express/url.h>

namespace Express {
    using namespace Net;
    using namespace Http;

    class Client {
    public:
        static auto request(const RequestConfig& config) -> std::future<Response>;

    private:
        static auto makeRequest(const RequestConfig& config) -> Response;
        static auto getSocket(const URL& url) -> std::unique_ptr<Socket>; 
    };
}