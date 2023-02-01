// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <gtest/gtest.h>

#include <express/client.h>

TEST(client_test, get) {
    Express::Client client;
    EXPECT_EQ(client.get("http://example.com"), 12);
}