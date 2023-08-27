// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <string>

#include "express_client_export.h"

namespace Express {
    struct EXPRESS_CLIENT_EXPORT UserAuth {
        std::string username;
        std::string password;

        [[nodiscard]] auto Empty() const {
            return username.empty() && password.empty();
        }
    };
}