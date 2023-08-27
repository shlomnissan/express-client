// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <string>
#include <string_view>

namespace Express::Http {
    class StatusLine {
    public:
        explicit StatusLine(std::string_view status_line);

        [[nodiscard]] auto code() const { return code_; }
        [[nodiscard]] auto text() const { return text_; }

    private:
        int code_;
        std::string status_line_;
        std::string text_;
    };
}