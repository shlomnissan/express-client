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

TEST_F(SocketSecureTest, throws_error_bad_ssl_certificate) {
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