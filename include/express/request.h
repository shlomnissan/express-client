// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <chrono>
#include <sstream>
#include <string>
#include <stdexcept>

#include <express/header.h>
#include <express/http_defs.h>
#include <express/url.h>
#include <express/data.h>
#include <express/user_information.h>

namespace Express::Http {
    struct RequestConfig {
        std::string_view url;
        Method method {Method::Get};
        Data data {};
        HeaderCollection headers {};
        Net::UserInformation auth {};
        std::chrono::milliseconds timeout {0};
    };

    class Request {
    public:
        Request(const Net::URL& url, const RequestConfig& config);

        [[nodiscard]] auto str() const { return buffer_.str(); }
        [[nodiscard]] auto timeout() const { return config_.timeout; } 

    private:
        std::stringstream buffer_;
        RequestConfig config_;
        Net::URL url_;

        auto setHeaders() -> void;
        auto setContentTypeAndSize() -> void;
        auto setBasicAuth(const Net::UserInformation& user_info) -> void;
        auto writeRequest() -> void;
        auto allowedData(const Method method) const -> bool;
    };
}