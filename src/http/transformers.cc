// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <algorithm>

#include <express/transformers.h>

namespace Express::Http::Transformers {
    using namespace Validators;

    auto str_to_lower(std::string_view str) -> std::string {
        std::string output {str};
        std::ranges::transform(output, begin(output), [](auto c) {
            return tolower(c);
        });
        return output;
    }

    auto trim_leading_whitespaces(std::string& str) noexcept -> void {
        size_t i = 0;
        while (i < str.size() && is_whitespace(str[i])) ++i;
        if (i != 0) str.erase(0, i);
    }

    auto trim_trailing_whitespaces(std::string& str) noexcept -> void {
        size_t i = size(str) - 1;
        while (i != 0 && is_whitespace(str[i])) --i;
        if (i != size(str) - 1) str.erase(i + 1);
    }
}