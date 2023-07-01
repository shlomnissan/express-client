// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <array>
#include <sstream>
#include <chrono>

#include <gtest/gtest.h>

#include <express/socket.h>
#include <express/endpoint.h>
#include <express/timeout.h>

using namespace Express;
using namespace Express::Net;
using namespace std::chrono_literals;

TEST(socket, basic_test) {
    Socket socket {{"example.com", "80"}};
    socket.connect();
    EXPECT_TRUE(socket.get() > 0);
    
    std::stringstream ss;
    ss << "GET / HTTP/1.1\r\n";
    ss << "Host: example.com\r\n";
    ss << "Connection: close\r\n";
    ss << "User-Agent: express\r\n";
    ss << "\r\n";

    Express::Timeout timeout {0s};

    socket.send(ss.str(), timeout);

    std::array<uint8_t, BUFSIZ> buffer;
    socket.recv(buffer.data(), buffer.size(), timeout);
    std::string response {
        reinterpret_cast<const char*>(buffer.data())
    };

    EXPECT_TRUE(response.starts_with("HTTP/1.1 200 OK"));    
}