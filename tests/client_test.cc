// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <chrono>
#include <string>

#include <gtest/gtest.h>

#include <express/client.h>
#include <express/method.h>
#include <express/socket.h>
#include <express/exception.h>

using namespace std::chrono_literals;

TEST(client, simple_get) {
    auto response = Express::Client::request({
        .url = "http://127.0.0.1:5000",
        .method = Express::Method::Get,
    }).get();

    EXPECT_EQ(response.statusCode, 200);
    EXPECT_EQ(response.statusText, "OK");
    EXPECT_EQ(response.headers.size(), 5);
    EXPECT_EQ(response.headers.has("Date"), true);
    EXPECT_EQ(response.headers.has("Server"), true);
    EXPECT_EQ(response.headers.get("Content-Type"), "text/html; charset=utf-8");
    EXPECT_EQ(response.headers.get("Content-Length"), "12");
    EXPECT_EQ(response.headers.get("Connection"), "close");

    auto body = std::string {cbegin(response.data), cend(response.data)};
    EXPECT_EQ(body, "Hello World!");
}

TEST(client, simple_post_with_form_data) {
    auto response = Express::Client::request({
        .url = "http://127.0.0.1:5000",
        .method = Express::Method::Post,
        .data = {{
            {"firstName", "Fred"},
            {"lastName", "Flintstone"}
        }},
    }).get();

    EXPECT_EQ(response.statusCode, 200);
    EXPECT_EQ(response.statusText, "OK");
    EXPECT_EQ(response.headers.size(), 5);
    EXPECT_EQ(response.headers.has("Date"), true);
    EXPECT_EQ(response.headers.has("Server"), true);
    EXPECT_EQ(response.headers.get("Content-Type"), "text/html; charset=utf-8");
    EXPECT_EQ(response.headers.get("Content-Length"), "22");
    EXPECT_EQ(response.headers.get("Connection"), "close");

    auto body = std::string {cbegin(response.data), cend(response.data)};
    EXPECT_EQ(body, "Hello Fred Flintstone!");
}

TEST(client, post_with_raw_string) {
    auto response = Express::Client::request({
        .url = "http://127.0.0.1:5000",
        .method = Express::Method::Post,
        .data = {"firstName=Fred&lastName=Flintstone"},
        .headers = {{
            {"Content-Type", "application/x-www-form-urlencoded"}
        }}
    }).get();

    EXPECT_EQ(response.statusCode, 200);
    EXPECT_EQ(response.statusText, "OK");
    EXPECT_EQ(response.headers.size(), 5);
    EXPECT_EQ(response.headers.has("Date"), true);
    EXPECT_EQ(response.headers.has("Server"), true);
    EXPECT_EQ(response.headers.get("Content-Type"), "text/html; charset=utf-8");
    EXPECT_EQ(response.headers.get("Content-Length"), "22");
    EXPECT_EQ(response.headers.get("Connection"), "close");

    auto body = std::string {cbegin(response.data), cend(response.data)};
    EXPECT_EQ(body, "Hello Fred Flintstone!");
}

TEST(client, get_with_basic_authorization_config) {
    auto response = Express::Client::request({
        .url = "http://127.0.0.1:5000/auth",
        .method = Express::Method::Get,
        .auth = {
            .username = "aladdin",
            .password = "opensesame"
        }
    }).get();

    EXPECT_EQ(response.statusCode, 200);
    EXPECT_EQ(response.statusText, "OK");

    auto body = std::string {cbegin(response.data), cend(response.data)};
    EXPECT_EQ(body, "Hello Aladdin!");
}

TEST(client, get_with_basic_authorization_url) {
    auto response = Express::Client::request({
        .url = "http://aladdin:opensesame@127.0.0.1:5000/auth",
        .method = Express::Method::Get
    }).get();

    EXPECT_EQ(response.statusCode, 200);
    EXPECT_EQ(response.statusText, "OK");

    auto body = std::string {cbegin(response.data), cend(response.data)};
    EXPECT_EQ(body, "Hello Aladdin!");
}

TEST(client, get_with_basic_authorization_custom_header) {
    auto response = Express::Client::request({
        .url = "http://127.0.0.1:5000/auth",
        .method = Express::Method::Get,
        .headers = {{
            {"Authorization", "Basic YWxhZGRpbjpvcGVuc2VzYW1l"}
        }}
    }).get();

    EXPECT_EQ(response.statusCode, 200);
    EXPECT_EQ(response.statusText, "OK");

    auto body = std::string {cbegin(response.data), cend(response.data)};
    EXPECT_EQ(body, "Hello Aladdin!");
}

TEST(client, get_with_basic_authorization_config_failure) {
    auto response = Express::Client::request({
        .url = "http://127.0.0.1:5000/auth",
        .method = Express::Method::Get,
    }).get();

    EXPECT_EQ(response.statusCode, 401);
    EXPECT_EQ(response.statusText, "UNAUTHORIZED");
}

TEST(client, throws_if_request_timed_out) {
    EXPECT_THROW({
        try {
            auto result = Express::Client::request({
                .url = "http://127.0.0.1:5000/slow",
                .method = Express::Method::Get,
                .timeout = 5ms,
            }).get();
        } catch (const Express::ResponseError& e) {
            EXPECT_STREQ(
                "Timeout error: Failed to receive data from the server",
                e.what()
            );
            throw;
        }
    }, Express::ResponseError);
}