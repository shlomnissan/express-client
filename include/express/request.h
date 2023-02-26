// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <sstream>
#include <string>
#include <stdexcept>

#include <express/header.h>
#include <express/http_defs.h>
#include <express/url.h>
#include <express/data.h>

namespace Express::Http {
    struct RequestConfig {
        std::string_view url;
        Method method {Method::Get};
        Data data {};
        HeaderCollection headers {};
    };

    class Request {
    public:
        Request(const Net::URL& url, const RequestConfig& config);

        auto writeRequest(std::stringstream& buffer) const -> void;

    private:
        RequestConfig config_;
        std::string data_;
        Net::URL url_;
    };
}