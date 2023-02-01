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

        Request(const Request&) = default;
        auto operator=(const Request&) -> Request& = default;

        Request(Request&&) = default;
        auto operator=(Request&&) -> Request& = default;

        auto writeRequest(std::stringstream& buffer) const -> void;

    private:
        Method method_;
        std::string resource_;
    };
}