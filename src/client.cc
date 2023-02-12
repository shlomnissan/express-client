// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <iostream>
#include <sstream>
#include <string>

#include <express/client.h>
#include <express/request.h>
#include <express/socket.h>
#include <express/url.h>

namespace Express {
    auto ExpressClient::request(const Http::RequestConfig& config) -> Http::Response {
        Net::URL url {config.url};

        if (url.scheme() != "http") {
            throw ExpressClientError {"Invalid URL scheme. Only http is supported."};
        }

        Net::Socket socket {{url.host(), url.port()}};
        socket.connect();

        // TODO: Remove writeRequest. Use constructor and an accessor instead
        std::stringstream request_buffer;
        Http::Request request {url, config};
        request.writeRequest(request_buffer);

        socket.send(request_buffer.str());

        uint8_t temp_buffer[BUFSIZ];
        Http::ResponseParser response_parser;
        while (true) {
            auto size = socket.recv(temp_buffer);
            if (size == 0) {
                break; // disconnected
            }
            response_parser.feed(temp_buffer, size);
            // TODO: we should also break if we're done processing the data
        }

        return response_parser.response();
    }
}
