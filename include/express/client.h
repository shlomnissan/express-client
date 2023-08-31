// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <future>

#include "express_client_export.h"
#include "express/config.h"
#include "express/response.h"

namespace Express {
    class EXPRESS_CLIENT_EXPORT Client {
    public:
        auto Request(const Config& config) const -> std::future<Response>;
    };
}