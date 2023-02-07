// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <sstream>
#include <stdexcept>

#include <express/header.h>
#include <express/http_defs.h>
#include <express/url.h>

namespace Express::Http {
    using Express::Net::URL;

    struct RequestConfig {
        Method method;
        std::string_view url;
        HeaderCollection headers;
    };

    class Request {
    public:
        Request(const URL& url, const RequestConfig& config);

        auto writeRequest(std::stringstream& buffer) const -> void;

    private:
        RequestConfig config_;
        URL url_;
    };

    struct RequestError : public std::logic_error {
        using std::logic_error::logic_error;
    };
}