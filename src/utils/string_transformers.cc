// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include "utils/string_transformers.h"

namespace Express::StringTransformers {
    static constexpr auto chars =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz"
            "0123456789+/";

    constexpr auto IsWhiteSpace = [](char c) { return c == 0x20 || c == 0x09; };

    auto StringToLowerCase(std::string str) noexcept -> std::string {
        for (auto& c : str) c = static_cast<char>(tolower(c));
        return str;
    }

    auto TrimLeadingWhiteSpacesInPlace(std::string& str) -> void {
        auto i = 0;
        while (i < static_cast<int>(str.size()) && IsWhiteSpace(str[i])) ++i;
        if (i != 0) str.erase(0, i);
    }

    auto TrimLeadingWhiteSpaces(std::string str) -> std::string {
        TrimLeadingWhiteSpacesInPlace(str);
        return str;
    }

    auto TrimTrailingWhiteSpacesInPlace(std::string& str) -> void {
        auto i = static_cast<int>(size(str)) - 1;
        while (i >= 0 && IsWhiteSpace(str[i])) --i;
        if (i != static_cast<int>(size(str)) - 1) str.erase(i + 1);
    }

    auto Base64Encoding(std::string_view str) noexcept -> std::string {
        size_t len = size(str);
        size_t i = 0;
        std::string output;
        output.reserve((len + 2) / 3 * 4);
        while (i < len) {
            output += chars[str[i] >> 2];
            if (i + 1 < len) {
                output += chars[((str[i] & 0x03) << 4) + (str[i + 1] >> 4)];
                if (i + 2 < len) {
                    output += chars[((str[i + 1] & 0x0F) << 2) + (str[i + 2] >> 6)];
                    output += chars[str[i + 2] & 0x3F];
                } else {
                    output += chars[(str[i + 1] & 0x0F) << 2];
                    output += '=';
                }
            } else {
                output += chars[(str[i] & 0x03) << 4];
                output += '=';
                output += '=';
            }
            i += 3;
        }
        return output;
    }
}