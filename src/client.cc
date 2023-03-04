// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <express/client.h>

#include <express/endpoint.h>
#include <express/socket.h>

namespace Express {
    auto ExpressClient::request(const RequestConfig& config) -> Response {
        URL url {config.url};
        Request request {url, config};

        return makeRequest(url, request);
    }

    auto ExpressClient::makeRequest(const URL& url, const Request& request) -> Response {
        Endpoint endpoint {url.host(), url.port()};
        Socket socket {std::move(endpoint)};

        socket.connect();
        socket.send(request.str());

        uint8_t temp_buffer[BUFSIZ];
        ResponseParser parser;
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
