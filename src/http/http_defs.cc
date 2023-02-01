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

    auto statusCodeString(StatusCode code) {
        switch (code) {
#define STATUS_CODE(_, name, str) \
    case StatusCode::name: return str;
            STATUS_CODES
#undef STATUS_CODE
        }
        return "";
    }

    auto operator<<(std::ostream& os, Method method) -> std::ostream& {
        os << methodString(method);
        return os;
    }

    auto operator<<(std::ostream& os, StatusCode code) -> std::ostream& {
        os << statusCodeString(code);
        return os;
    }
}