// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include "status_line.h"

#include "client/error.h"
#include "http/validators.h"

namespace Express::Http {
    StatusLine::StatusLine(std::string_view status_line)
    : status_line_(status_line) {
        using namespace std::string_literals;

        std::string status {status_line_};

        if (!status.starts_with("HTTP/"s)) {
            Error::Runtime("Status line error", "Malformed status line");
        }

        // html version
        status = status.substr(status.find('/') + 1);
        auto version = status.substr(0, status.find(0x20));
        if (version != "1.0" && version != "1.1") {
            Error::Runtime("Status line error", "Unsupported HTML version (" + version + ")");
        }
        status = status.substr(4);

        // status code
        auto separator = status.find(0x20);
        auto status_code = status.substr(0, separator);
        if (separator != 3 || !Validators::IsDigitRange(status_code)) {
            Error::Runtime("Status line error", "Invalid status code (" + status_code + ")");
        }
        code_ = std::stoi(status_code);
        
        // reason phrase
        auto response_phrase = status.substr(separator + 1);
        if (!Validators::IsValidCharRange(response_phrase)) {
            Error::Runtime("Status line error", "Invalid characters in reason phrase");
        }
        text_ = status.substr(separator + 1);
    }
}