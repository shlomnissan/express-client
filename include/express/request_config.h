// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <string_view>
#include <vector>

#include <express/http_defs.h>
#include <express/header.h>

namespace Express::Http {
    struct RequestConfig {
        Method method;
        std::string_view url;
        HeaderCollection headers;
    };
}
