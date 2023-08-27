// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <stdexcept>

#include "express_client_export.h"

namespace Express {
    struct EXPRESS_CLIENT_EXPORT RequestError : public std::logic_error {
        using std::logic_error::logic_error;
    };

    struct EXPRESS_CLIENT_EXPORT ResponseError : public std::runtime_error {
        using std::runtime_error::runtime_error;
    };
}