// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <gtest/gtest.h>

#include <express/body.h>

using namespace Express::Http;

TEST(body_test, initializes_body_with_string) {
    Body body {"Country=Brasil&City=Belo Horizonte"};

    EXPECT_EQ(body.str(), "Country=Brasil&City=Belo Horizonte");
}