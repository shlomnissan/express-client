// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "express_client_export.h"

#include "express/headers.h"

namespace Express {
    struct EXPRESS_CLIENT_EXPORT Response {
        int status_code;
        std::string status_text;
        std::string data;
        Headers headers;
    };
}