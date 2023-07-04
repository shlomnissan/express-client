// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <array>
#include <sstream>
#include <chrono>

#include <gtest/gtest.h>

#include <express/socket_secure.h>
#include <express/endpoint.h>

using namespace Express;
using namespace Express::Net;
using namespace std::chrono_literals;

TEST(secured_socket, basic_test) {
    Express::Timeout timeout {0s};

    SocketSecure socket_sec {{"example.com", "443"}};
    socket_sec.connect(timeout);
    EXPECT_TRUE(socket_sec.get() > 0);

    std::stringstream ss;
    ss << "GET / HTTP/1.1\r\n";
    ss << "Host: example.com\r\n";
    ss << "Connection: close\r\n";
    ss << "User-Agent: express\r\n";
    ss << "\r\n";

    socket_sec.send(ss.str(), timeout);

    std::array<uint8_t, BUFSIZ> buffer;
    socket_sec.recv(buffer.data(), buffer.size(), timeout);
    std::string response {
        reinterpret_cast<const char*>(buffer.data())
    };

    EXPECT_TRUE(response.starts_with("HTTP/1.1 200 OK"));    
}

TEST(secured_socket, throws_error_bad_ssl_certificate) {
    SocketSecure socket_sec {{"expired.badssl.com", "443"}};

    EXPECT_THROW({
        try {
            socket_sec.connect(Express::Timeout {1s});
        } catch (const SocketSecureError& e) {
            EXPECT_STREQ(
                "Failed to verify SSL certificate.",
                e.what()
            );
            throw e;
        }
    }, SocketSecureError);
}