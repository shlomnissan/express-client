// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include "express/headers.h"

#include <gtest/gtest.h>

#include "express/exception.h"

TEST(Headers, InitializesWithStringPairs) {
    Express::Headers headers {{
        {"Content-Type", "application/json; charset=utf-8"},
        {"Content-Encoding", "gzip"}
    }};

    EXPECT_EQ(headers.Get("Content-Type"), "application/json; charset=utf-8");
    EXPECT_EQ(headers.Get("Content-Encoding"), "gzip");
}

TEST(Headers, AddsHeader) {
    Express::Headers headers;

    headers.Add("Content-Type", "application/json; charset=utf-8");
    headers.Add("Content-Encoding", "   gzip   ");

    EXPECT_EQ(headers.Get("Content-Type"), "application/json; charset=utf-8");
    EXPECT_EQ(headers.Get("content-encoding"), "gzip");
}

TEST(Headers, RemovesHeader) {
    Express::Headers headers {{
        {"Content-Type", "application/json; charset=utf-8"},
        {"Content-Encoding", "gzip"}
    }};

    headers.Remove("Content-Type");
    headers.Remove("content-encoding");

    EXPECT_FALSE(headers.Contains("Content-Type"));
    EXPECT_FALSE(headers.Contains("Content-Encoding"));
}

TEST(Headers, GetsHeaderValueWithName) {
    Express::Headers headers {{
        {"Content-Type", "application/json; charset=utf-8"},
        {"Content-Encoding", "gzip"}
    }};

    EXPECT_EQ(headers.Get("Content-Type"), "application/json; charset=utf-8");
    EXPECT_EQ(headers.Get("content-encoding"), "gzip");
}

TEST(Headers, ChecksIfHeaderExistsWithName) {
    Express::Headers headers {{
        {"Content-Type", "application/json; charset=utf-8"},
        {"Content-Encoding", "gzip"}
    }};

    EXPECT_TRUE(headers.Contains("Content-Type"));
    EXPECT_TRUE(headers.Contains("content-encoding"));
    EXPECT_FALSE(headers.Contains("Authorization"));
}

TEST(Headers, IteratesThroughHeaders) {
    Express::Headers headers {{
        {"Content-Type", "application/json; charset=utf-8"},
        {"Content-Encoding", "gzip"}
    }};

    for (const auto& [_, header] : headers) {
        if (header.first == "Content-Type") {
            EXPECT_EQ(header.second, "application/json; charset=utf-8");
        }
        if (header.first == "Content-Encoding") {
            EXPECT_EQ(header.second, "gzip");
        }
    }
}

TEST(Headers, ThrowsErrorIfAddingWithInvalidName) {
    Express::Headers headers;

    EXPECT_THROW({
        try {
            headers.Add("Invalid-Token@", "Valid test value");
        } catch (Express::RequestError& e) {
            EXPECT_STREQ(
                "Header error: Invalid header name",
                e.what()
            );
            throw;
        }
    }, Express::RequestError);
}

TEST(Headers, ThrowsErrorIfAddingWithInvalidValue) {
    Express::Headers headers;

    EXPECT_THROW({
        try {
            headers.Add("Valid-Test-Name", "Invisible form feed character \f");
        } catch (Express::RequestError& e) {
            EXPECT_STREQ(
                "Header error: Invalid header value",
                e.what()
            );
            throw;
        }
    }, Express::RequestError);
}

TEST(Headers, ThrowsErrorIfRemovingNonExistingHeader) {
    Express::Headers headers {{{"Content-Encoding", "gzip"}}};

    EXPECT_THROW({
        try {
            headers.Remove("Content-Type");
        } catch(Express::RequestError& e) {
            EXPECT_STREQ(
                "Header error: Attempting to remove a header that doesn't exist",
                e.what()
            ); 
            throw;
        }
    }, Express::RequestError);
}