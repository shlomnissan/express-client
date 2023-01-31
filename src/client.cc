// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <express_client/client.h>
#include <express_client/socket.h>
#include <express_client/request.h>

#include <ios>
#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <string.h>

namespace Express {
    auto Client::get() const -> int {
        Net::Socket socket {{"example.com", "80"}};
        socket.connect();

        Http::Request request(Http::Method::Get, "example.com");
        std::stringstream request_buffer;
        request.writeRequest(request_buffer);

        socket.send(request_buffer.str());

        std::stringstream response_buffer;
        char buffer[BUFSIZ];
        std::streamsize bytes = 0;
        while ((bytes = socket.recv(buffer)) > 0) {
            response_buffer.write(buffer, bytes);
            response_buffer << Http::crlf;
        }

        auto output = response_buffer.str();
        std::cout << output;

        return 12;
    }
}
