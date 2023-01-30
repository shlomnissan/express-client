// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

namespace Express {
    class Client {
    public:
        auto get() const -> int;
    };
}