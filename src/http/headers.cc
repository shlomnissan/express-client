// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include "headers.h"

#include "client/error.h"
#include "http/validators.h"
#include "utils/string_transformers.h"

namespace Express {
    using namespace Http::Validators;
    using namespace StringTransformers;

    Headers::Headers(const std::vector<string_pair>& headers) {
        for (const auto& [name, value] : headers) Add(name, value);
    }

    auto Headers::Add(const std::string& name, std::string value) -> void {
        TrimLeadingWhiteSpacesInPlace(value);
        TrimTrailingWhiteSpacesInPlace(value);

        if (name.empty() || !IsTokenRange(name)) {
            Error::Logic("Header error", "Invalid header name");
        }

        if (!IsValidCharRange(value)) {
            Error::Logic("Header error", "Invalid header value");
        }

        headers_.try_emplace(StringToLowerCase(name), name, value);
    }

    auto Headers::Remove(const std::string& name) -> void {
        auto iter = headers_.find(StringToLowerCase(name));
        if (iter == headers_.end()) {
            Error::Logic(
                "Header error",
                "Attempting to remove a header that doesn't exist"
            );
        }
        headers_.erase(iter);
    }

    auto Headers::Get(const std::string& name) -> std::string {
        auto iter = headers_.find(StringToLowerCase(name));
        if (iter == headers_.end()) {
            Error::Logic(
                "Header error",
                "Attempting to access value for a header that doesn't exist"
            );
        }
        return (*iter).second.second;
    }

    auto Headers::Contains(const std::string& name) const -> bool {
        auto iter = headers_.find(StringToLowerCase(name));
        if (iter == headers_.end()) {
            return false;
        }
        return true;
    }
}