// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <gtest/gtest.h>

#include <express/endpoint.h>
#include <sys/socket.h>

using namespace Express::Net;

TEST(endpoint, initializes_basic_endpoint) {
    Endpoint endpoint {"example.com", "80"};

    EXPECT_EQ(endpoint.family(), AF_INET);
    EXPECT_EQ(endpoint.socketType(), SOCK_STREAM);
    EXPECT_EQ(endpoint.protocol(), IPPROTO_TCP);
    EXPECT_EQ(endpoint.addressLength(), 16);
    EXPECT_TRUE(endpoint.address() != nullptr);
}

TEST(endpoint, initializes_ipv4_endpoint) {
    Endpoint endpoint {"93.184.216.34", "80"};

    EXPECT_EQ(endpoint.family(), AF_INET);
    EXPECT_EQ(endpoint.addressLength(), 16);
    EXPECT_TRUE(endpoint.address() != nullptr);
}

TEST(endpoint, initializes_ipv6_endpoint) {
    Endpoint endpoint {"2606:2800:220:1:248:1893:25c8:1946", "80"};

    EXPECT_EQ(endpoint.family(), AF_INET6);
    EXPECT_EQ(endpoint.addressLength(), 28);
    EXPECT_TRUE(endpoint.address() != nullptr);
}

TEST(endpoint, throws_initialization_error) {
    EXPECT_THROW({
        try {
            Endpoint endpoint("invalid-address", "80");
        } catch (const AddressError& e) {
            EXPECT_STREQ(e.what(),
                "Failed to initialize an endpoint. Check your hostname "
                "and ensure the port you're requesting is free."
            );
            throw;
        }
    }, AddressError);
}