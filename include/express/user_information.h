// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <string>
#include <string_view>

namespace Express::Net {
    struct UserInformation {
        std::string username;
        std::string password;

        [[nodiscard]] auto empty() const {
            return username.empty() && password.empty();
        }        
    };
}