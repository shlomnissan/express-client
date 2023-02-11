// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <vector>
#include <string>
#include <string_view>

namespace Express::Http::Body {
    class __Base {
    public:
        __Base() = default;
        virtual auto contentType() const -> std::string { return ""; };
        virtual auto size() const -> std::size_t { return 0; }
        virtual auto data() const -> std::string { return ""; }

        virtual ~__Base() = default; 
    };

    class FormFields : public __Base {
        class FormField;

    public:
        explicit FormFields(const std::vector<FormField>& fields);

        [[nodiscard]] auto contentType() const -> std::string override;
        [[nodiscard]] auto size() const -> std::size_t override;
        [[nodiscard]] auto data() const -> std::string override;

        ~FormFields() override = default; 

    private:
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

        std::string data_;
    };
}