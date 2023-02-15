// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <cstdint>
#include <string>
#include <cinttypes>

#include <gtest/gtest.h>
#include <express/response.h>
#include <vector>

using namespace Express::Http;

auto to_input(std::string input) {
    return std::vector<std::uint8_t> {
        begin(input),
        end(input)
    };
}

TEST(response_parser, parses_a_valid_response_successfully) {
    auto input = to_input(
        "HTTP/1.1 200 OK\r\n"
        "Server: Werkzeug/2.2.2 Python/3.10.6\r\n"
        "Date: Sun, 12 Feb 2023 22:29:15 GMT\r\n"
        "Content-Type: text/html; charset=utf-8\r\n"
        "Content-Length: 17\r\n"
        "Connection: close\r\n"
        "\r\n"
        "Hello World!"
    );
    
    ResponseParser parser;    
    parser.feed(input.data(), input.size());

    auto response = parser.response();

    EXPECT_EQ(response.status_code, 200);
    EXPECT_EQ(response.status_text, "OK");
    EXPECT_EQ(response.headers.size(), 5);
    // TODO: assert headers
}

TEST(response_parser_status_line, throws_error_malformed_status_line) {
    auto input = to_input("XHTTPX/1.1 200 OK\r\nContent-Length: 17\r\n\r\n");

    ResponseParser parser;
    EXPECT_THROW({
        try {
            parser.feed(input.data(), input.size());
        } catch (const ResponseError& e) {
            EXPECT_STREQ(e.what(), "Malformed status line");
            throw;
        }
    }, ResponseError);
}

TEST(response_parser_status_line, throws_error_unsupported_version) {
    auto input = to_input("HTTP/2.0 200 OK\r\nContent-Length: 17\r\n\r\n");

    ResponseParser parser;
    EXPECT_THROW({
        try {
            parser.feed(input.data(), input.size());
        } catch (const ResponseError& e) {
            EXPECT_STREQ(e.what(), "Unsupported HTML version (2.0)");
            throw;
        }
    }, ResponseError);
}

TEST(response_parser_status_line, throws_error_unsupported_code) {
    auto input = to_input("HTTP/1.0 2999 OK\r\nContent-Length: 17\r\n\r\n");

    ResponseParser parser;
    EXPECT_THROW({
        try {
            parser.feed(input.data(), input.size());
        } catch (const ResponseError& e) {
            EXPECT_STREQ(e.what(), "Invalid status code (2999)");
            throw;
        }
    }, ResponseError);

    input = to_input("HTTP/1.0 32X OK\r\nContent-Length: 17\r\n\r\n");

    ResponseParser another_parser;
    EXPECT_THROW({
        try {
            another_parser.feed(input.data(), input.size());
        } catch (const ResponseError& e) {
            EXPECT_STREQ(e.what(), "Invalid status code (32X)");
            throw;
        }
    }, ResponseError);
}

TEST(response_parser_status_line, throws_error_invalid_char_reason_phrase) {
    auto input = to_input("HTTP/1.0 200 \fOK\r\nContent-Length: 17\r\n\r\n");

    ResponseParser parser;
    EXPECT_THROW({
        try {
            parser.feed(input.data(), input.size());
        } catch (const ResponseError& e) {
            EXPECT_STREQ(e.what(), "Invalid characters in reason phrase");
            throw;
        }
    }, ResponseError);
}