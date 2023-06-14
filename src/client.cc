// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <memory>

#include <express/client.h>
#include <express/endpoint.h>
#include <express/socket_secure.h>
#include <express/timeout.h>

#if defined(_WIN32)
    #include <express/winsock.h>
#endif

namespace Express {
    auto Client::request(const RequestConfig& config) -> std::future<Response> {
        return std::async(std::launch::async, makeRequest, config);
    }

    auto Client::makeRequest(const RequestConfig& config) -> Response {
        #if defined(_WIN32)
            WinSock winsock;
        #endif

        URL url {config.url};
        #ifndef BUILD_SSL
            if (url.scheme() == "https") {
                throw ClientError {
                    "You must build the library with BUILD_SSL=ON to use https."
                };
            }
        #endif

        auto socket = getSocket(url); 

        Request request {url, config};
        Timeout timeout {request.timeout()};

        socket->connect();
        socket->sendAll(request.str(), timeout);

        uint8_t temp_buffer[BUFSIZ];
        ResponseParser parser;
        while (true) {
            auto size = socket->recv(temp_buffer, timeout);
            if (size == 0 || parser.doneReadingData()) {
                break; // disconnected
            }
            parser.feed(temp_buffer, size);
        }

        return parser.response();
    }

    auto Client::getSocket(const URL& url) -> std::unique_ptr<Socket> {
        Endpoint endpoint {url.host(), url.port()};
        if (url.scheme() == "https") {
            return std::make_unique<SocketSecure>(std::move(endpoint));
        }
        return std::make_unique<Socket>(std::move(endpoint));
    }
}
