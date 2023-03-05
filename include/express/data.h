// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <stdexcept>

namespace Express::Http {
    class FormField {
    public:
        FormField(std::string_view key, std::string_view value)
            : key_(key), value_(value) {}

        [[nodiscard]] auto key() const { return key_; }
        [[nodiscard]] auto value() const { return value_; }

    private:
        std::string key_;
        std::string value_;
    };

    class Data {
    public:
        Data() = default;
        Data(std::string_view data);
        Data(const std::vector<FormField>& data);

        [[nodiscard]] auto contentTypeHint() const { return content_type_hint_; }
        [[nodiscard]] auto size() const { return data_.size(); }
        [[nodiscard]] auto data() const { return data_; }

    private:
        std::string data_;
        std::string content_type_hint_;
    };

    struct DataError : public std::logic_error {
        using std::logic_error::logic_error;
    };
}