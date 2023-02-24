// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <string>
#include <string_view>

#include <express/validators.h>

namespace Express::Http::Transformers {
    auto str_to_lower(std::string_view str) -> std::string;

    auto trim_leading_whitespaces(std::string_view str) noexcept -> std::string;
    auto trim_leading_whitespaces(std::string& str) noexcept -> void; 
    auto trim_trailing_whitespaces(std::string& str) noexcept -> void;
}