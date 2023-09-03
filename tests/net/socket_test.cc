// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include "net/socket.h"

#include <array>
#include <chrono>
#include <sstream>

#include <gtest/gtest.h>

#include "client/timeout.h"

#if defined(_WIN32)
    #include "net/winsock.h"
#endif

class Socket: public ::testing::Test {
#if defined(_WIN32)
    Express::Net::WinSock winsock;
#endif
};

using namespace std::chrono_literals;

TEST_F(Socket, SendsAndReceivesData) {
    Express::Net::Socket socket {{"example.com", "80"}};

    Express::Timeout timeout {0s};
    socket.Connect(timeout);

    ASSERT_TRUE(socket.Get() > 0);

    std::stringstream ss;
    ss << "GET / HTTP/1.1\r\n";
    ss << "Host: example.com\r\n";
    ss << "Connection: close\r\n";
    ss << "User-Agent: express\r\n";
    ss << "\r\n";

    socket.Send(ss.str(), timeout);

    std::array<uint8_t, BUFSIZ> buffer;
    socket.Recv(buffer.data(), buffer.size(), timeout);
    std::string response {
        reinterpret_cast<const char*>(buffer.data())
    };

    EXPECT_TRUE(response.starts_with("HTTP/1.1 200 OK"));
}