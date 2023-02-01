// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <express/client.h>
#include <express/socket.h>
#include <express/request.h>

#include <ios>
#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <string.h>

namespace Express {
    // TODO: replace vars with request config
    auto Client::request(Http::Method method, std::string_view url) const -> void {

        Net::Socket socket {{"example.com", "80"}};
        socket.connect();

        Http::Request request(method, "example.com");
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
    }

    // TODO: add request config
    auto Client::get(std::string_view url) const -> int {
        request(Http::Method::Get, url);
        return 12;
    }
}
