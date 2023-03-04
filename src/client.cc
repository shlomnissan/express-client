// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <string>

#include <express/client.h>
#include <express/request.h>
#include <express/endpoint.h>
#include <express/socket.h>
#include <express/url.h>

namespace Express {
    auto ExpressClient::request(const Http::RequestConfig& config) -> Http::Response {
        Net::URL url {config.url};
        Http::Request request {url, config};
        
        Net::Endpoint endpoint {url.host(), url.port()};
        Net::Socket socket {std::move(endpoint)};

        socket.connect();
        socket.send(request.str());

        uint8_t temp_buffer[BUFSIZ];
        Http::ResponseParser parser;
        while (true) {
            auto size = socket.recv(temp_buffer);
            if (size == 0 || parser.doneReadingData()) {
                break; // disconnected
            }
            parser.feed(temp_buffer, size);
        }

        return parser.response();
    }
}
