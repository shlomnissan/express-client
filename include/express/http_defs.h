// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <ostream>

namespace Express::Http {
#define HTTP_METHODS                                                                               \
    METHOD(Options, "OPTIONS")                                                                     \
    METHOD(Get, "GET")                                                                             \
    METHOD(Post, "POST")                                                                           \
    METHOD(Head, "HEAD")                                                                           \
    METHOD(Put, "PUT")                                                                             \
    METHOD(Patch, "PATCH")                                                                         \
    METHOD(Delete, "DELETE")

    enum class Method {
#define METHOD(m, _) m,
        HTTP_METHODS
#undef METHOD
    };

    auto methodString(Method method);

    auto operator<<(std::ostream& os, Method method) -> std::ostream&;

    template <class CharT, class Traits>
    std::basic_ostream<CharT, Traits>& crlf(std::basic_ostream<CharT, Traits>& os) {
        static constexpr char CRLF[] = {0xD, 0xA};
        os.write(CRLF, 2);
        return os;
    }
}