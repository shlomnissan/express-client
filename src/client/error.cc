// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include "error.h"

#include <cerrno>
#include <sstream>
#include <system_error>

#include "exception.h"

namespace Express::Error {
    std::string FormatString(std::string_view type, std::string_view what_arg) {
        std::stringstream output;
        output << type << ": ";
        output << what_arg;
        return output.str();    
    }

    void System(std::string_view what_arg) {
        throw std::system_error {errno, std::system_category(), what_arg.data()};
    }

    void Logic(std::string_view type, std::string_view what_arg) {
        throw Express::RequestError { FormatString(type, what_arg) };
    }

    void Runtime(std::string_view type, std::string_view what_arg) {
        throw Express::ResponseError { FormatString(type, what_arg) };
    }
}