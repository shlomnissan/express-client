// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include "method.h"

namespace Express {
    auto MethodToString(Method method) -> const char * {
        switch (method) {
#define METHOD(name, str) \
    case Method::name: return str;
            HTTP_METHODS
#undef METHOD
        }
        return "";
    }

    auto operator<<(std::ostream& os, Method method) -> std::ostream& {
        os << MethodToString(method);
        return os;
    }
}