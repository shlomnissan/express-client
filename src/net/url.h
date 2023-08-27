// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <string>
#include <string_view>

namespace Express::Net {
    static constexpr auto kDefaultPortHTTP = 80;
    static constexpr auto kDefaultPortHTTPs = 443;

    class Url {
        public:
            explicit Url(std::string_view url);

            [[nodiscard]] auto host() const { return host_; }
            [[nodiscard]] auto password() const { return password_; }
            [[nodiscard]] auto path() const { return path_; }
            [[nodiscard]] auto port() const { return port_; }
            [[nodiscard]] auto query() const { return query_; }
            [[nodiscard]] auto scheme() const { return scheme_; }
            [[nodiscard]] auto source() const { return source_; }
            [[nodiscard]] auto user() const { return user_; }

            [[nodiscard]] auto HasUserInformation() const {
                return !user_.empty() || !password_.empty();
            }

        private:
            std::string authority_;
            std::string host_;
            std::string password_;
            std::string path_;
            std::string port_;
            std::string query_;
            std::string scheme_;
            std::string source_;
            std::string user_;

            auto ParseURL(std::string_view url) -> void;
            auto ProcessAuthority() -> void;
            auto ProcessPath() -> void;
    };
}