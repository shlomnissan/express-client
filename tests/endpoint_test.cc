// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <gtest/gtest.h>

#include <express/endpoint.h>

#if defined(_WIN32)
#include <express/winsock.h>
#else
#include <sys/socket.h>
#endif

using namespace Express::Net;

class EndpointTest : public ::testing::Test {
protected:
#if defined(_WIN32)
    WinSock winsock;
#endif
};

TEST_F(EndpointTest, initializes_basic_endpoint) {
    Endpoint endpoint {"example.com", "80"};

    EXPECT_EQ(endpoint.family(), AF_INET);
    EXPECT_EQ(endpoint.socketType(), SOCK_STREAM);
    EXPECT_EQ(endpoint.addressLength(), 16);
    EXPECT_TRUE(endpoint.address() != nullptr);
}

TEST_F(EndpointTest, initializes_ipv4_endpoint) {
    Endpoint endpoint {"93.184.216.34", "80"};

    EXPECT_EQ(endpoint.family(), AF_INET);
    EXPECT_EQ(endpoint.addressLength(), 16);
    EXPECT_TRUE(endpoint.address() != nullptr);
}

TEST_F(EndpointTest, initializes_ipv6_endpoint) {
    Endpoint endpoint {"2606:2800:220:1:248:1893:25c8:1946", "80"};

    EXPECT_EQ(endpoint.family(), AF_INET6);
    EXPECT_EQ(endpoint.addressLength(), 28);
    EXPECT_TRUE(endpoint.address() != nullptr);
}

TEST_F(EndpointTest, throws_initialization_error) {
    EXPECT_THROW({
        try {
            Endpoint endpoint("invalid-address", "80");
        } catch (const AddressError& e) {
            EXPECT_STREQ(e.what(), "Failed to initialize an endpoint.");
            throw;
        }
    }, AddressError);
}