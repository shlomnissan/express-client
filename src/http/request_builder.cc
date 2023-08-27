// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include "request_builder.h"

#include "client/error.h"
#include "express/method.h"
#include "express/user_auth.h"
#include "express/version.h"
#include "http/defs.h"
#include "utils/string_transformers.h"

namespace Express::Http {
    RequestBuilder::RequestBuilder(const Config& config)
    : config_(config), url_(config.url) {
        SetHeaders();
        WriteToStream();
    }

    auto RequestBuilder::SetHeaders() -> void {
        auto version {
            std::to_string(Version::Major) + "." +
            std::to_string(Version::Minor)
        };

        config_.headers.Add("Host", url_.host());
        config_.headers.Add("User-Agent", "express/" + version);

        if (!config_.auth.Empty() || url_.HasUserInformation()) {
            if (config_.headers.Contains("Authorization")) {
                config_.headers.Remove("Authorization");
            }

            auto auth = config_.auth;
            if (auth.Empty()) {
                auth.username = url_.user();
                auth.password = url_.password();
            }
            auto encoded_auth = StringTransformers::Base64Encoding(
                auth.username + ":" + auth.password
            );

            config_.headers.Add("Authorization", "Basic " + encoded_auth);
        }

        if (!config_.data.empty()) {
            if (!IsDataAllowed()) {
                Error::Logic(
                    "Request error",
                    "Data can only be added for "
                    "PUT, POST, DELETE, and PATCH requests"
                );
            }
            config_.headers.Add(
                "Content-Length",
                std::to_string(config_.data.size())
            );
        }

        config_.headers.Add("Connection", "close");
    }

    auto RequestBuilder::WriteToStream() -> void {
        stream_ << config_.method << " /" << url_.path() <<  " HTTP/1.1" << crlf;
        for (const auto& [_, header] : config_.headers) {
            stream_ << header.first + ": " + header.second << crlf;
        }
        stream_ << crlf;
        stream_ << config_.data;
    }

    auto RequestBuilder::IsDataAllowed() const -> bool {
        using enum Express::Method;
        switch (config_.method) {
            case Get:
            case Options:
            case Head:
                return false;
            default:
                return true;
        }
    }
}