// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <express/data.h>

namespace Express::Http {
    Data::Data(std::string_view data) : data_(data) {}

    Data::Data(const std::vector<FormField>& data) {
        for (const auto& field : data) {
            data_ += field.key() + '=' + field.value() + '&';
        }
        if (!data_.empty()) { data_.pop_back(); }

        content_type_hint_ = "application/x-www-form-urlencoded";

        // TODO: encode URL string
    }

    auto Data::contentType() const -> std::string {
        if (!content_type_hint_.empty()) {
            return content_type_hint_;
        }
        throw DataError {
            "Data was provided for this request, "
            "but the ContentType header isn't set."
        };
    }
}