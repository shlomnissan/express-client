// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <express/request.h>

#include <string>

#include <express/error.h>
#include <express/transformers.h>
#include <express/version.h>

namespace Express::Http {
    using namespace Net;

    Request::Request(const Net::URL& url, const RequestConfig& config)
    : config_(config), url_(url) {
        setHeaders();
        writeRequest();
    }

    auto Request::setHeaders() -> void {
        auto version {
            std::to_string(Version::Major) + "." +
            std::to_string(Version::Minor)
        };

        config_.headers.add({"Host", url_.host()});
        config_.headers.add({"User-Agent", "express/" + version});

        if (!config_.auth.empty()) {
            setBasicAuth(config_.auth);
        } else if (!url_.userInformation().empty()) {
            setBasicAuth(url_.userInformation());
        }

        if (config_.data.size()) {
            setContentTypeAndSize();
        }

        config_.headers.add({"Connection", "close"});
    }

    auto Request::setBasicAuth(const UserInformation& info) -> void {
        if (config_.headers.has("authorization")) {
            config_.headers.remove("authorization");
        }
        const auto auth = Transformers::base64_encode(
            info.username + ":" + info.password
        );
        config_.headers.add({"Authorization", "Basic " + auth});
    }

    auto Request::setContentTypeAndSize() -> void {
        if (!allowedData(config_.method)) {
            Error::logic("Request error", "Request data can only be added "
                "for PUT, POST, DELETE, and PATCH.");
        }

        if (!config_.headers.has("content-type")) {
            const auto content_type_hint = config_.data.contentTypeHint();
            if (!content_type_hint.empty()) {
                config_.headers.add({"Content-Type", content_type_hint});
            } else {
                Error::logic("Request error", "Data was provided for this request, "
                    "but the Content-Type header isn't set.");
            }
        }

        config_.headers.add({
            "Content-Length",
            std::to_string(config_.data.size())
        });
    }

    auto Request::writeRequest() -> void {
        buffer_ << config_.method << " /" << url_.path() << " HTTP/1.1" << crlf;
        for (const auto& header : config_.headers) {
            buffer_ << header;
        }
        buffer_ << crlf;
        buffer_ << config_.data.data();
    }

    auto Request::allowedData(const Method method) const -> bool {
        using enum Method;
        if (method == Get || method == Options || method == Head) {
            return false;
        }
        return true;
    }
}