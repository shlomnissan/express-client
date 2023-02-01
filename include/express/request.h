// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <sstream>
#include <string>
#include <chrono>

#include <express/http_defs.h>

namespace Express::Http {
    class Request {
    public:
        Request(Method method, const std::string& resource);

        Request(const Request& other) = default;
        auto operator=(const Request& other) -> Request& = default;

        Request(Request&& other) = default;
        auto operator=(Request&& other) -> Request& = default;

        auto writeRequest(std::stringstream& buffer) const -> void;

    private:
        Method method_;
        std::string resource_;
    };
}