// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include "http/data_readers.h"

#include <gtest/gtest.h>

#include "express/exception.h"
#include "express/response.h"

TEST(ConnectionClose, ReadsBody) {
    Express::Response response;
    Express::Http::ConnectionClose reader(response);

    reader.Feed("Hello World");

    EXPECT_EQ(response.data, "Hello World");
}

TEST(ConnectionClose, ReadsBodyInStream) {
    Express::Response response;
    Express::Http::ConnectionClose reader(response);

    reader.Feed("Hello ");
    reader.Feed("World");

    EXPECT_EQ(response.data, "Hello World");
}

TEST(ContentLength, ReadsBody) {
    Express::Response response;
    response.headers.Add("Content-Length", "11");

    Express::Http::ContentLength reader(response);

    reader.Feed("Hello World");

    EXPECT_EQ(response.data, "Hello World");
}

TEST(ContentLength, ReadsBodyInStream) {
    Express::Response response;
    response.headers.Add("Content-Length", "11");

    Express::Http::ContentLength reader(response);

    reader.Feed("Hel");
    reader.Feed("lo Wo");
    reader.Feed("rld");

    EXPECT_EQ(response.data, "Hello World");
}

TEST(ContentLength, SkipsAdditionalData) {
    Express::Response response;
    response.headers.Add("Content-Length", "11");

    Express::Http::ContentLength reader(response);

    reader.Feed("Hello World ----");

    EXPECT_EQ(response.data, "Hello World");
}

TEST(ChunkedTransfer, ReadsBody) {
    Express::Response response;
    response.headers.Add("Transfer-Encoding", "chunked");

    Express::Http::ChunkedTransfer reader(response);

    reader.Feed(
        "8\r\n"
        "Mozilla \r\n"
        "11\r\n"
        "Developer Network\r\n"
        "0\r\n"
        "\r\n"
    );

    EXPECT_EQ(response.data, "Mozilla Developer Network");
}

TEST(ChunkedTransfer, ReadsBodyInStream) {
    Express::Response response;
    response.headers.Add("Transfer-Encoding", "chunked");

    Express::Http::ChunkedTransfer reader(response);

    reader.Feed("8\r\n");
    reader.Feed("Mozilla \r");
    reader.Feed("\n");
    reader.Feed("11\r\n");
    reader.Feed("Develop");
    reader.Feed("er Network\r");
    reader.Feed("\n");
    reader.Feed("0\r\n");

    EXPECT_EQ(response.data, "Mozilla Developer Network");
}

TEST(ChunkedTransfer, ThrowsErrorIfMissingDelimiter) {
    Express::Response response;
    response.headers.Add("Transfer-Encoding", "chunked");

    Express::Http::ChunkedTransfer reader(response);

    EXPECT_THROW({
        try {
            reader.Feed(
                "8\r\n"
                "Mozilla " // missing delimiter
                "11\r\n"
                "Developer Network\r\n"
                "0\r\n"
                "\r\n"
            );
        } catch (Express::ResponseError& e) {
            EXPECT_STREQ(
                e.what(),
                "Response error: Every chunk must end with a delimiter"
            );
            throw;
        }
    }, Express::ResponseError);
}

TEST(ChunkedTransfer, ThrowsErrorIfChunkSizeIsInvalid) {
    Express::Response response;
    response.headers.Add("Transfer-Encoding", "chunked");

    Express::Http::ChunkedTransfer reader(response);

    EXPECT_THROW({
        try {
            reader.Feed(
                "8\r\n"
                "Mozilla \r\n"
                "s1\r\n" // invalid chunk size
                "Developer Network\r\n"
                "0\r\n"
                "\r\n"
            );
        } catch (Express::ResponseError& e) {
            EXPECT_STREQ(
                e.what(),
                "Response error: Invalid chunk size"
            );
            throw;
        }
    }, Express::ResponseError);
}