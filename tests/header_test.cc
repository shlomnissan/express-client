// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <gtest/gtest.h>
#include <iostream>
#include <express/header.h>
#include <sstream>
#include <string>

using namespace Express::Http;

TEST(header_test, creates_valid_header_object) {
    Header header {"Host", "example.com"};

    EXPECT_EQ(header.name(), "Host");
    EXPECT_EQ(header.value(), "example.com");

    std::stringstream buffer;
    buffer << header;
    EXPECT_EQ(buffer.str(), "Host: example.com\r\n");
}

TEST(header_test, throws_if_the_header_name_is_empty) {
    EXPECT_THROW({
        try {
            Header header("", "value");
        } catch (const HeaderError& e) {
            EXPECT_STREQ("Invalid header name.", e.what());
            throw;
        }
    }, HeaderError);
}

TEST(header_test, throws_if_the_header_name_contains_invalid_characters) {
    EXPECT_THROW({
        try {
            Header header("Invalid-Token(@", "value");
        } catch (const HeaderError& e) {
            EXPECT_STREQ("Invalid header name.", e.what());
            throw;
        }
    }, HeaderError);
}

TEST(header_test, throws_if_the_header_value_contains_invalid_characters) {
    EXPECT_THROW({
        try {
            Header header("Host", "Invisible form feed character \f");
        } catch (const HeaderError& e) {
            EXPECT_STREQ("Invalid header value.", e.what());
            throw;
        }
    }, HeaderError);
}

TEST(collection_header_test, initializes_an_object_with_initializer_list) {
    HeaderCollection headers {{
        {"Host", "example.com"},
        {"User-Agent", "express/0.1"}
    }};

    EXPECT_EQ(headers.front().name(), "Host");
    EXPECT_EQ(headers.front().value(), "example.com");
    EXPECT_EQ(headers.back().name(), "User-Agent");
    EXPECT_EQ(headers.back().value(), "express/0.1");
}

TEST(collection_header_test, adds_headers_to_the_collection) {
    HeaderCollection headers;
    headers.add({"Host", "example.com"});

    EXPECT_EQ(headers.front().name(), "Host");
    EXPECT_EQ(headers.front().value(), "example.com");
}

TEST(collection_header_test, overrides_headers_if_the_same_name_is_used) {
    HeaderCollection headers {{
        {"Host", "example.com"},
    }};
    headers.add({"Host", "google.com"});

    EXPECT_EQ(headers.front().name(), "Host");
    EXPECT_EQ(headers.front().value(), "google.com");
}

TEST(collection_header_test, iterates_through_the_collection_using_iterators) {
    HeaderCollection headers {{
        {"Host", "example.com"},
        {"User-Agent", "express/0.1"}
    }};
    auto iter = 0;
    for (const auto& header : headers) {
        if (iter == 0) {
            EXPECT_EQ(header.name(), "Host");
            EXPECT_EQ(header.value(), "example.com");
        }
        if (iter == 1) {
            EXPECT_EQ(header.name(), "User-Agent");
            EXPECT_EQ(header.value(), "express/0.1");
        }
        ++iter;
    }
}