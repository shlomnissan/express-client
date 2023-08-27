// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <string>
#include <map>
#include <vector>

namespace Express {
    using string_pair = std::pair<std::string, std::string>;

    class Headers {
    public:
        Headers() = default;
        explicit(false) Headers(const std::vector<string_pair>& headers);

        auto Add(const std::string& name, std::string value) -> void;
        auto Remove(const std::string& name) -> void;

        [[nodiscard]] auto Contains(const std::string& name) const -> bool;
        [[nodiscard]] auto Get(const std::string& name) -> std::string;

        [[nodiscard]] auto begin() { return headers_.begin(); }
        [[nodiscard]] auto end() { return headers_.end(); }

    private:
        std::map<std::string, string_pair, std::less<>> headers_;
    };
}