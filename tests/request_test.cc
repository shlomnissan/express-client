// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include "express/response.h"
#include <sstream>

#include <gtest/gtest.h>
#include <express/request.h>

using namespace Express::Http;

TEST(request, creates_valid_request_object) {
    Express::Net::URL url {"http://example.com"};
    Request request {url, {
        .url = url.source(),
        .method = Method::Get
    }};

    std::stringstream buffer;
    request.writeRequest(buffer);
    EXPECT_EQ(
        buffer.str(),
        "GET / HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "User-Agent: express/0.1\r\n"
        "Connection: close\r\n"
        "\r\n"
    );
}

// TODO: create request with data and raw string
// TODO: throws error if content type isn't set for raw string

TEST(request, creates_valid_request_object_with_form_data) {
    Express::Net::URL url {"http://example.com"};
    Request request {url, {
        .url = url.source(),
        .method = Method::Post,
        .data = {{
            {"firstName", "Fred"},
            {"lastName", "Flintstone"}
        }},
        .headers = {{
            {"Content-Type", "application/x-www-form-urlencoded"}
        }}
    }};

    std::stringstream buffer;
    request.writeRequest(buffer);
    EXPECT_EQ(
        buffer.str(),
        "POST / HTTP/1.1\r\n"
        "Content-Type: application/x-www-form-urlencoded\r\n"
        "Host: example.com\r\n"
        "User-Agent: express/0.1\r\n"
        "Content-Length: 34\r\n"
        "Connection: close\r\n"
        "\r\n"
        "firstName=Fred&lastName=Flintstone"
    );
}

TEST(request, throws_if_http_method_should_not_include_data) {
    Express::Net::URL url {"http://example.com"};

    EXPECT_THROW({
        try {
            Request request(url, {
                .url = url.source(),
                .method = Method::Get,
                .data = {{
                    {"firstName", "Fred"},
                    {"lastName", "Flintstone"}
                }},
            });
        } catch (const ResponseError& e) {
            EXPECT_STREQ(e.what(),
                "Request data can only be added "
                "for PUT, POST, DELETE, and PATCH."
            );
            throw;
        }
    }, RequestError);
}