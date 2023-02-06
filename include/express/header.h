// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <stdexcept>

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

        // TODO: get(std::string_view)

        auto add(const Header& header) -> void;

        [[nodiscard]] auto front() { return headers_.front(); }
        [[nodiscard]] auto back() { return headers_.back(); }

        [[nodiscard]] auto begin() { return headers_.begin(); }
        [[nodiscard]] auto begin() const { return headers_.begin(); }
        [[nodiscard]] auto end() { return headers_.end(); }
        [[nodiscard]] auto end() const { return headers_.end(); }

    private:
        std::vector<Header> headers_;
        std::unordered_set<std::string> existing_headers_;
    };

    struct HeaderError : public std::runtime_error {
        using std::runtime_error::runtime_error;
    };

    auto operator<<(std::ostream& os, const Header& header) -> std::ostream&;
}