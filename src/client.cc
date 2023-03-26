// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <express/client.h>

#include <express/endpoint.h>
#include <express/socket.h>

#if defined(_WIN32)
    #include <express/winsock.h>
#endif

namespace Express {
    auto ExpressClient::request(const RequestConfig& config) -> std::future<Response> {
        return std::async(std::launch::async, makeRequest, config);
    }

    auto ExpressClient::makeRequest(const RequestConfig& config) -> Response {
        #if defined(_WIN32)
            WinSock winsock;
        #endif

        URL url {config.url};
        Request request {url, config};
        Endpoint endpoint {url.host(), url.port()};
        Socket socket {std::move(endpoint)};

        socket.connect();
        socket.send(request.str(), request.timeout());

        uint8_t temp_buffer[BUFSIZ];
        ResponseParser parser;
        while (true) {
            auto size = socket.recv(temp_buffer, request.timeout());
            if (size == 0 || parser.doneReadingData()) {
                break; // disconnected
            }
            parser.feed(temp_buffer, size);
        }

        return parser.response();
    }
}
