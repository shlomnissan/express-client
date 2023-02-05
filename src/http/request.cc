// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <express/request.h>

#include <iostream>

namespace Express::Http {
    Request::Request(const RequestConfig& config, const URL& url) :
        config_(config),
        url_(url) {
           // TODO: throws exception if scheme is not supported
           config_.headers.add({"Host", url_.host() + ":" + url_.port()});
           config_.headers.add({"User-Agent", "express/0.1"});
    }

    auto Request::writeRequest(std::stringstream& buffer) const -> void {
        buffer << config_.method << " ";
        buffer << "/";
        buffer << " HTTP/1.1" << crlf;
        buffer << "Host: " << url_.host() << ":" << url_.port() << crlf;
        buffer << "Connection: close" << crlf;
        buffer << "User-Agent: express/0.1" << crlf;
        buffer << crlf;
    }
}