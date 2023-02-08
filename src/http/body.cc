// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <express/body.h>

namespace Express::Http {
    Body::Body(std::string_view data) {
        // TODO: sanitize data
        data_ << data;
    }

    auto Body::str() const -> std::string {
        return data_.str();
    }
}