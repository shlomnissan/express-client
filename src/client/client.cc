// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include "client.h"

#include <array>
#include <string>

#include "client/timeout.h"
#include "http/request_builder.h"
#include "http/response_parser.h"
#include "net/socket.h"
#include "net/url.h"

#if defined(_WIN32)
    #include "net/winsock.h"
#endif

namespace Express {
    auto Client::Request(const Config& config) const -> std::future<Response> {
        return std::async(std::launch::async, [config](){
            #if defined(_WIN32)
                Net::WinSock winsock;
            #endif

            const Timeout timeout {config.timeout};
            const Net::Url url {config.url};
            const Net::Socket socket {{url.host(), url.port()}};
            const Http::RequestBuilder request(config);

            socket.Connect(timeout);
            socket.Send(request.GetData(), timeout);

            std::array<unsigned char, BUFSIZ> buffer;
            Http::ResponseParser parser;

            while (true) {
                auto size = socket.Recv(
                    buffer.data(),
                    buffer.size(),
                    timeout
                );
                if (size == 0 || parser.done_reading_data()) {
                    break;
                }
                parser.Feed(buffer.data(), size);
            }

            return parser.response();
        });
    }
}