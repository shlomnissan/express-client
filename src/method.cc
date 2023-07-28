// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <express/method.h>

namespace Express {
    auto method_to_string(Method method) {
        switch (method) {
#define METHOD(name, str) \
    case Method::name: return str;
            HTTP_METHODS
#undef METHOD
        }
        return "";
    }

    auto operator<<(std::ostream& os, Method method) -> std::ostream& {
        os << method_to_string(method);
        return os;
    }
}