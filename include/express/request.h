// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <sstream>
#include <string>
#include <chrono>

#include <express/request_config.h>
#include <express/url.h>

namespace Express::Http {
    using Express::Net::URL;

    class Request {
    public:
        Request(const RequestConfig& config, const URL& url);

        auto writeRequest(std::stringstream& buffer) const -> void;

    private:
        RequestConfig config_;
        URL url_;
    };
}