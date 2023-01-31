// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <express_client/request.h>

#include <iostream>

namespace Express::Http {
    Request::Request(Method method, const std::string& resource) :
        method_(method),
        resource_(resource) {
    }

    auto Request::writeRequest(std::stringstream& buffer) const -> void {
        buffer << method_ << " ";
        buffer << "/";
        buffer << " HTTP/1.1" << crlf;
        buffer << "Host: " << resource_ << ":80" << crlf;
        buffer << "Connection: close" << crlf;
        buffer << "User-Agent: express/0.1" << crlf;
        buffer << crlf;
    }
}