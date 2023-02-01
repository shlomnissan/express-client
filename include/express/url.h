// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <string>
#include <string_view>
#include <stdexcept>

namespace Express::Net {
    class URL {
        public:
            explicit URL(std::string_view url);

            URL(const URL&) = default;
            auto operator=(const URL&) -> URL& = default;

            URL(URL&&) = default;
            auto operator=(URL&&) -> URL& = default;

            [[nodiscard]] auto scheme() const { return scheme_; }
            [[nodiscard]] auto host() const { return host_; }
            [[nodiscard]] auto port() const { return port_; }
            [[nodiscard]] auto path() const { return path_; }
            [[nodiscard]] auto query() const { return query_; }
            [[nodiscard]] auto fragment() const { return fragment_; }

        private:
            std::string scheme_;
            std::string host_;
            std::string port_;
            std::string path_;
            std::string query_;
            std::string fragment_;

            auto parseURL(std::string_view url) -> void;
    };

    struct URLError : public std::logic_error { using logic_error::logic_error; };
}