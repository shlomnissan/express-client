// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <express/client.h>

#include <array>
#include <cstdint>
#include <memory>
#include <express/endpoint.h>
#include <express/error.h>
#include <express/socket_secure.h>
#include <express/timeout.h>

namespace Express {
    auto Client::request(const RequestConfig& config) -> std::future<Response> {
        return std::async(std::launch::async, makeRequest, config);
    }

    auto Client::makeRequest(const RequestConfig& config) -> Response {
        URL url {config.url};
        #ifndef BUILD_SSL
            if (url.scheme() == "https") {
                Error::logic(
                    "Client error",
                    "You must build the library with BUILD_SSL=ON to use https"
                );
            }
        #endif

        Request request {url, config};
        Timeout timeout {request.timeout()};

        auto socket = getSocket(url);
        socket->connect(timeout);
        socket->send(request.str(), timeout);

        std::array<uint8_t, BUFSIZ> buffer;
        ResponseParser parser;
        while (true) {
            auto size = socket->recv(buffer.data(), buffer.size(), timeout);
            if (size == 0 || parser.doneReadingData()) {
                break; // disconnected
            }
            parser.feed(buffer.data(), size);
        }

        return parser.response();
    }

    auto Client::getSocket(const URL& url) -> std::unique_ptr<Socket> {
        Endpoint endpoint {url.host(), url.port()};
        #ifdef BUILD_SSL
        if (url.scheme() == "https") {
            return std::make_unique<SocketSecure>(std::move(endpoint));
        }
        #endif
        return std::make_unique<Socket>(std::move(endpoint));
    }
}
