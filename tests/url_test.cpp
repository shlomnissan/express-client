// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <gtest/gtest.h>

#include <express/url.h>

using namespace Express::Net;

TEST(url_test, basic_test) {
    URL url("http://example.com:80/user?q=foo#bar"); 

    EXPECT_EQ(url.scheme(), "http");
    // EXPECT_EQ(url.host(), "example.com");
    // EXPECT_EQ(url.port(), "80");
    // EXPECT_EQ(url.path(), "user");
    // EXPECT_EQ(url.query(), "q=foo");
    // EXPECT_EQ(url.fragment(), "bar");
}

// TODO: empty url
// TODO: invalid url scheme