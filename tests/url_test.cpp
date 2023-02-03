// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <gtest/gtest.h>

#include <express/url.h>

using namespace Express::Net;

TEST(url_test, basic_test) {
    URL url("http://user:pass@example.com:80/user?q=foo#bar"); 

    EXPECT_EQ(url.scheme(), "http");
    EXPECT_EQ(url.authority(), "user:pass@example.com:80");
    EXPECT_EQ(url.user_information().username(), "user");
    EXPECT_EQ(url.user_information().password(), "pass");
    EXPECT_EQ(url.host(), "example.com");
    EXPECT_EQ(url.port(), "80");
    EXPECT_EQ(url.path(), "user");
    EXPECT_EQ(url.query(), "q=foo");
    EXPECT_EQ(url.fragment(), "bar");
}

// TODO: simple URL
// TODO: simple URL with path
// TODO: no query
// TODO: no fragment
// TODO: no scheme 
// TODO: invalid scheme
// TODO: only user name
// TODO: no username or password but @
// TODO: url with no port defaults to 80
// TODO: IPv4
// TODO: IPv6
// TODO: invalid URL
// TODO: empty URL