// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <sstream>

#include <gtest/gtest.h>
#include <express/request.h>

using namespace Express::Http;

TEST(request, creates_valid_request_object) {
    Express::Net::URL url {"http://example.com"};
    Request request {{Method::Get}, url};

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

TEST(request, throws_error_if_url_scheme_isnt_supported) {
    Express::Net::URL url {"ftp://example.com"};
    EXPECT_THROW({
        try {
            Request request({Method::Get}, url);
        } catch (const RequestError& e) {
            EXPECT_STREQ(
                "Invalid URL scheme. Only http is supported.",
                e.what()
            );
            throw;
        }
    }, RequestError);
}