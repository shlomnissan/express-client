// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <express/url.h>

namespace Express::Net {
    URL::URL(std::string_view url) : source_(url) {
        parseURL(source_);
    }

    auto URL::parseURL(std::string_view url) -> void {
        if (!url.starts_with("http://")) {
            throw URLError {
                "Unsupported URL scheme. "
                "The client currently supports only HTTP."
            };
        }

        scheme_ = "http";
        port_ = "80";

        auto authority_begin = cbegin(url) + scheme_.size() + 3;
        auto authority_end = std::find_if(authority_begin, cend(url), [](char c) {
            return c == '/' || c == '?' || c == '#'; // authority delimiters
        });

        authority_ = std::string {authority_begin, authority_end};
        processAuthority();

        path_ = std::string {authority_end, cend(url)};
        processPath();
    }

    auto URL::processAuthority() -> void {
        host_ = authority_;

        // User information
        auto host_pos = authority_.find('@');
        if (host_pos != std::string::npos) {
            auto user_info = authority_.substr(0, host_pos);
            auto password_pos = user_info.find(':');
            if (password_pos != std::string::npos) {
                user_information_.username = user_info.substr(0, password_pos);
                user_information_.password = user_info.substr(password_pos + 1);
            } else {
                user_information_.username = user_info;
            }
            host_.erase(0, host_pos + 1);
        }

        // Port
        auto port_pos = host_.find(':');
        if (port_pos != std::string::npos) {
            port_ = host_.substr(port_pos + 1);
            host_.resize(port_pos);
        }
    }

    auto URL::processPath() -> void {
        if (path_.empty()) return;
        if (path_.starts_with("/")) {
            path_.erase(0, 1);
        }

        // Skip fragment if needed
        auto fragment_pos = path_.find('#');
        if (fragment_pos != std::string::npos) {
            path_.resize(fragment_pos);
        }

        // Query params
        auto query_pos = path_.find('?');
        if (query_pos != std::string::npos) {
            query_ = path_.substr(query_pos + 1);
            path_.resize(query_pos);
        }
    }
}