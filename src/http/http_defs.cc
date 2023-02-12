// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <express/http_defs.h>

namespace Express::Http {
    auto methodString(Method method) {
        switch (method) {
#define METHOD(name, str) \
    case Method::name: return str;
            HTTP_METHODS
#undef METHOD
        }
        return "";
    }

    auto operator<<(std::ostream& os, Method method) -> std::ostream& {
        os << methodString(method);
        return os;
    }
}