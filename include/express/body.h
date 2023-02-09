// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <sstream>
#include <string>
#include <string_view>

namespace Express::Http {
    class Body {
    public:
        Body() = default;
        Body(std::string_view data);

        [[nodiscard]] auto str() const -> std::string;

    private:
        std::stringstream data_;
    };
}