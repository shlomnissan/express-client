// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <string_view>

#include <express/request_config.h>
#include <express/http_defs.h>

namespace Express {
    class Client {
    public:
        auto request(const Http::RequestConfig& config) const -> void;
        auto get(std::string_view url) const -> void;
        auto post(std::string_view url) const -> void;

    private:
        auto prepareRequestWithNoData(
            const Http::Method method,
            std::string_view url
        ) const -> void;

        auto prepareRequestWithData(
            const Http::Method method,
            std::string_view url
        ) const -> void;
    };
}