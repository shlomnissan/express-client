// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <express/request.h>

namespace Express::Http {
    Request::Request(const Net::URL& url, const RequestConfig& config) :
        config_(config),
        url_(url) {
        config_.headers.add({"Host", url_.host()});
        config_.headers.add({"User-Agent", "express/0.1"});
        if (config_.data.size()) {
            if (!allowedData(config.method)) {
                throw RequestError {
                    "Request data can only be added "
                    "for PUT, POST, DELETE, and PATCH."
                };
            }
            config_.headers.add({"Content-Type", config_.data.contentType()});
            config_.headers.add({"Content-Length", std::to_string(config_.data.size())});
        }
        config_.headers.add({"Connection", "close"});
    }

    auto Request::writeRequest(std::stringstream& buffer) const -> void {
        buffer << config_.method << " " << "/ HTTP/1.1" << crlf;
        for (const auto& header : config_.headers) {
            buffer << header;
        }
        buffer << crlf;
        buffer << config_.data.data();
    }

    auto Request::allowedData(const Method method) const -> bool {
        using enum Method;
        if (method == Get || method == Options || method == Head) {
            return false;
        }
        return true;
    }
}