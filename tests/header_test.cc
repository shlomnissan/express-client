// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <gtest/gtest.h>
#include <iostream>
#include <express/header.h>
#include <sstream>
#include <string>

using namespace Express::Http;

TEST(header, creates_valid_header_object) {
    Header header {"Host", "example.com"};

    EXPECT_EQ(header.name(), "Host");
    EXPECT_EQ(header.value(), "example.com");

    std::stringstream buffer;
    buffer << header;
    EXPECT_EQ(buffer.str(), "Host: example.com\r\n");
}

TEST(header, trims_leading_and_trailing_whitespaces_from_value) {
    Header header {"Host", " \t example.com  \t "};

    EXPECT_EQ(header.name(), "Host");
    EXPECT_EQ(header.value(), "example.com");

    std::stringstream buffer;
    buffer << header;
    EXPECT_EQ(buffer.str(), "Host: example.com\r\n");
}

TEST(header, throws_if_header_name_is_empty) {
    EXPECT_THROW({
        try {
            Header header("", "value");
        } catch (const HeaderError& e) {
            EXPECT_STREQ("Invalid header name.", e.what());
            throw;
        }
    }, HeaderError);
}

TEST(header, throws_if_header_name_contains_leading_whitespaces) {
    EXPECT_THROW({
        try {
            Header header("\t Host", "example.com");
        } catch (const HeaderError& e) {
            EXPECT_STREQ("Invalid header name.", e.what());
            throw;
        }
    }, HeaderError);
}

TEST(header, throws_if_header_name_contains_trailing_whitespaces) {
    EXPECT_THROW({
        try {
            Header header("Host ", "example.com");
        } catch (const HeaderError& e) {
            EXPECT_STREQ("Invalid header name.", e.what());
            throw;
        }
    }, HeaderError);
}

TEST(header, throws_if_the_header_name_contains_invalid_characters) {
    EXPECT_THROW({
        try {
            Header header("Invalid-Token(@", "value");
        } catch (const HeaderError& e) {
            EXPECT_STREQ("Invalid header name.", e.what());
            throw;
        }
    }, HeaderError);
}

TEST(header, throws_if_the_header_value_contains_invalid_characters) {
    EXPECT_THROW({
        try {
            Header header("Host", "Invisible form feed character \f");
        } catch (const HeaderError& e) {
            EXPECT_STREQ("Invalid header value.", e.what());
            throw;
        }
    }, HeaderError);
}

TEST(header_collection, initializes_an_object_with_initializer_list) {
    HeaderCollection headers {{
        {"Host", "example.com"},
        {"User-Agent", "express/0.1"}
    }};

    EXPECT_EQ(headers.front().name(), "Host");
    EXPECT_EQ(headers.front().value(), "example.com");
    EXPECT_EQ(headers.back().name(), "User-Agent");
    EXPECT_EQ(headers.back().value(), "express/0.1");
}

TEST(header_collection, overrides_headers_if_the_same_name_is_used) {
    HeaderCollection headers {{
        {"Host", "example.com"},
    }};
    headers.add({"Host", "google.com"});

    EXPECT_EQ(headers.front().name(), "Host");
    EXPECT_EQ(headers.front().value(), "google.com");
}

TEST(header_collection, adds_headers_to_the_collection) {
    HeaderCollection headers;
    headers.add({"Host", "example.com"});

    EXPECT_EQ(headers.front().name(), "Host");
    EXPECT_EQ(headers.front().value(), "example.com");
}

TEST(header_collection, has_method_returns_the_correct_value) {
    HeaderCollection headers {{
        {"Host", "example.com"},
        {"User-Agent", "express/0.1"}
    }};

    EXPECT_EQ(headers.has("Host"), true);
    EXPECT_EQ(headers.has("User-Agent"), true);
    EXPECT_EQ(headers.has("Date"), false);
}

TEST(header_collection, has_method_case_insensitive) {
    HeaderCollection headers {{
        {"Host", "example.com"},
        {"User-Agent", "express/0.1"}
    }};

    EXPECT_EQ(headers.has("host"), true);
    EXPECT_EQ(headers.has("useR-agEnt"), true);
}

TEST(header_Collection, removes_header_correctly) {
    HeaderCollection headers {{
        {"Host", "example.com"},
        {"User-Agent", "express/0.1"}
    }};

    EXPECT_EQ(headers.size(), 2);
    EXPECT_EQ(headers.has("host"), true);

    headers.remove("host");

    EXPECT_EQ(headers.size(), 1);
    EXPECT_EQ(headers.has("host"), false);
    EXPECT_EQ(headers.has("user-agent"), true);

    headers.remove("user-agent");

    EXPECT_EQ(headers.size(), 0);
    EXPECT_EQ(headers.has("user-agent"), false);

    EXPECT_THROW({
        try {
            headers.remove("date");
        } catch (const HeaderError& e) {
            EXPECT_STREQ(e.what(), "Attempting to remove a header that doesn't exist");
            throw;
        }
    }, HeaderError);
}

TEST(header_collection, get_method_returns_the_correct_value) {
    HeaderCollection headers {{
        {"Host", "example.com"},
        {"User-Agent", "express/0.1"}
    }};

    EXPECT_EQ(headers.get("Host"), "example.com");
    EXPECT_EQ(headers.get("User-Agent"), "express/0.1");

    EXPECT_THROW({
        try {
            EXPECT_EQ(headers.get("Date"), "Wed, 15 Feb 2023 14:43:39 GMT"); 
        } catch (const HeaderError& e) {
            EXPECT_STREQ(e.what(),
                "Attempting to access value for a header that doesn't exist."
            );
            throw;
        }
    }, HeaderError);
}

TEST(header_collection, get_method_case_insensitive) {
    HeaderCollection headers {{
        {"Host", "example.com"},
        {"User-Agent", "express/0.1"}
    }};

    EXPECT_EQ(headers.get("host"), "example.com");
    EXPECT_EQ(headers.get("useR-agEnt"), "express/0.1");
}

TEST(header_collection, iterates_through_the_collection_using_iterators) {
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