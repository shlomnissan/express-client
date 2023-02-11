// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <express/body.h>

namespace Express::Http::Body {
    FormFields::FormFields(const std::vector<FormField>& fields) {
        for (const auto& field : fields) {
            data_ += field.key() + '=' + field.value() + '&';
        }
        if (!data_.empty()) {
            data_.pop_back();
        }
        // TODO: encode the string to URL
    }

    auto FormFields::contentType() const -> std::string {
        return "application/x-www-form-urlencoded";
    }

    auto FormFields::size() const -> std::size_t {
        return data_.size();
    }

    auto FormFields::data() const -> std::string {
        return data_;
    } 
}