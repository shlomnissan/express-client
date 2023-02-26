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

    EXPECT_EQ(response.status, 200);
    EXPECT_EQ(response.status_text, "OK");
    EXPECT_EQ(response.headers.size(), 5);
    EXPECT_EQ(response.headers.has("Date"), true);
    EXPECT_EQ(response.headers.has("Server"), true);
    EXPECT_EQ(response.headers.get("Content-Type"), "text/html; charset=utf-8");
    EXPECT_EQ(response.headers.get("Content-Length"), "12");
    EXPECT_EQ(response.headers.get("Connection"), "close");

    auto body = std::string {begin(response.data), end(response.data)};
    EXPECT_EQ(body, "Hello World!");
}

TEST(client, simple_post) {
    auto response = ExpressClient::request({
        .url = "http://127.0.0.1:5000",
        .method = Http::Method::Post,
        .data = {{
            {"firstName", "Fred"},
            {"lastName", "Flintstone"}
        }},
    });

    EXPECT_EQ(response.status, 200);
    EXPECT_EQ(response.status_text, "OK");
    EXPECT_EQ(response.headers.size(), 5);
    EXPECT_EQ(response.headers.has("Date"), true);
    EXPECT_EQ(response.headers.has("Server"), true);
    EXPECT_EQ(response.headers.get("Content-Type"), "text/html; charset=utf-8");
    EXPECT_EQ(response.headers.get("Content-Length"), "22");
    EXPECT_EQ(response.headers.get("Connection"), "close");

    auto body = std::string {begin(response.data), end(response.data)};
    EXPECT_EQ(body, "Hello Fred Flintstone!");
}