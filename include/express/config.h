// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <string_view>
#include <chrono>

#include "express_client_export.h"

#include "express/headers.h"
#include "express/method.h"
#include "express/user_auth.h"

namespace Express {
    struct EXPRESS_CLIENT_EXPORT Config {
        std::string_view url;
        Method method {Method::Get};
        Headers headers {};
        std::string_view data;
        UserAuth auth {};
        std::chrono::milliseconds timeout {0};
    };
}