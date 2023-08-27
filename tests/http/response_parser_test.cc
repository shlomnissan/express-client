// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include "http/response_parser.h"

#include <string>

#include <gtest/gtest.h>

#include "express/exception.h"

struct ResponseParser : public ::testing::Test {
    [[no_unique_address]] Express::Http::ResponseParser parser{};
};

TEST_F(ResponseParser, ParsesResponse) {
    unsigned char input[] {
        "HTTP/1.1 200 OK\r\n"
        "Server: Werkzeug/2.2.2 Python/3.10.6\r\n"
        "Date: Sun, 12 Feb 2023 22:29:15 GMT\r\n"
        "Content-Type: text/html; charset=utf-8\r\n"
        "Connection: close\r\n"
        "\r\n"
        "Hello World!"
    };

    parser.Feed(input, sizeof(input));
    auto response = parser.response();

    EXPECT_EQ(response.status_code, 200);
    EXPECT_EQ(response.status_text, "OK");
    EXPECT_EQ(response.data, "Hello World!");

    auto& h = response.headers; 
    EXPECT_EQ(h.Get("Server"), "Werkzeug/2.2.2 Python/3.10.6");
    EXPECT_EQ(h.Get("Date"), "Sun, 12 Feb 2023 22:29:15 GMT");
    EXPECT_EQ(h.Get("Content-Type"), "text/html; charset=utf-8");
    EXPECT_EQ(h.Get("Connection"), "close");
}

TEST_F(ResponseParser, ParsesResponseWithContentLength) {
    unsigned char input[] {
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: 12\r\n"
        "\r\n"
        "Hello World!"
    };

    parser.Feed(input, sizeof(input));
    auto response = parser.response();

    EXPECT_EQ(response.status_code, 200);
    EXPECT_EQ(response.status_text, "OK");
    EXPECT_EQ(response.data, "Hello World!");
}

TEST_F(ResponseParser, ParsesResponseWithChunkedTransfer) {
    unsigned char input[] {
        "HTTP/1.1 200 OK\r\n"
        "Transfer-Encoding: chunked\r\n"
        "\r\n"
        "8\r\n"
        "Mozilla \r\n"
        "11\r\n"
        "Developer Network\r\n"
        "0\r\n"
        "\r\n"
    };

    parser.Feed(input, sizeof(input));
    auto response = parser.response();

    EXPECT_EQ(response.status_code, 200);
    EXPECT_EQ(response.status_text, "OK");
    EXPECT_EQ(response.data, "Mozilla Developer Network");
}

TEST_F(ResponseParser, ParsesResponseWithObsoleteLineFolding) {
    unsigned char input[] {
        "HTTP/1.0 200 OK\r\n"
        "Server: Werkzeug/2.2.2 Python/3.10.6\r\n"
        "Content-Length:\r\n "
        "\t 12\r\n"
        "\r\n"
        "Hello World!"
    };

    parser.Feed(input, sizeof(input));
    auto response = parser.response();

    EXPECT_EQ(response.status_code, 200);
    EXPECT_EQ(response.status_text, "OK");
    EXPECT_EQ(response.data, "Hello World!");

    auto& h = response.headers; 
    EXPECT_EQ(h.Get("Server"), "Werkzeug/2.2.2 Python/3.10.6");
}

TEST_F(ResponseParser, ParsesResponseWithEmptyBody) {
    unsigned char input[] {
        "HTTP/1.1 200 OK\r\n"
        "\r\n"
    };

    parser.Feed(input, sizeof(input));
    auto response = parser.response();

    EXPECT_EQ(response.status_code, 200);
    EXPECT_EQ(response.status_text, "OK");
    EXPECT_EQ(response.data, "");
}

TEST_F(ResponseParser, ThrowsErrorIfFetchingIncompleteResponse) {
    unsigned char input[] {
        "HTTP/1.0 200 OK\r\n"
        "Server: Werkzeug/2.2.2 Python/3.10.6\r\n"
        "Content-Length: 20\r\n"
        "\r\n"
        "Missing data"
    };

    parser.Feed(input, sizeof(input));

    EXPECT_THROW({
        try {
            auto response = parser.response();
        } catch(Express::ResponseError e) {
            EXPECT_STREQ(
                e.what(),
                "Response error: Incomplete data transfer"
            );
            throw;
        }
    }, Express::ResponseError);
}

TEST_F(ResponseParser, ThrowsErrorIfInvalidHeader) {
    unsigned char input[] {
        "HTTP/1.0 200 OK\r\n"
        "Server; Werkzeug/2.2.2 Python/3.10.6\r\n"
        "Content-Length; 12\r\n"
        "\r\n"
        "Hello World!"
    };

    EXPECT_THROW({
        try {
            parser.Feed(input, sizeof(input));
        } catch(Express::ResponseError e) {
            EXPECT_STREQ(
                e.what(),
                "Response error: Failed to process invalid response header"
            );
            throw;
        }
    }, Express::ResponseError);
}

TEST_F(ResponseParser, ThrowsErrorIfMultipleContentLengthFields) {
    unsigned char input[] {
        "HTTP/1.0 200 OK\r\n"
        "Content-Length: 12\r\n"
        "Content-Length: 13\r\n"
        "\r\n"
        "Hello World!"
    };

    EXPECT_THROW({
        try {
            parser.Feed(input, sizeof(input));
        } catch(Express::ResponseError e) {
            EXPECT_STREQ(
                e.what(),
                "Response error: Received multiple content length fields "
                "with different values"
            );
            throw;
        }
    }, Express::ResponseError);
}

TEST_F(ResponseParser, ThrowsErrorIfUnsupportedTransferEncoding) {
    unsigned char input[] {
        "HTTP/1.1 200 OK\r\n"
        "Transfer-Encoding: compress\r\n"
        "\r\n"
        "XXXXXX"
    }; 

    EXPECT_THROW({
        try {
            parser.Feed(input, sizeof(input));
        } catch(Express::ResponseError e) {
            EXPECT_STREQ(
                e.what(),
                "Response error: Unsupport transfer encoding (compress)"
            );
            throw;
        }
    }, Express::ResponseError);
}