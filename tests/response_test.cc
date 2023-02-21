// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <cstdint>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include <express/response.h>
#include <express/header.h>

using namespace Express::Http;

auto str_to_data(const std::string& str) {
    return std::vector<std::uint8_t> {begin(str), end(str)};
}

auto data_to_str(const std::vector<uint8_t>& data) {
    return std::string {begin(data), end(data)};
}

TEST(response_parser, parses_a_valid_response_successfully) {
    auto input = str_to_data(
        "HTTP/1.1 200 OK\r\n"
        "Server: Werkzeug/2.2.2 Python/3.10.6\r\n"
        "Date: Sun, 12 Feb 2023 22:29:15 GMT\r\n"
        "Content-Type: text/html; charset=utf-8\r\n"
        "Content-Length: 16\r\n"
        "Connection: close\r\n"
        "\r\n"
        "Hello GET World!"
    );
    
    ResponseParser parser;    
    parser.feed(input.data(), input.size());

    auto response = parser.response();

    EXPECT_EQ(response.status_code, 200);
    EXPECT_EQ(response.status_text, "OK");
    EXPECT_EQ(response.headers.size(), 5);
    EXPECT_EQ(response.headers.get("Server"), "Werkzeug/2.2.2 Python/3.10.6");
    EXPECT_EQ(response.headers.get("Date"), "Sun, 12 Feb 2023 22:29:15 GMT");
    EXPECT_EQ(response.headers.get("Content-Type"), "text/html; charset=utf-8");
    EXPECT_EQ(response.headers.get("Content-Length"), "16");
    EXPECT_EQ(response.headers.get("Connection"), "close");

    auto body = data_to_str(response.body);
    EXPECT_EQ(body, "Hello GET World!");
}

TEST(response_parser_status_line, throws_error_malformed_status_line) {
    auto input = str_to_data(
        "XHTTPX/1.1 200 OK\r\n"
        "Content-Length: 17\r\n"
        "\r\n"
    );

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
    auto input = str_to_data(
        "HTTP/2.0 200 OK\r\n"
        "Content-Length: 17\r\n"
        "\r\n"
    );

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
    auto input = str_to_data(
        "HTTP/1.0 2999 OK\r\n"
        "Content-Length: 17\r\n"
        "\r\n"
    );

    ResponseParser parser;
    EXPECT_THROW({
        try {
            parser.feed(input.data(), input.size());
        } catch (const ResponseError& e) {
            EXPECT_STREQ(e.what(), "Invalid status code (2999)");
            throw;
        }
    }, ResponseError);

    input = str_to_data(
        "HTTP/1.0 32X OK\r\n"
        "Content-Length: 17\r\n"
        "\r\n"
    );

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
    auto input = str_to_data(
        "HTTP/1.0 200 \fOK\r\n"
        "Content-Length: 17\r\n"
        "\r\n"
    );

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

TEST(response_parser_headers, throws_when_parsing_invalid_header) {
    auto input = str_to_data(
        "HTTP/1.0 200 OK\r\n"
        "Server Werkzeug/2.2.2 Python/3.10.6\r\n"
        "\r\n"
    );
    
    ResponseParser parser;
    EXPECT_THROW({
        try {
            parser.feed(input.data(), input.size());
        } catch (const ResponseError& e) {
            EXPECT_STREQ(e.what(), "Failed to process invalid response header");
            throw;
        }
    }, ResponseError);
}

TEST(response_parser_headers, throws_when_parsing_invalid_header_name) {
    auto input = str_to_data(
        "HTTP/1.0 200 OK\r\n"
        "Sun, 12 Feb 2023 22:29:15 GMT\r\n"
        "\r\n"
    );

    ResponseParser parser;
    EXPECT_THROW({
        try {
            parser.feed(input.data(), input.size());
        } catch (const HeaderError& e) {
            EXPECT_STREQ(e.what(), "Invalid header name.");
            throw;
        }
    }, HeaderError);
}

TEST(response_parser_headers, throws_when_parsing_invalid_header_value) {
    auto input = str_to_data(
        "HTTP/1.0 200 OK\r\n"
        "Date: Sun, 12 Feb  \f2023 22:29:15 GMT\r\n"
        "\r\n"
    );

    ResponseParser parser;
    EXPECT_THROW({
        try {
            parser.feed(input.data(), input.size());
        } catch (const HeaderError& e) {
            EXPECT_STREQ(e.what(), "Invalid header value.");
            throw;
        }
    }, HeaderError);
}

TEST(response_parser_headers, handles_obsolete_line_folding) {
    auto input = str_to_data(
        "HTTP/1.0 200 OK\r\n"
        "Server: Werkzeug/2.2.2 Python/3.10.6\r\n"
        "Content-Length:\r\n "
        "\t 16\r\n"
        "\r\n"
        "Hello GET World!"
    );

    ResponseParser parser;
    parser.feed(input.data(), input.size());

    auto response = parser.response();

    EXPECT_EQ(response.headers.size(), 2);
    EXPECT_EQ(response.headers.get("Server"), "Werkzeug/2.2.2 Python/3.10.6");
    EXPECT_EQ(response.headers.get("Content-Length"), "16");

    auto body = data_to_str(response.body);
    EXPECT_EQ(body, "Hello GET World!");

    input = str_to_data(
        "HTTP/1.0 200 OK\r\n"
        "Server: Werk\r\n "
        "zeug/2.2.2\r\n\t"
        "Python/3.10.6\r\n"
        "Content-Length:16\r\n"
        "\r\n"
        "Hello GET World!"
    );

    ResponseParser another_parser;    
    another_parser.feed(input.data(), input.size());

    auto another_response = parser.response();
    EXPECT_EQ(another_response.headers.size(), 2);
    EXPECT_EQ(another_response.headers.get("Server"), "Werkzeug/2.2.2 Python/3.10.6");
    EXPECT_EQ(another_response.headers.get("Content-Length"), "16");

    auto another_body = data_to_str(another_response.body);
    EXPECT_EQ(another_body, "Hello GET World!");
}

TEST(response_parser_headers, throws_if_multiple_content_length_values) {
    auto input = str_to_data(
        "HTTP/1.0 200 OK\r\n"
        "Server: Werkzeug/2.2.2 Python/3.10.6\r\n"
        "Content-Length: 17\r\n"
        "Content-Length: 14\r\n"
        "\r\n"
    );

    ResponseParser parser;
    EXPECT_THROW({
        try {
            parser.feed(input.data(), input.size());
        } catch (const ResponseError& e) {
            EXPECT_STREQ(e.what(), "Received multiple content length fields.");
            throw;
        }
    }, ResponseError);
}

TEST(response_parser_headers, throws_if_content_length_is_invalid) {
    auto input = str_to_data(
        "HTTP/1.0 200 OK\r\n"
        "Server: Werkzeug/2.2.2 Python/3.10.6\r\n"
        "Content-Length: 0x17\r\n"
        "\r\n"
    );
    ResponseParser parser;
    EXPECT_THROW({
        try {
            parser.feed(input.data(), input.size());
        } catch (const ResponseError& e) {
            EXPECT_STREQ(e.what(), "Invalid content length value");
            throw;
        }
    }, ResponseError);
}

TEST(response_parser_body_content_length, parses_body_with_content_length_correctly) {
    auto input_0 = str_to_data(
        "HTTP/1.0 200 OK\r\n"
        "Server: Werkzeug/2.2.2 Python/3.10.6\r\n"
        "Content-Length: 16\r\n"
        "\r\n"
        "Hel"
    );

    auto input_1 = str_to_data(
        "lo GET World!"
    );

    ResponseParser parser;
    parser.feed(input_0.data(), input_0.size());
    parser.feed(input_1.data(), input_1.size());

    auto response = parser.response();
    EXPECT_EQ(data_to_str(response.body), "Hello GET World!");
}

TEST(response_parser_body_content_length, skips_body_data_that_exceeds_content_length) {
    auto input_0 = str_to_data(
        "HTTP/1.0 200 OK\r\n"
        "Server: Werkzeug/2.2.2 Python/3.10.6\r\n"
        "Content-Length: 16\r\n"
        "\r\n"
        "Hel"
    );

    auto input_1 = str_to_data(
        "lo GET World!"
        "THIS LINE SHOULD BE IGNORED"
    );

    ResponseParser parser;
    parser.feed(input_0.data(), input_0.size());
    parser.feed(input_1.data(), input_1.size());

    auto response = parser.response();
    EXPECT_EQ(data_to_str(response.body), "Hello GET World!");
}

TEST(response_parser_body_content_length, throws_when_fetching_incomplete_response) {
    auto input = str_to_data(
        "HTTP/1.0 200 OK\r\n"
        "Server: Werkzeug/2.2.2 Python/3.10.6\r\n"
        "Content-Length: 16\r\n"
        "\r\n"
        "Hel"
    );

    ResponseParser parser;
    parser.feed(input.data(), input.size());

    EXPECT_THROW({
        try {
            auto response = parser.response();
        } catch (const ResponseError& e) {
            EXPECT_STREQ(e.what(), "Incomplete data transfer.");
            throw;
        }
    }, ResponseError);
}

// TODO: response_parser_body_connection_close, parses_body_until_connection_closes
// TODO: parses a response with encoding that's not chuncked
// TODO: parses a response with a body but no explicit body size