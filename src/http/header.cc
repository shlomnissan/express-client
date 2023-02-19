// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <express/transformers.h>
#include <express/header.h>
#include <express/http_defs.h>
#include <express/validators.h>

namespace Express::Http {
    using namespace Validators;
    using namespace Transformers;

    Header::Header(std::string_view name, std::string_view value) : name_(name), value_(value) {
        trim_trailing_whitespaces(value_);
        trim_leading_whitespaces(value_);

        if (name_.empty()) {
            throw HeaderError {"Invalid header name."};
        }

        if (!is_token_range(name)) {
            throw HeaderError {"Invalid header name."};
        }

        if (!is_valid_char_range(value)) {
            throw HeaderError {"Invalid header value."};
        }
    }

    HeaderCollection::HeaderCollection(const std::vector<Header>& headers)
        : headers_(headers) {
        for (size_t i = 0; i < headers.size(); ++i) {
            auto lowercase_name = str_to_lower(headers[i].name());
            existing_headers_[lowercase_name] = i;
        }
    }

    auto HeaderCollection::add(const Header &header) -> void {
        auto lowercase_name = str_to_lower(header.name());
        if (existing_headers_.find(lowercase_name) != std::end(existing_headers_)) {
            headers_[existing_headers_[lowercase_name]] = header;
        } else {
            headers_.emplace_back(header);
            existing_headers_[lowercase_name] = headers_.size() - 1;
        }
    }

    auto HeaderCollection::has(const std::string& name) const -> bool {
        auto lowercase_name = str_to_lower(name);
        if (existing_headers_.find(lowercase_name) != std::end(existing_headers_)) {
            return true;
        }
        return false;
    }

    auto HeaderCollection::remove(const std::string& name) -> void {
        auto lowercase_name = str_to_lower(name);
        auto iterator = existing_headers_.find(lowercase_name);
        if (iterator == std::end(existing_headers_)) {
            throw HeaderError {"Attempting to remove a header that doesn't exist"};
        }

        auto idx = iterator->second;
        auto last_idx = headers_.size() - 1;
        if (idx != last_idx) {
            std::swap(headers_[idx], headers_[last_idx]);
            lowercase_name = str_to_lower(headers_[idx].name());
            existing_headers_[lowercase_name] = idx;
        }

        existing_headers_.erase(iterator);
        headers_.pop_back();
    }

    auto HeaderCollection::get(const std::string& name) -> std::string {
        auto lowercase_name = str_to_lower(name);
        if (existing_headers_.find(lowercase_name) != std::end(existing_headers_)) {
            return headers_[existing_headers_[lowercase_name]].value();
        }
        throw HeaderError {"Attempting to access value for a header that doesn't exist."};
    }

    auto operator<<(std::ostream& os, const Header& header) -> std::ostream& {
        os << header.name() + ": " + header.value() << crlf;
        return os;
    }
}