// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <ostream>
#include <array>

namespace Express::Http {
#define HTTP_METHODS                                                                               \
    METHOD(Options, "OPTIONS")                                                                     \
    METHOD(Get, "GET")                                                                             \
    METHOD(Head, "HEAD")                                                                           \
    METHOD(Put, "PUT")                                                                             \
    METHOD(Post, "POST")                                                                           \
    METHOD(Patch, "PATCH")                                                                         \
    METHOD(Delete, "DELETE")

    enum class Method {
#define METHOD(m, _) m,
        HTTP_METHODS
#undef METHOD
    };

    auto methodString(Method method);

    auto operator<<(std::ostream& os, Method method) -> std::ostream&;

    static constexpr std::array<char, 2> CRLF {0xD, 0xA};
    static constexpr std::array<char, 4> HCRLF {0xD, 0xA, 0xD, 0xA};

    template <class CharT, class Traits>
    std::basic_ostream<CharT, Traits>& crlf(std::basic_ostream<CharT, Traits>& os) {
        os.write(CRLF.data(), 2);
        return os;
    }
}