// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <gtest/gtest.h>

#include <simple_http/client.h>

TEST(client_test, get) {
    simplehttp::Client client;
    EXPECT_EQ(client.get(), 12);
}