// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <express/header.h>

namespace Express::Http {
    Header::Header(std::string_view name, std::string_view value)
        : name_(name), value_(value) {
        // TODO: validate name (raise if invalid)
        // TODO: validate value (raise if invalid)
    }

    HeaderCollection::HeaderCollection(const std::vector<Header>& headers)
        : headers_(headers) {
        for (int i = 0; i < static_cast<int>(size(headers)); ++i) {
            header_pos_[headers[i].name()] = i;
        }
    }

    auto HeaderCollection::add(const Header &header) -> void {
        if (header_pos_.find(header.name()) == std::end(header_pos_)) {
            headers_.emplace_back(header);
            header_pos_[header.name()] = static_cast<int>(size(headers_));
        } else {
            auto idx = header_pos_[header.name()];
            headers_[idx].value_ = header.value();
        }
    }
}