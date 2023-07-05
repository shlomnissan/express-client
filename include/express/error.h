// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <string_view>

namespace Express::Error {
    [[noreturn]] void system(std::string_view what_arg);
    [[noreturn]] void logic(std::string_view type, std::string_view what_arg);
    [[noreturn]] void runtime(std::string_view type, std::string_view what_arg);
}