// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <algorithm>

#include <express/transformers.h>

namespace Express::Transformers {
    using namespace Http::Validators;

    auto str_to_lower(std::string_view str) noexcept -> std::string {
        std::string output;
        output.reserve(str.size());
        for (const auto c : str) {
            output += static_cast<char>(tolower(c));
        }
        return output;
    }

    auto trim_leading_whitespaces_in_place(std::string& str) noexcept -> void {
        size_t i = 0;
        while (i < str.size() && is_whitespace(str[i])) ++i;
        if (i != 0) str.erase(0, i);
    }

    auto trim_trailing_whitespaces_in_place(std::string& str) noexcept -> void {
        size_t i = size(str) - 1;
        while (i != 0 && is_whitespace(str[i])) --i;
        if (i != size(str) - 1) str.erase(i + 1);
    }

    auto trim_leading_whitespaces(std::string_view str) noexcept -> std::string {
        std::string output {str};
        trim_leading_whitespaces_in_place(output);
        return output;
    }

    auto base64_encode(std::string_view data) -> std::string {
        size_t len = size(data);
        size_t i = 0;
        
        std::string output;
        output.reserve((len + 2) / 3 * 4);

        while (i < len) {
            output += chars[data[i] >> 2];
            if (i + 1 < len) {
                output += chars[((data[i] & 0x03) << 4) + (data[i + 1] >> 4)];
                if (i + 2 < len) {
                    output += chars[((data[i + 1] & 0x0F) << 2) + (data[i + 2] >> 6)];
                    output += chars[data[i + 2] & 0x3F];
                } else {
                    output += chars[(data[i + 1] & 0x0F) << 2];
                    output += '=';
                }
            } else {
                output += chars[(data[i] & 0x03) << 4];
                output += '=';
                output += '=';
            }
            i += 3;
        }

        return output;
    }
}