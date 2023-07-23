// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <gtest/gtest.h>
#include <express/response_parser.h>

#include <cstdint>
#include <express/exception.h>
#include <express/header.h>
#include <string>
#include <vector>

using namespace Express::Http;

auto str_to_data(const std::string& str) {
    return std::vector<std::uint8_t> {cbegin(str), cend(str)};
}

auto data_to_str(const std::vector<uint8_t>& data) {
    return std::string {cbegin(data), cend(data)};
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

    EXPECT_EQ(response.statusCode, 200);
    EXPECT_EQ(response.statusText, "OK");
    EXPECT_EQ(response.headers.size(), 5);
    EXPECT_EQ(response.headers.get("Server"), "Werkzeug/2.2.2 Python/3.10.6");
    EXPECT_EQ(response.headers.get("Date"), "Sun, 12 Feb 2023 22:29:15 GMT");
    EXPECT_EQ(response.headers.get("Content-Type"), "text/html; charset=utf-8");
    EXPECT_EQ(response.headers.get("Content-Length"), "16");
    EXPECT_EQ(response.headers.get("Connection"), "close");

    auto body = data_to_str(response.data);
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
        } catch (const Express::ResponseError& e) {
            EXPECT_STREQ(e.what(), "Response error: Malformed status line");
            throw;
        }
    }, Express::ResponseError);
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
        } catch (const Express:: ResponseError& e) {
            EXPECT_STREQ(e.what(), "Response error: Unsupported HTML version (2.0)");
            throw;
        }
    }, Express::ResponseError);
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
        } catch (const Express:: ResponseError& e) {
            EXPECT_STREQ(e.what(), "Response error: Invalid status code (2999)");
            throw;
        }
    }, Express::ResponseError);

    input = str_to_data(
        "HTTP/1.0 32X OK\r\n"
        "Content-Length: 17\r\n"
        "\r\n"
    );

    ResponseParser another_parser;
    EXPECT_THROW({
        try {
            another_parser.feed(input.data(), input.size());
        } catch (const Express:: ResponseError& e) {
            EXPECT_STREQ(e.what(), "Response error: Invalid status code (32X)");
            throw;
        }
    }, Express::ResponseError);
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
        } catch (const Express:: ResponseError& e) {
            EXPECT_STREQ(e.what(), "Response error: Invalid characters in reason phrase");
            throw;
        }
    }, Express::ResponseError);
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
        } catch (const Express:: ResponseError& e) {
            EXPECT_STREQ(e.what(), "Response error: Failed to process invalid response header");
            throw;
        }
    }, Express::ResponseError);
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

    auto body = data_to_str(response.data);
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

    auto another_body = data_to_str(another_response.data);
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
        } catch (const Express:: ResponseError& e) {
            EXPECT_STREQ(e.what(), "Response error: Received multiple content length fields");
            throw;
        }
    }, Express::ResponseError);
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
        } catch (const Express:: ResponseError& e) {
            EXPECT_STREQ(e.what(), "Response error: Invalid content length value");
            throw;
        }
    }, Express::ResponseError);
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
    EXPECT_EQ(data_to_str(response.data), "Hello GET World!");
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
    EXPECT_EQ(data_to_str(response.data), "Hello GET World!");
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
        } catch (const Express:: ResponseError& e) {
            EXPECT_STREQ(e.what(), "Response error: Incomplete data transfer");
            throw;
        }
    }, Express::ResponseError);
}

TEST(response_parser_body_content_length, parses_body_without_content_length) {
    auto input_0 = str_to_data(
        "HTTP/1.0 200 OK\r\n"
        "Server: Werkzeug/2.2.2 Python/3.10.6\r\n"
        "\r\n"
        "Hel"
    );

    auto input_1 = str_to_data(
        "lo World!"
    );

    ResponseParser parser;
    parser.feed(input_0.data(), input_0.size());
    parser.feed(input_1.data(), input_1.size());

    auto response = parser.response();
    EXPECT_EQ(data_to_str(response.data), "Hello World!");
}

TEST(response_parser_body_chunked, parses_body_with_chunked_encoding_correctly) {
    auto input = str_to_data(
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Transfer-Encoding: chunked\r\n"
        "\r\n"
        "8\r\n"
        "Mozilla \r\n"
        "11\r\n"
        "Developer Network\r\n"
        "0\r\n"
        "\r\n"
    );

    ResponseParser parser;
    parser.feed(input.data(), input.size());

    auto response = parser.response();
    EXPECT_EQ(data_to_str(response.data), "Mozilla Developer Network");
}

TEST(response_parser_body_chunked, parses_body_with_chunked_with_multiple_inputs) {
    auto input_0 = str_to_data(
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Transfer-Encoding: chunked\r\n"
        "\r\n"
        "8\r\n"
        "Mozilla "
    );

    auto input_1 = str_to_data(
        "\r\n"
        "11\r\n"
        "Developer Network\r\n"
    );

    auto input_2 = str_to_data(
        "0\r\n"
        "\r\n"
    );

    ResponseParser parser;
    parser.feed(input_0.data(), input_0.size());
    parser.feed(input_1.data(), input_1.size());
    parser.feed(input_2.data(), input_2.size());
    
    auto response = parser.response();
    EXPECT_EQ(data_to_str(response.data), "Mozilla Developer Network");

    auto input_3 = str_to_data(
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Transfer-Encoding: chunked\r\n"
        "\r\n"
        "8\r\n"
        "Mozil"
    );

    auto input_4 = str_to_data(
        "la \r\n"
        "11\r\n"
        "Developer Network\r\n"
        "0\r\n"
        "\r\n"
    );

    auto input_5 = str_to_data("la \r\n");

    ResponseParser another_parser;
    another_parser.feed(input_3.data(), input_3.size());
    another_parser.feed(input_4.data(), input_4.size());
    another_parser.feed(input_5.data(), input_5.size());

    response = another_parser.response();
    EXPECT_EQ(data_to_str(response.data), "Mozilla Developer Network");
}

TEST(response_parser_body_chunked, throws_error_when_parsing_invalid_chunked_data) {
    auto input = str_to_data(
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Transfer-Encoding: chunked\r\n"
        "\r\n"
        "8\r\n"
        "Mozilla " // missing \r\n at the end of a chunk 
        "11\r\n"
        "Developer Network\r\n"
    );

    ResponseParser parser;

    EXPECT_THROW({
        try {
            parser.feed(input.data(), input.size());
        } catch (const Express:: ResponseError& e) {
            EXPECT_STREQ(e.what(), "Response error: Invalid chunk delimiter");
            throw;
        }
    }, Express::ResponseError);
}

TEST(response_parser_body_chunked, chunked_transfer_encoding_overrides_content_length) {
    auto input = str_to_data(
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 25\r\n"
        "Transfer-Encoding: chunked\r\n"
        "\r\n"
        "8\r\n"
        "Mozilla \r\n" 
        "11\r\n"
        "Developer Network\r\n"
        "0\r\n"
        "\r\n"
    );

    ResponseParser parser;
    parser.feed(input.data(), input.size());
    auto response = parser.response();
    EXPECT_EQ(response.headers.has("Content-Length"), false);
    EXPECT_EQ(data_to_str(response.data), "Mozilla Developer Network");
}

TEST(response_parser_body_chunked, parses_body_with_unsupported_transfer_encoding) {
    auto input_0 = str_to_data(
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 25\r\n" // defaults the content length 
        "Transfer-Encoding: deflate\r\n"
        "\r\n"
        "Mozilla Developer Network"
    );

    ResponseParser parser;
    parser.feed(input_0.data(), input_0.size());

    auto response = parser.response();
    EXPECT_EQ(data_to_str(response.data), "Mozilla Developer Network");

    auto input_1 = str_to_data(
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Transfer-Encoding: deflate\r\n"
        "\r\n"
        "Mozilla Developer Network"
    );

    ResponseParser another_parser;
    another_parser.feed(input_1.data(), input_1.size());

    auto another_response = another_parser.response();
    EXPECT_EQ(data_to_str(another_response.data), "Mozilla Developer Network");
}