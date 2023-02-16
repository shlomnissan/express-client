// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

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

    auto operator<<(std::ostream& os, const Header& header) -> std::ostream&;

    class HeaderCollection {
    public:
        HeaderCollection() = default;
        HeaderCollection(const std::vector<Header>& headers);

        auto add(const Header& header) -> void;
        auto has(const std::string& name) const -> bool;
        auto get(const std::string& name) -> std::string;

        [[nodiscard]] auto size() const { return headers_.size(); }
        [[nodiscard]] auto front() { return headers_.front(); }
        [[nodiscard]] auto back() { return headers_.back(); }
        [[nodiscard]] auto begin() { return headers_.begin(); }
        [[nodiscard]] auto begin() const { return headers_.begin(); }
        [[nodiscard]] auto end() { return headers_.end(); }
        [[nodiscard]] auto end() const { return headers_.end(); }

    private:
        std::vector<Header> headers_;
        std::unordered_map<std::string, size_t> existing_headers_;
    };

    struct HeaderError : public std::runtime_error {
        using std::runtime_error::runtime_error;
    };
}