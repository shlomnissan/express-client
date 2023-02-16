// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <express/transformers.h>
#include <express/header.h>
#include <express/http_defs.h>
#include <express/validators.h>

namespace Express::Http {
    Header::Header(std::string_view name, std::string_view value) : name_(name), value_(value) {
        using namespace Validators;
        using namespace Transformers;

        trim_trailing_whitespaces(&value_);
        trim_leading_whitespaces(&value_);

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
        for (size_t i = 0; i < (headers.size()); ++i) {
            existing_headers_[headers[i].name()] = i;
        }
    }

    auto HeaderCollection::add(const Header &header) -> void {
        if (existing_headers_.find(header.name()) != std::end(existing_headers_)) {
            headers_[existing_headers_[header.name()]] = header;
        } else {
            headers_.emplace_back(header);
            existing_headers_[header.name()] = headers_.size() - 1;
        }
    }

    auto HeaderCollection::has(const std::string& name) const -> bool {
        if (existing_headers_.find(name) != std::end(existing_headers_)) {
            return true;
        }
        return false;
    }

    auto HeaderCollection::get(const std::string& name) -> std::string {
        if (existing_headers_.find(name) != std::end(existing_headers_)) {
            return headers_[existing_headers_[name]].value();
        }
        throw HeaderError {"Attempting to access value for a header that doesn't exist."};
    }

    auto operator<<(std::ostream& os, const Header& header) -> std::ostream& {
        os << header.name() + ": " + header.value() << crlf;
        return os;
    }
}