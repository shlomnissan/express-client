// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <express_client/client.h>
#include <express_client/socket.h>

#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>

namespace Express {
    using namespace Net;

    auto Client::get() const -> int {
        Socket socket {{"example.com", "80"}};
        socket.connect();

        char buffer[2048];
        sprintf(buffer, "GET / HTTP/1.1\r\n");
        sprintf(buffer + strlen(buffer), "Host: %s:%s\r\n", "example.com", "80");
        sprintf(buffer + strlen(buffer), "Connection: close\r\n");
        sprintf(buffer + strlen(buffer), "User-Agent: Express-client 1.0\r\n");
        sprintf(buffer + strlen(buffer), "\r\n");

        socket.send(buffer);

        std::string response;
        long bytes = 0;
        while ((bytes = socket.recv(buffer)) > 0) {
            buffer[bytes] = '\0';
            response += buffer;
        }
        std::cout << response << '\n';

        return 12;
    }
}
