// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include "net/endpoint.h"

#include <string_view>

#include <arpa/inet.h>
#include <gtest/gtest.h>
#include <netinet/in.h>

TEST(Endpoint, InitializesWithDomain) {
    Express::Net::Endpoint endpoint {"example.com", "80"};

    EXPECT_EQ(endpoint.family(), AF_INET);
    EXPECT_EQ(endpoint.socket_type(), SOCK_STREAM);
    EXPECT_EQ(endpoint.address_length(), INET_ADDRSTRLEN);
    EXPECT_TRUE(endpoint.address() != nullptr);
}

TEST(Endpoint, InitializesWithIPv4) {
    Express::Net::Endpoint endpoint {"93.184.216.34", "80"};

    EXPECT_EQ(endpoint.family(), AF_INET);
    EXPECT_EQ(endpoint.socket_type(), SOCK_STREAM);
    EXPECT_EQ(endpoint.address_length(), INET_ADDRSTRLEN);
    EXPECT_TRUE(endpoint.address() != nullptr);

    char buf[INET_ADDRSTRLEN];
    auto addr = reinterpret_cast<sockaddr_in*>(endpoint.address());
    inet_ntop(endpoint.family(), &addr->sin_addr, buf, INET_ADDRSTRLEN);
    EXPECT_STREQ(buf, "93.184.216.34");
}

TEST(Endpoint, InitializesWithIPv6) {
    Express::Net::Endpoint endpoint {"2606:2800:220:1:248:1893:25c8:1946", "80"};

    EXPECT_EQ(endpoint.family(), AF_INET6);
    EXPECT_EQ(endpoint.socket_type(), SOCK_STREAM);
    EXPECT_EQ(endpoint.address_length(), 28);
    EXPECT_TRUE(endpoint.address() != nullptr);

    char buf[INET6_ADDRSTRLEN];
    auto addr = reinterpret_cast<sockaddr_in6*>(endpoint.address());
    inet_ntop(endpoint.family(), &addr->sin6_addr, buf, INET6_ADDRSTRLEN);
    EXPECT_STREQ(buf, "2606:2800:220:1:248:1893:25c8:1946");
}

TEST(Endpoint, ThrowsErrorIfInitializationFails) {
    EXPECT_THROW({
        try {
            Express::Net::Endpoint endpoint("invalid-address", "80");
        } catch (const std::system_error& e) {
            std::string_view error_str = e.what();
            EXPECT_TRUE(error_str.starts_with("Endpoint error"));
            throw;
        }
    }, std::system_error);
}