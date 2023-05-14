// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <sstream>
#include <chrono>

#include <gtest/gtest.h>

#include <express/socket.h>
#include <express/endpoint.h>
#include <express/timeout.h>

#if defined(_WIN32)
#include <express/winsock.h>
#endif

using namespace Express;
using namespace Express::Net;
using namespace std::chrono_literals;

class SocketTest : public ::testing::Test {
#if defined(_WIN32)
    WinSock winsock;
#endif
};

TEST_F(SocketTest, basic_test) {
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

    socket.sendAll(ss.str(), timeout);

    uint8_t buffer[BUFSIZ];
    socket.recv(buffer, timeout);
    std::string response {
        reinterpret_cast<const char*>(buffer)
    };

    EXPECT_TRUE(response.starts_with("HTTP/1.1 200 OK"));    
}

// TODO: failed to initialize
// TODO: failed to connect
// TODO: failed to send
// TODO: failed to receive
// TODO: failed to select
// TODO: timeout