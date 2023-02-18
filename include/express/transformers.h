// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <algorithm>
#include <string>

namespace Express::Http::Transformers {
    auto trim_leading_whitespaces(std::string* str_ptr) noexcept -> void; 
    auto trim_trailing_whitespaces(std::string* str_ptr) noexcept -> void;

    auto to_lowercase(auto str) {
        std::ranges::transform(str, begin(str), [](auto c) mutable {
            return tolower(c);
        });
        return str;
    }
}