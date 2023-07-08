// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <gtest/gtest.h>

#include <express/endpoint.h>

#include <string_view>
#include <sys/socket.h>
#include <system_error>

using namespace Express::Net;

bool startsWith(std::string_view str, std::string_view ptrn) {
    return str.starts_with(ptrn);
}

TEST(endpoint, initializes_basic_endpoint) {
    Endpoint endpoint {"example.com", "80"};

    EXPECT_EQ(endpoint.family(), AF_INET);
    EXPECT_EQ(endpoint.socketType(), SOCK_STREAM);
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
        } catch (const std::system_error& e) {
            EXPECT_TRUE(startsWith(e.what(), "Endpoint error"));
            throw;
        }
    }, std::system_error);
}