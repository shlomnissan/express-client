// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <gtest/gtest.h>

#include <express/socket_secure.h>
#include <express/endpoint.h>

#if defined(_WIN32)
#include <express/winsock.h>
#endif

using namespace Express::Net;

class SocketSecureTest : public ::testing::Test {
#if defined(_WIN32)
    WinSock winsock;
#endif
};

TEST_F(SocketSecureTest, basic_test) {
    SocketSecure socket_sec {{"example.com", "443"}};    
    socket_sec.connect();
    EXPECT_TRUE(socket_sec.get() > 0);
}