// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <gtest/gtest.h>

#include <express/url.h>

using namespace Express::Net;

TEST(url_test, parses_complete_url) {
    URL url("https://user:pass@example.com:80/user?q=foo#bar"); 

    EXPECT_EQ(url.scheme(), "https");
    EXPECT_EQ(url.authority(), "user:pass@example.com:80");
    EXPECT_EQ(url.user_information().username(), "user");
    EXPECT_EQ(url.user_information().password(), "pass");
    EXPECT_EQ(url.host(), "example.com");
    EXPECT_EQ(url.port(), "80");
    EXPECT_EQ(url.path(), "user");
    EXPECT_EQ(url.query(), "q=foo");
    EXPECT_EQ(url.fragment(), "bar");
}

TEST(url_test, parses_simple_url) {
    URL url("http://example.com");

    EXPECT_EQ(url.scheme(), "http");
    EXPECT_EQ(url.authority(), "example.com");
    EXPECT_EQ(url.user_information().username(), "");
    EXPECT_EQ(url.user_information().password(), "");
    EXPECT_EQ(url.host(), "example.com");
    EXPECT_EQ(url.port(), "");
    EXPECT_EQ(url.path(), "");
    EXPECT_EQ(url.query(), "");
    EXPECT_EQ(url.fragment(), "");
}

TEST(url_test, parses_simple_url_with_path) {
    URL url("https://example.com/user");

    EXPECT_EQ(url.scheme(), "https");
    EXPECT_EQ(url.authority(), "example.com");
    EXPECT_EQ(url.user_information().username(), "");
    EXPECT_EQ(url.user_information().password(), "");
    EXPECT_EQ(url.host(), "example.com");
    EXPECT_EQ(url.port(), "");
    EXPECT_EQ(url.path(), "user");
    EXPECT_EQ(url.query(), "");
    EXPECT_EQ(url.fragment(), "");
}

TEST(url_test, parses_url_with_multiple_path_segments) {
    URL url("https://www.example.com/user/david/image.png");

    EXPECT_EQ(url.scheme(), "https");
    EXPECT_EQ(url.authority(), "www.example.com");
    EXPECT_EQ(url.user_information().username(), "");
    EXPECT_EQ(url.user_information().password(), "");
    EXPECT_EQ(url.host(), "www.example.com");
    EXPECT_EQ(url.port(), "");
    EXPECT_EQ(url.path(), "user/david/image.png");
    EXPECT_EQ(url.query(), "");
    EXPECT_EQ(url.fragment(), "");
}

TEST(url_test, parses_url_with_query_and_fragment_no_path) {
    URL url("https://example.com?q=foo#bar");

    EXPECT_EQ(url.scheme(), "https");
    EXPECT_EQ(url.authority(), "example.com");
    EXPECT_EQ(url.user_information().username(), "");
    EXPECT_EQ(url.user_information().password(), "");
    EXPECT_EQ(url.host(), "example.com");
    EXPECT_EQ(url.port(), "");
    EXPECT_EQ(url.path(), "");
    EXPECT_EQ(url.query(), "q=foo");
    EXPECT_EQ(url.fragment(), "bar");
}

TEST(url_test, parses_url_with_query_and_fragment_no_path_after_slash) {
    URL url("https://example.com/?q=foo#bar");

    EXPECT_EQ(url.scheme(), "https");
    EXPECT_EQ(url.authority(), "example.com");
    EXPECT_EQ(url.user_information().username(), "");
    EXPECT_EQ(url.user_information().password(), "");
    EXPECT_EQ(url.host(), "example.com");
    EXPECT_EQ(url.port(), "");
    EXPECT_EQ(url.path(), "");
    EXPECT_EQ(url.query(), "q=foo");
    EXPECT_EQ(url.fragment(), "bar");
}

TEST(url_test, parses_url_with_query_no_path) {
    URL url("https://example.com?q=foo");

    EXPECT_EQ(url.scheme(), "https");
    EXPECT_EQ(url.authority(), "example.com");
    EXPECT_EQ(url.user_information().username(), "");
    EXPECT_EQ(url.user_information().password(), "");
    EXPECT_EQ(url.host(), "example.com");
    EXPECT_EQ(url.port(), "");
    EXPECT_EQ(url.path(), "");
    EXPECT_EQ(url.query(), "q=foo");
    EXPECT_EQ(url.fragment(), "");
}

TEST(url_test, parses_url_with_query_and_path) {
    URL url("https://example.com/user?q=foo");

    EXPECT_EQ(url.scheme(), "https");
    EXPECT_EQ(url.authority(), "example.com");
    EXPECT_EQ(url.user_information().username(), "");
    EXPECT_EQ(url.user_information().password(), "");
    EXPECT_EQ(url.host(), "example.com");
    EXPECT_EQ(url.port(), "");
    EXPECT_EQ(url.path(), "user");
    EXPECT_EQ(url.query(), "q=foo");
    EXPECT_EQ(url.fragment(), "");
}

TEST(url_test, parses_url_with_fragment_no_path) {
    URL url("https://example.com#bar");

    EXPECT_EQ(url.scheme(), "https");
    EXPECT_EQ(url.authority(), "example.com");
    EXPECT_EQ(url.user_information().username(), "");
    EXPECT_EQ(url.user_information().password(), "");
    EXPECT_EQ(url.host(), "example.com");
    EXPECT_EQ(url.port(), "");
    EXPECT_EQ(url.path(), "");
    EXPECT_EQ(url.query(), "");
    EXPECT_EQ(url.fragment(), "bar");
}

TEST(url_test, parses_url_with_fragment_and_path) {
    URL url("https://example.com/user#bar");

    EXPECT_EQ(url.scheme(), "https");
    EXPECT_EQ(url.authority(), "example.com");
    EXPECT_EQ(url.user_information().username(), "");
    EXPECT_EQ(url.user_information().password(), "");
    EXPECT_EQ(url.host(), "example.com");
    EXPECT_EQ(url.port(), "");
    EXPECT_EQ(url.path(), "user");
    EXPECT_EQ(url.query(), "");
    EXPECT_EQ(url.fragment(), "bar");
}

TEST(url_test, parses_url_with_empty_user_info) {
    URL url("https://@example.com");

    EXPECT_EQ(url.scheme(), "https");
    EXPECT_EQ(url.authority(), "@example.com");
    EXPECT_EQ(url.user_information().username(), "");
    EXPECT_EQ(url.user_information().password(), "");
    EXPECT_EQ(url.host(), "example.com");
    EXPECT_EQ(url.port(), "");
    EXPECT_EQ(url.path(), "");
    EXPECT_EQ(url.query(), "");
    EXPECT_EQ(url.fragment(), "");
}

TEST(url_test, parses_url_with_username_and_no_password) {
    URL url("https://user@example.com");

    EXPECT_EQ(url.scheme(), "https");
    EXPECT_EQ(url.authority(), "user@example.com");
    EXPECT_EQ(url.user_information().username(), "user");
    EXPECT_EQ(url.user_information().password(), "");
    EXPECT_EQ(url.host(), "example.com");
    EXPECT_EQ(url.port(), "");
    EXPECT_EQ(url.path(), "");
    EXPECT_EQ(url.query(), "");
    EXPECT_EQ(url.fragment(), "");
}

TEST(url_test, parses_url_with_password_and_no_username) {
    URL url("https://:pass@example.com");

    EXPECT_EQ(url.scheme(), "https");
    EXPECT_EQ(url.authority(), ":pass@example.com");
    EXPECT_EQ(url.user_information().username(), "");
    EXPECT_EQ(url.user_information().password(), "pass");
    EXPECT_EQ(url.host(), "example.com");
    EXPECT_EQ(url.port(), "");
    EXPECT_EQ(url.path(), "");
    EXPECT_EQ(url.query(), "");
    EXPECT_EQ(url.fragment(), "");
}

TEST(url_test, throws_if_the_url_is_empty) {
    EXPECT_THROW({
        try {
            URL url("");
        } catch (const URLError& e) {
            EXPECT_STREQ("Invalid URL", e.what());
            throw;
        }
    }, URLError);
}

TEST(url_test, throws_if_theres_no_scheme) {
    EXPECT_THROW({
        try {
            URL url("example.com");
        } catch (const URLError& e) {
            EXPECT_STREQ("Invalid URL scheme", e.what());
            throw;
        }
    }, URLError);
}

TEST(url_test, throws_if_theres_an_invalid_scheme_delimiter) {
    EXPECT_THROW({
        try {
            URL url("https:/example.com");
        } catch (const URLError& e) {
            EXPECT_STREQ("Invalid URL scheme", e.what());
            throw;
        }
    }, URLError);
}