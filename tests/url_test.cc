// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <gtest/gtest.h>

#include <express/url.h>

using namespace Express::Net;

TEST(url, parses_complete_url) {
    URL url("http://user:pass@example.com:3000/user?q=foo#bar"); 

    EXPECT_EQ(url.scheme(), "http");
    EXPECT_EQ(url.authority(), "user:pass@example.com:3000");
    EXPECT_EQ(url.host(), "example.com");
    EXPECT_EQ(url.port(), "3000");
    EXPECT_EQ(url.path(), "user");
    EXPECT_EQ(url.query(), "q=foo");
}

TEST(url, parses_simple_url) {
    URL url("http://example.com");

    EXPECT_EQ(url.scheme(), "http");
    EXPECT_EQ(url.authority(), "example.com");
    EXPECT_EQ(url.host(), "example.com");
    EXPECT_EQ(url.port(), "80");
    EXPECT_EQ(url.path(), "");
    EXPECT_EQ(url.query(), "");
}

TEST(url, parses_simple_url_with_path) {
    URL url("http://example.com/user");

    EXPECT_EQ(url.scheme(), "http");
    EXPECT_EQ(url.authority(), "example.com");
    EXPECT_EQ(url.host(), "example.com");
    EXPECT_EQ(url.port(), "80");
    EXPECT_EQ(url.path(), "user");
    EXPECT_EQ(url.query(), "");
}

TEST(url, parses_url_with_multiple_path_segments) {
    URL url("http://www.example.com/user/david/image.png");

    EXPECT_EQ(url.scheme(), "http");
    EXPECT_EQ(url.authority(), "www.example.com");
    EXPECT_EQ(url.host(), "www.example.com");
    EXPECT_EQ(url.port(), "80");
    EXPECT_EQ(url.path(), "user/david/image.png");
    EXPECT_EQ(url.query(), "");
}

TEST(url, parses_url_with_query_and_fragment_no_path) {
    URL url("http://example.com?q=foo#bar");

    EXPECT_EQ(url.scheme(), "http");
    EXPECT_EQ(url.authority(), "example.com");
    EXPECT_EQ(url.host(), "example.com");
    EXPECT_EQ(url.port(), "80");
    EXPECT_EQ(url.path(), "");
    EXPECT_EQ(url.query(), "q=foo");
}

TEST(url, parses_url_with_query_and_fragment_no_path_after_slash) {
    URL url("http://example.com/?q=foo#bar");

    EXPECT_EQ(url.scheme(), "http");
    EXPECT_EQ(url.authority(), "example.com");
    EXPECT_EQ(url.host(), "example.com");
    EXPECT_EQ(url.port(), "80");
    EXPECT_EQ(url.path(), "");
    EXPECT_EQ(url.query(), "q=foo");
}

TEST(url, parses_url_with_query_no_path) {
    URL url("http://example.com?q=foo");

    EXPECT_EQ(url.scheme(), "http");
    EXPECT_EQ(url.authority(), "example.com");
    EXPECT_EQ(url.host(), "example.com");
    EXPECT_EQ(url.port(), "80");
    EXPECT_EQ(url.path(), "");
    EXPECT_EQ(url.query(), "q=foo");
}

TEST(url, parses_url_with_query_and_path) {
    URL url("http://example.com/user?q=foo");

    EXPECT_EQ(url.scheme(), "http");
    EXPECT_EQ(url.authority(), "example.com");
    EXPECT_EQ(url.host(), "example.com");
    EXPECT_EQ(url.port(), "80");
    EXPECT_EQ(url.path(), "user");
    EXPECT_EQ(url.query(), "q=foo");
}

TEST(url, parses_url_with_fragment_no_path) {
    URL url("http://example.com#bar");

    EXPECT_EQ(url.scheme(), "http");
    EXPECT_EQ(url.authority(), "example.com");
    EXPECT_EQ(url.host(), "example.com");
    EXPECT_EQ(url.port(), "80");
    EXPECT_EQ(url.path(), "");
    EXPECT_EQ(url.query(), "");
}

TEST(url, parses_url_with_fragment_and_path) {
    URL url("http://example.com/user#bar");

    EXPECT_EQ(url.scheme(), "http");
    EXPECT_EQ(url.authority(), "example.com");
    EXPECT_EQ(url.host(), "example.com");
    EXPECT_EQ(url.port(), "80");
    EXPECT_EQ(url.path(), "user");
    EXPECT_EQ(url.query(), "");
}

TEST(url, parses_url_with_empty_user_info) {
    URL url("http://@example.com");

    EXPECT_EQ(url.scheme(), "http");
    EXPECT_EQ(url.authority(), "@example.com");
    EXPECT_EQ(url.userInformation().username, "");
    EXPECT_EQ(url.userInformation().password, "");
    EXPECT_EQ(url.host(), "example.com");
    EXPECT_EQ(url.port(), "80");
    EXPECT_EQ(url.path(), "");
    EXPECT_EQ(url.query(), "");
}

TEST(url, parses_url_with_username_and_no_password) {
    URL url("http://user@example.com");

    EXPECT_EQ(url.scheme(), "http");
    EXPECT_EQ(url.authority(), "user@example.com");
    EXPECT_EQ(url.userInformation().username, "user");
    EXPECT_EQ(url.userInformation().password, "");
    EXPECT_EQ(url.host(), "example.com");
    EXPECT_EQ(url.port(), "80");
    EXPECT_EQ(url.path(), "");
    EXPECT_EQ(url.query(), "");
}

TEST(url, parses_url_with_password_and_no_username) {
    URL url("http://:pass@example.com");

    EXPECT_EQ(url.scheme(), "http");
    EXPECT_EQ(url.authority(), ":pass@example.com");
    EXPECT_EQ(url.userInformation().username, "");
    EXPECT_EQ(url.userInformation().password, "pass");
    EXPECT_EQ(url.host(), "example.com");
    EXPECT_EQ(url.port(), "80");
    EXPECT_EQ(url.path(), "");
    EXPECT_EQ(url.query(), "");
}

TEST(url, throws_if_the_url_is_empty) {
    EXPECT_THROW({
        try {
            URL url("");
        } catch (const URLError& e) {
            EXPECT_STREQ(
                "Unsupported URL scheme. "
                "The client currently supports only HTTP."
                , e.what()
            );
            throw;
        }
    }, URLError);
}

TEST(url, throws_if_theres_no_scheme) {
    EXPECT_THROW({
        try {
            URL url("example.com");
        } catch (const URLError& e) {
            EXPECT_STREQ(
                "Unsupported URL scheme. "
                "The client currently supports only HTTP."
                , e.what()
            );
            throw;
        }
    }, URLError);
}

TEST(url, throws_if_theres_an_invalid_scheme_delimiter) {
    EXPECT_THROW({
        try {
            URL url("http:/example.com");
        } catch (const URLError& e) {
            EXPECT_STREQ(
                "Unsupported URL scheme. "
                "The client currently supports only HTTP."
                , e.what()
            );
            throw;
        }
    }, URLError);
}

TEST(url, throws_if_url_scheme_is_unsupported) {
    EXPECT_THROW({
        try {
            URL url("https://example.com");
        } catch (const URLError& e) {
            EXPECT_STREQ(
                "Unsupported URL scheme. "
                "The client currently supports only HTTP."
                , e.what()
            );
            throw;
        }
    }, URLError);
}