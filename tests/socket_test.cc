// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <gtest/gtest.h>

#include <express/socket.h>
#include <express/endpoint.h>

#if defined(_WIN32)
#include <express/winsock.h>
#endif

using namespace Express::Net;

class SocketTest : public ::testing::Test {
#if defined(_WIN32)
    WinSock winsock;
#endif
};

TEST_F(SocketTest, basic_test) {
    Socket socket {{"example.com", "80"}};

    EXPECT_TRUE(socket.get() > 0);
}

// TODO: provides complete example

// TODO: failed to initialize
// TODO: failed to connect
// TODO: failed to send
// TODO: failed to receive
// TODO: failed to select
// TODO: timeout