// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <algorithm>

#include <express/transformers.h>
#include <express/validators.h>

namespace Express::Http::Transformers {
    using namespace Validators;

    auto trim_leading_whitespaces(std::string* str_ptr) noexcept -> void {
        auto& str = *str_ptr;
        size_t i = 0;
        while (i < str.size() && is_whitespace(str[i])) ++i;
        if (i != 0) str.erase(0, i);
    }

    auto trim_trailing_whitespaces(std::string* str_ptr) noexcept -> void {
        auto& str = *str_ptr;
        size_t n = size(str) - 1;
        size_t i = n;
        while (i != 0 && is_whitespace(str[i])) --i;
        if (i != n) str.erase(i + 1);
    }

    auto str_to_lowercase(std::string* str_ptr) noexcept -> std::string {
        auto& str = *str_ptr;
        std::ranges::transform(str, begin(str), [](auto c) mutable {
            return tolower(c);
        });
        return str;
    }
}