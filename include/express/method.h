// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <ostream>

#include "express_client_export.h"

namespace Express {
#define HTTP_METHODS            \
    METHOD(Delete, "DELETE")    \
    METHOD(Get, "GET")          \
    METHOD(Head, "HEAD")        \
    METHOD(Options, "OPTIONS")  \
    METHOD(Patch, "PATCH")      \
    METHOD(Post, "POST")        \
    METHOD(Put, "PUT")

    enum class EXPRESS_CLIENT_EXPORT Method {
#define METHOD(m, _) m,
        HTTP_METHODS
#undef METHOD
    };

    auto MethodToString(Method method) -> const char *;

    EXPRESS_CLIENT_EXPORT std::ostream& operator<<(std::ostream& os, Method method);
}