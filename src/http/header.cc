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
        for (const auto& h : headers) {
            existing_headers_.emplace(h.name());
        }
    }

    auto HeaderCollection::add(const Header &header) -> void {
        if (existing_headers_.find(header.name()) != std::end(existing_headers_)) {
            for (auto& h : headers_) {
                if (h.name() == header.name()) {
                    h.value_ = header.value();
                    return;
                }
            }
        } else {
            headers_.emplace_back(header);
            existing_headers_.emplace(header.name());
        }
    }
}