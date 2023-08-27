// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include "net/url.h"

#include <gtest/gtest.h>

#include "exception.h"

TEST(Url, ParsesCompleteUrl) {
    Express::Net::Url url("http://user:pass@example.com:3000/user?q=foo#bar");

    EXPECT_EQ(url.host(), "example.com");
    EXPECT_EQ(url.password(), "pass");
    EXPECT_EQ(url.path(), "user");
    EXPECT_EQ(url.port(), "3000");
    EXPECT_EQ(url.query(), "q=foo");
    EXPECT_EQ(url.scheme(), "http");
    EXPECT_EQ(url.user(), "user");
}

TEST(Url, ParsesWithHttpScheme) {
     Express::Net::Url url("http://example.com");

    EXPECT_EQ(url.port(), "80");
    EXPECT_EQ(url.scheme(), "http");
}

TEST(Url, ParsesWithHttpsScheme) {
     Express::Net::Url url("https://example.com");

    EXPECT_EQ(url.port(), "443");
    EXPECT_EQ(url.scheme(), "https");
}

TEST(Url, ParsesWithPath) {
    Express::Net::Url url("http://example.com/user");

    EXPECT_EQ(url.host(), "example.com");
    EXPECT_EQ(url.path(), "user");
}

TEST(Url, ParsesWithMultiplePathSegments) {
    Express::Net::Url url("http://www.example.com/user/david/image.png");

    EXPECT_EQ(url.host(), "www.example.com");
    EXPECT_EQ(url.path(), "user/david/image.png");
}

TEST(Url, ParsesWithQuery) {
    Express::Net::Url url("http://example.com?q=foo");

    EXPECT_EQ(url.host(), "example.com");
    EXPECT_EQ(url.query(), "q=foo");
}

TEST(Url, ParsesWithQueryAndPath) {
    Express::Net::Url url("http://example.com/user?q=foo");

    EXPECT_EQ(url.host(), "example.com");
    EXPECT_EQ(url.path(), "user");
    EXPECT_EQ(url.query(), "q=foo");
}

TEST(Url, ParsesWithQueryAndFragment) {
    Express::Net::Url url("http://example.com?q=foo#bar");

    EXPECT_EQ(url.host(), "example.com");
    EXPECT_EQ(url.query(), "q=foo");
}

TEST(Url, ParsesWithQueryAndFragmentAfterSlash) {
    Express::Net::Url url("http://example.com/?q=foo#bar");

    EXPECT_EQ(url.host(), "example.com");
    EXPECT_EQ(url.query(), "q=foo");
}

TEST(Url, ParsesWithFragmentAndPath) {
    Express::Net::Url url("http://example.com/?q=foo#bar");

    EXPECT_EQ(url.host(), "example.com");
}

TEST(Url, ParsesWithUserInfo) {
    Express::Net::Url url("http://username:password@example.com");

    EXPECT_EQ(url.host(), "example.com");
    EXPECT_EQ(url.user(), "username");
    EXPECT_EQ(url.password(), "password");
    EXPECT_TRUE(url.HasUserInformation());
}

TEST(Url, ParsesWithEmptyUserInfo) {
    Express::Net::Url url("http://@example.com");

    EXPECT_EQ(url.host(), "example.com");
    EXPECT_EQ(url.user(), "");
    EXPECT_EQ(url.password(), "");
    EXPECT_FALSE(url.HasUserInformation());
}

TEST(Url, ParsesWithUserOnly) {
    Express::Net::Url url("http://username@example.com");

    EXPECT_EQ(url.host(), "example.com");
    EXPECT_EQ(url.user(), "username");
    EXPECT_EQ(url.password(), "");
    EXPECT_TRUE(url.HasUserInformation());
}

TEST(Url, ParsesWithPasswordOnly) {
    Express::Net::Url url("http://:password@example.com");

    EXPECT_EQ(url.host(), "example.com");
    EXPECT_EQ(url.user(), "");
    EXPECT_EQ(url.password(), "password");
    EXPECT_TRUE(url.HasUserInformation());
}

TEST(Url, ThrowsErrorIfEmpty) {
    EXPECT_THROW({
        try {
            Express::Net::Url url("");
        } catch(Express::RequestError& e) {
            EXPECT_STREQ(
                "URL error: Missing URL scheme. Use 'http://' or 'https://'",
                e.what()
            );
            throw;
        }
    }, Express::RequestError);
}

TEST(Url, ThrowsErrorIfMissingScheme) {
    EXPECT_THROW({
        try {
            Express::Net::Url url("example.com");
        } catch(Express::RequestError& e) {
            EXPECT_STREQ(
                "URL error: Missing URL scheme. Use 'http://' or 'https://'",
                e.what()
            );
            throw;
        }
    }, Express::RequestError);
}

TEST(Url, ThrowsErrorIfInvalidSchemeDelimiter) {
    EXPECT_THROW({
        try {
            Express::Net::Url url("http:/example.com");
        } catch(Express::RequestError& e) {
            EXPECT_STREQ(
                "URL error: Missing URL scheme. Use 'http://' or 'https://'",
                e.what()
            );
            throw;
        }
    }, Express::RequestError);
}

TEST(Url, ThrowsErrorIfUnsupportedScheme) {
    EXPECT_THROW({
        try {
            Express::Net::Url url("ftp://example.com");
        } catch(Express::RequestError& e) {
            EXPECT_STREQ(
                "URL error: Unsupported URL scheme. Use 'http://' or 'https://'",
                e.what()
            );
            throw;
        }
    }, Express::RequestError);
}