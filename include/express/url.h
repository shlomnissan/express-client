// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <string>
#include <string_view>
#include <stdexcept>

#include <express/user_information.h>

namespace Express::Net {
    static constexpr auto kDefaultPortHTTP = 80;
    static constexpr auto kDefaultPortHTTPs = 443;

    class URL {
        public:
            explicit URL(std::string_view url);

            [[nodiscard]] auto scheme() const { return scheme_; }
            [[nodiscard]] auto authority() const { return authority_; }
            [[nodiscard]] auto host() const { return host_; }
            [[nodiscard]] auto userInformation() const { return user_information_; }
            [[nodiscard]] auto port() const { return port_; }
            [[nodiscard]] auto path() const { return path_; }
            [[nodiscard]] auto query() const { return query_; }
            [[nodiscard]] auto source() const { return source_; }

        private:
            std::string scheme_;
            std::string authority_;
            std::string host_;
            std::string port_;
            std::string path_;
            std::string query_;
            std::string source_;

            UserInformation user_information_;

            auto parseURL(std::string_view url) -> void;
            auto processAuthority() -> void;
            auto processPath() -> void;
    };

    struct URLError : public std::logic_error {
        using logic_error::logic_error;
    };
}