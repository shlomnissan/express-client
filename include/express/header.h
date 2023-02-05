// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <string>
#include <vector>
#include <unordered_map>

namespace Express::Http {
    class Header {
    public:
        friend class HeaderCollection;

        Header(std::string_view name, std::string_view value);

        [[nodiscard]] auto name() const { return name_; }
        [[nodiscard]] auto value() const { return value_; }

    private:
        std::string name_;
        std::string value_;
    };

    class HeaderCollection {
    public:
        HeaderCollection() = default;
        HeaderCollection(const std::vector<Header>& headers);

        auto add(const Header& header) -> void;

        auto begin() { return headers_.begin(); }
        auto begin() const { return headers_.begin(); }
        auto end() { return headers_.end(); }
        auto end() const { return headers_.end(); }

    private:
        std::vector<Header> headers_;
        std::unordered_map<std::string, int> header_pos_;
    };
}