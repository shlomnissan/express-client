// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <sstream>
#include <stdexcept>

#include <express/header.h>
#include <express/http_defs.h>
#include <express/url.h>
#include <express/body.h>

namespace Express::Http {
    struct RequestConfig {
        std::string_view url;
        Method method = Method::Get;
        const Body& body = {""};
        HeaderCollection headers = {};
    };

    class Request {
    public:
        Request(const Net::URL& url, const RequestConfig& config);

        auto writeRequest(std::stringstream& buffer) const -> void;

    private:
        RequestConfig config_;
        Net::URL url_;
    };

    struct RequestError : public std::logic_error {
        using std::logic_error::logic_error;
    };
}