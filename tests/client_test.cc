// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <string>

#include <gtest/gtest.h>

#include <express/client.h>
#include <express/http_defs.h>

using namespace Express;

TEST(client, simple_get) {
    auto response = ExpressClient::request({
        .url = "http://127.0.0.1:5000",
        .method = Http::Method::Get,
    });

    EXPECT_EQ(response.status_code, 200);
    EXPECT_EQ(response.status_text, "OK");
    EXPECT_EQ(response.headers.size(), 5);
    EXPECT_EQ(response.headers.has("Date"), true);
    EXPECT_EQ(response.headers.get("Server"), "Werkzeug/2.2.2 Python/3.10.6");
    EXPECT_EQ(response.headers.get("Content-Type"), "text/html; charset=utf-8");
    EXPECT_EQ(response.headers.get("Content-Length"), "12");
    EXPECT_EQ(response.headers.get("Connection"), "close");

    auto body = std::string {begin(response.body), end(response.body)};
    EXPECT_EQ(body, "Hello World!");
}

TEST(client, simple_post) {
    auto response = ExpressClient::request({
        .url = "http://127.0.0.1:5000",
        .method = Http::Method::Post,
        .body = Http::Body::FormFields {{
            {"firstName", "Fred"},
            {"lastName", "Flintstone"}
        }},
    });

    EXPECT_EQ(response.status_code, 200);
    EXPECT_EQ(response.status_text, "OK");
    EXPECT_EQ(response.headers.size(), 5);
    EXPECT_EQ(response.headers.has("Date"), true);
    EXPECT_EQ(response.headers.get("Server"), "Werkzeug/2.2.2 Python/3.10.6");
    EXPECT_EQ(response.headers.get("Content-Type"), "text/html; charset=utf-8");
    EXPECT_EQ(response.headers.get("Content-Length"), "22");
    EXPECT_EQ(response.headers.get("Connection"), "close");

    auto body = std::string {begin(response.body), end(response.body)};
    EXPECT_EQ(body, "Hello Fred Flintstone!");
}

TEST(client, throws_error_if_url_scheme_isnt_supported) {
    EXPECT_THROW({
        try {
            ExpressClient::request({
                .url = "https://127.0.0.1:5000",
                .method = Http::Method::Get,
            });
        } catch (const ExpressClientError& e) {
            EXPECT_STREQ(
                "Invalid URL scheme. Only http is supported.",
                e.what()
            );
            throw;
        }
    }, ExpressClientError);
}