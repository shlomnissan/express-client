// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <express/request.h>

#include <iostream>

namespace Express::Http {
    Request::Request(const URL& url, const RequestConfig& config) :
        config_(config),
        url_(url) {
        if (url.scheme() != "http") {
            throw RequestError {"Invalid URL scheme. Only http is supported."};
        }

        config_.headers.add({"Host", url_.host()});
        config_.headers.add({"User-Agent", "express/0.1"});
        config_.headers.add({"Connection", "close"});
    }

    auto Request::writeRequest(std::stringstream& buffer) const -> void {
        buffer << config_.method << " " << "/ HTTP/1.1" << crlf;
        for (const auto& header : config_.headers) {
            buffer << header;
        } 
        buffer << crlf;
    }
}