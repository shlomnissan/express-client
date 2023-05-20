// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <string>
#include <string_view>

#include <express/validators.h>

namespace Express::Transformers {
    static constexpr auto chars =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz"
            "0123456789+/";

    [[nodiscard]] auto str_to_lower(std::string_view str) noexcept -> std::string;
    [[nodiscard]] auto trim_leading_whitespaces(std::string_view str) noexcept -> std::string;
    [[nodiscard]] auto base64_encode(std::string_view data) -> std::string;

    auto trim_leading_whitespaces_in_place(std::string& str) noexcept -> void; 
    auto trim_trailing_whitespaces_in_place(std::string& str) noexcept -> void;
}