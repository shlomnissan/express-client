// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <string>
#include <string_view>

namespace Express::StringTransformers {
    [[nodiscard]] auto StringToLowerCase(std::string str) noexcept -> std::string;
    [[nodiscard]] auto Base64Encoding(std::string_view str) noexcept -> std::string;

    auto TrimLeadingWhiteSpacesInPlace(std::string& str) -> void;
    [[nodiscard]] auto TrimLeadingWhiteSpaces(std::string) -> std::string;

    auto TrimTrailingWhiteSpacesInPlace(std::string& str) -> void;
}