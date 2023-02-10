// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <ranges>

#include <express/url.h>
#include <express/validators.h>
#include <string_view>

namespace Express::Net {
    URL::URL(std::string_view url) : source_(url) {
        parseURL(source_);
    }

    auto URL::parseURL(std::string_view url) -> void {
        using namespace Http::Validators;

        if (url.empty() || !is_alnum(url[0])) {
            throw URLError {"Invalid URL."};
        }

        // RFC3986, 3.1. Scheme
        auto scheme_pos = std::ranges::find_if_not(url, [](char c) {
            return is_alnum(c) || c == '+' || c == '-' || c == '.'; // scheme characters
        });
        scheme_ = std::string(begin(url), scheme_pos);
        if (std::string_view(scheme_pos, scheme_pos + 3) != "://") {
            throw URLError {"Invalid URL scheme."};
        }
        std::advance(scheme_pos, 3);
        
        // RFC3986, 3.2. Authority
        auto authority_pos = std::find_if(scheme_pos, end(url), [](char c) {
            return c == '/' || c == '?' || c == '#'; // authority delimiters
        });
        authority_ = std::string(scheme_pos, authority_pos);
        processAuthority(authority_);

        // RFC3986, 3.3. Path
        if (authority_pos != end(url)) {
            if (*authority_pos == '/') ++authority_pos;
            path_ = std::string(authority_pos, end(url));

            // RFC3986, 3.5. Fragment
            auto fragment_pos = path_.find('#');
            if (fragment_pos != std::string::npos) {
                fragment_ = path_.substr(fragment_pos + 1);
                path_.resize(fragment_pos);
            }

            // RFC3986, 3.4. Query
            auto query_pos = path_.find('?');
            if (query_pos != std::string::npos) {
                query_ = path_.substr(query_pos + 1);
                path_.resize(query_pos);
            }
        }

        // TODO: scheme to port if not specified
        // https://gist.github.com/mahmoud/2fe281a8daaff26cfe9c15d2c5bf5c8b
    }

    auto URL::processAuthority(std::string_view authority) -> void {
        // RFC3986, 3.2.1. User Information
        auto host_pos = authority.find('@');
        if (host_pos != std::string::npos) {
            auto user_info = authority.substr(0, host_pos);
            auto password_pos = user_info.find(':');
            if (password_pos != std::string::npos) {
                user_information_.username_ = user_info.substr(0, password_pos);
                user_information_.password_ = user_info.substr(password_pos + 1);
            } else {
                user_information_.username_ = user_info;
            }
        }

        // RFC3986, 3.2.2. Host
        host_ = authority.substr(host_pos + 1);
        auto port_pos = host_.find(':');
        if (port_pos != std::string::npos) {

            // RFC3986, 3.2.3. Port
            port_ = host_.substr(port_pos + 1);
            host_.resize(port_pos);
        }
    }
}