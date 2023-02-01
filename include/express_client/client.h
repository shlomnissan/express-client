// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <string_view>

#include <express_client/http_defs.h>

namespace Express {
    class Client {
    public:
        auto request(Http::Method method, std::string_view url) const -> void;
        auto get(std::string_view url) const -> int;
    };
}