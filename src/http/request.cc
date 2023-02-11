// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <express/request.h>

namespace Express::Http {
    Request::Request(const Net::URL& url, const RequestConfig& config) :
        config_(config),
        url_(url) {
        // TODO: Move this check to the request (socket should not connect)
        if (url.scheme() != "http") {
            throw RequestError {"Invalid URL scheme. Only http is supported."};
        }
        config_.headers.add({"Host", url_.host()});
        config_.headers.add({"User-Agent", "express/0.1"});
        if (config_.body.size()) {
            // TODO: Must be a valid body request or ignore?
            config_.headers.add({"Content-Type", config_.body.contentType()});
            config_.headers.add({"Content-Length", std::to_string(config_.body.size())});

            // The body in the configuration object is a reference to a temp,
            // so we need to store a copy of the data locally.
            data_ = config_.body.data();
        }
        config_.headers.add({"Connection", "close"});
    }

    auto Request::writeRequest(std::stringstream& buffer) const -> void {
        buffer << config_.method << " " << "/ HTTP/1.1" << crlf;
        for (const auto& header : config_.headers) {
            buffer << header;
        }
        buffer << crlf;
        buffer << data_;
    }
}