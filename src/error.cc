// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <express/error.h>
#include <express/exceptions.h>

#include <cerrno>
#include <stdexcept>
#include <system_error>
#include <sstream>

namespace Express::Error {
    std::string format_string(std::string_view type, std::string_view what_arg) {
        std::stringstream output;
        output << type << ": ";
        output << what_arg;
        return output.str();    
    }

    void system(std::string_view what_arg) {
        throw std::system_error {errno, std::system_category(), what_arg.data()};
    } 

    void logic(std::string_view type, std::string_view what_arg) {
        throw Express::RequestError { format_string(type, what_arg) };
    }

    void runtime(std::string_view type, std::string_view what_arg) {
        throw Express::ResponseError { format_string(type, what_arg) };
    }
}