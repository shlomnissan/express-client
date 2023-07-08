// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <express/header.h>

namespace Express {
    struct Response {
        int statusCode;
        std::string statusText;
        std::vector<uint8_t> data;
        Http::HeaderCollection headers;
    };
}