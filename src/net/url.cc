// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include "url.h"

#include "client/error.h"

namespace Express::Net {
    Url::Url(std::string_view url) : source_(url) {
        ParseURL(source_);
    }

    auto Url::ParseURL(std::string_view url) -> void {
        auto idx = url.find("://");
        if (idx == std::string::npos) {
            Error::Logic("URL error", "Missing URL scheme. Use 'http://' or 'https://'");
        }

        scheme_ = url.substr(0, idx);
        if (scheme_ != "http" && scheme_ != "https") {
            Error::Logic("URL error", "Unsupported URL scheme. Use 'http://' or 'https://'");
        }

        port_ = (scheme_ == "http") ?
            std::to_string(kDefaultPortHTTP) :
            std::to_string(kDefaultPortHTTPs);

        auto authority_begin = cbegin(url) + scheme_.size() + 3;
        auto authority_end = std::find_if(authority_begin, cend(url), [](char c) {
            return c == '/' || c == '?' || c == '#'; // authority delimiters
        });

        authority_ = std::string {authority_begin, authority_end};
        ProcessAuthority();

        path_ = std::string {authority_end, cend(url)};
        ProcessPath();
    }

    auto Url::ProcessAuthority() -> void {
        host_ = authority_;

        if (auto host_pos = authority_.find('@'); host_pos != std::string::npos) {
            auto user_info = authority_.substr(0, host_pos);
            if (auto pass_pos = user_info.find(':'); pass_pos != std::string::npos) {
                user_ = user_info.substr(0, pass_pos);
                password_ = user_info.substr(pass_pos + 1);
            } else {
                user_ = user_info;
            }
            host_.erase(0, host_pos + 1);
        }

        auto port_pos = host_.find(':');
        if (port_pos != std::string::npos) {
            port_ = host_.substr(port_pos + 1);
            host_.resize(port_pos);
        }
    }

    auto Url::ProcessPath() -> void {
        if (path_.empty()) return;
        if (path_.starts_with("/")) {
            path_.erase(0, 1);
        }

        if (auto frag_pos = path_.find('#'); frag_pos != std::string::npos) {
            path_.resize(frag_pos);
        }

        auto query_pos = path_.find('?');
        if (query_pos != std::string::npos) {
            query_ = path_.substr(query_pos + 1);
            path_.resize(query_pos);
        }
    }
}