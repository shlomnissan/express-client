// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <gtest/gtest.h>

#include <express_client/endpoint.h>
#include <sys/socket.h>

using namespace Express::Net;

TEST(endpoint_test, basic_test) {
    Endpoint endpoint {"example.com", "80"};

    EXPECT_EQ(endpoint.getFamily(), AF_INET);
    EXPECT_EQ(endpoint.getSocketType(), SOCK_STREAM);
    EXPECT_EQ(endpoint.getProtocol(), IPPROTO_TCP);
    EXPECT_EQ(endpoint.getAddressLength(), 16);
    EXPECT_TRUE(endpoint.getAddress() != nullptr);
}

TEST(endpoint_test, throws_initialization_error) {
    EXPECT_THROW({
        Endpoint endpoint("invalid-address", "80");
    }, InvalidAddress);
}
