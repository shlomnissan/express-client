// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

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

TEST(request, creates_valid_request_object_with_form_data) {
    Express::Net::URL url {"http://example.com"};
    Request request {url, {
        .url = url.source(),
        .method = Method::Post,
        .body = Body::FormFields {{
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

TEST(request, throws_error_if_url_scheme_isnt_supported) {
    Express::Net::URL url {"ftp://example.com"};
    EXPECT_THROW({
        try {
            Request request(url, {
                .url = url.source(),
                .method = Method::Get
            });
        } catch (const RequestError& e) {
            EXPECT_STREQ(
                "Invalid URL scheme. Only http is supported.",
                e.what()
            );
            throw;
        }
    }, RequestError);
}