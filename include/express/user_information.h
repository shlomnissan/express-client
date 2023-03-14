// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <string>
#include <string_view>

#include <express/url.h>

namespace Express::Net {
    class UserInformation {
        public:
            friend class URL;

            UserInformation() = default;

            UserInformation(std::string_view username, std::string_view password) :
                username_(username),
                password_(password) {}

            [[nodiscard]] auto empty() const {
                return username_.empty() && password_.empty();
            }

            [[nodiscard]] auto username() const { return username_; }
            [[nodiscard]] auto password() const { return password_; }

        private:
            std::string username_;
            std::string password_;
    };
}