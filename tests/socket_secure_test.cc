// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <sstream>
#include <chrono>

#include <gtest/gtest.h>

#include <express/socket_secure.h>
#include <express/endpoint.h>

using namespace Express;
using namespace Express::Net;
using namespace std::chrono_literals;

TEST(secured_socket, basic_test) {
    SocketSecure socket_sec {{"example.com", "443"}};
    socket_sec.connect();
    EXPECT_TRUE(socket_sec.get() > 0);

    std::stringstream ss;
    ss << "GET / HTTP/1.1\r\n";
    ss << "Host: example.com\r\n";
    ss << "Connection: close\r\n";
    ss << "User-Agent: express\r\n";
    ss << "\r\n";

    Express::Timeout timeout {0s};

    socket_sec.send(ss.str(), timeout);

    uint8_t buffer[BUFSIZ];
    socket_sec.recv(buffer, timeout);
    std::string response {
        reinterpret_cast<const char*>(buffer)
    };

    EXPECT_TRUE(response.starts_with("HTTP/1.1 200 OK"));    
}

TEST(secured_socket, throws_error_bad_ssl_certificate) {
    SocketSecure socket_sec {{"expired.badssl.com", "443"}};

    EXPECT_THROW({
        try {
            socket_sec.connect();
        } catch (const SocketSecureError& e) {
            EXPECT_STREQ(
                "Failed to verify SSL certificate.",
                e.what()
            );
            throw e;
        }
    }, SocketSecureError);
}