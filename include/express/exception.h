// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <stdexcept>

namespace Express {
    struct RequestError : public std::logic_error {
        using std::logic_error::logic_error;
    };

    struct ResponseError : public std::runtime_error {
        using std::runtime_error::runtime_error;
    };
}