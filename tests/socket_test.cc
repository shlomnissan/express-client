// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <gtest/gtest.h>

#include <express/socket.h>
#include <express/endpoint.h>

using namespace Express::Net;

TEST(socket, basic_test) {
    Endpoint endpoint {"example.com", "80"};
    Socket socket {std::move(endpoint)};

    EXPECT_TRUE(socket.get() > 0);
}

// create stubs for C library functions
// TODO: failed to connect
// TODO: failed to initialize
// TODO: timeout