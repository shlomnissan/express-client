// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <ostream>
#include <array>

namespace Express::Http {
    static constexpr std::array<char, 2> CRLF {0xD, 0xA};
    static constexpr std::array<char, 4> HCRLF {0xD, 0xA, 0xD, 0xA};

    template <class CharT, class Traits>
    std::basic_ostream<CharT, Traits>& crlf(std::basic_ostream<CharT, Traits>& os) {
        os.write(CRLF.data(), 2);
        return os;
    }
}