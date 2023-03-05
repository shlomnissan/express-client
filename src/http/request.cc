// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <express/request.h>

namespace Express::Http {
    Request::Request(const Net::URL& url, const RequestConfig& config)
    : config_(config), url_(url) {
        setHeaders();
        writeRequest();
    }

    auto Request::setHeaders() -> void {
        config_.headers.add({"Host", url_.host()});
        config_.headers.add({"User-Agent", "express/0.1"});

        if (config_.data.size()) setBody();

        config_.headers.add({"Connection", "close"});
    }

    auto Request::setBody() -> void {
        if (!allowedData(config_.method)) {
            throw RequestError {
                "Request data can only be added "
                "for PUT, POST, DELETE, and PATCH."
            };
        }

        if (!config_.headers.has("content-type")) {
            auto content_type_hint = config_.data.contentTypeHint();
            if (!content_type_hint.empty()) {
                config_.headers.add({"Content-Type", content_type_hint});
            } else {
                throw RequestError {
                    "Data was provided for this request, "
                    "but the Content-Type header isn't set."
                };
            }
        }

        config_.headers.add({
            "Content-Length",
            std::to_string(config_.data.size())
        });
    }

    auto Request::writeRequest() -> void {
        buffer_ << config_.method << " " << "/ HTTP/1.1" << crlf;
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