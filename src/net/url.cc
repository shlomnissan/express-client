// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <iostream>
#include <algorithm>

#include <express/url.h>

namespace Express::Net {
    URL::URL(std::string_view url) {
        parseURL(url);
    }

    auto URL::parseURL(std::string_view url) -> void{
        auto iter = std::begin(url);
        if (url.empty() || !std::isalnum(*iter)) {
            throw URLError {"Invalid URL"};
        }

        auto is_end = [iter, url]() { return iter == std::end(url); };

        // RFC 3986, 3.1. Scheme
        while (!is_end() && (std::isalnum(*iter) || *iter == '+' || *iter == '-' || *iter == '.')) {
            scheme_.push_back(*iter++);
        }
        if (is_end() || *iter++ != ':') { throw URLError {"Invalid URL scheme"}; }
        if (is_end() || *iter++ != '/') { throw URLError {"Invalid URL scheme"}; }
        if (is_end() || *iter++ != '/') { throw URLError {"Invalid URL scheme"}; }

        // RFC 3986, 3.2. Authority
        const auto authority = std::string(iter, std::end(url));
    }
}