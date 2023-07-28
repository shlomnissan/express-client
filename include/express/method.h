// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <ostream>

namespace Express {
#define HTTP_METHODS            \
    METHOD(Delete, "DELETE")    \
    METHOD(Get, "GET")          \
    METHOD(Head, "HEAD")        \
    METHOD(Options, "OPTIONS")  \
    METHOD(Patch, "PATCH")      \
    METHOD(Post, "POST")        \
    METHOD(Put, "PUT")

    enum class Method {
#define METHOD(m, _) m,
        HTTP_METHODS
#undef METHOD
    };

    auto method_to_string(Method method);

    auto operator<<(std::ostream& os, Method method) -> std::ostream&;
}