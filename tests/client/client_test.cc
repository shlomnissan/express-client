// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include "express/client.h"

#include <chrono>

#include <gtest/gtest.h>

#include "express/exception.h"

using namespace std::chrono_literals;

struct Client : public ::testing::Test {
    [[no_unique_address]] Express::Client client {};
}; 

TEST_F(Client, ProcessGetRequest) {
    auto response = client.Request({
        .url = "http://127.0.0.1:5000"
    }).get();

    EXPECT_EQ(response.status_code, 200);
    EXPECT_EQ(response.status_text, "OK");
    EXPECT_EQ(response.data, "Hello World!");
}

TEST_F(Client, ProcessPostRequestWithData) {
    auto response = client.Request({
        .url = "http://127.0.0.1:5000",
        .method = Express::Method::Post,
        .headers = {{
            {"Content-Type", "application/x-www-form-urlencoded"}
        }},
        .data = "firstName=Fred&lastName=Flintstone",
    }).get();

    EXPECT_EQ(response.status_code, 200);
    EXPECT_EQ(response.status_text, "OK");
    EXPECT_EQ(response.data, "Hello Fred Flintstone!");
}

TEST_F(Client, ProcessRequestWithUserAuth) {
    auto response = client.Request({
        .url = "http://127.0.0.1:5000/secured",
        .method = Express::Method::Get,
        .auth = {
            .username = "aladdin",
            .password = "opensesame"
        }
    }).get();

    EXPECT_EQ(response.status_code, 200);
    EXPECT_EQ(response.status_text, "OK");
    EXPECT_EQ(response.data, "Hello Aladdin!");
}

TEST_F(Client, ProcessRequestWithUserInformation) {
    auto response = client.Request({
        .url = "http://aladdin:opensesame@127.0.0.1:5000/secured",
        .method = Express::Method::Get,
    }).get();

    EXPECT_EQ(response.status_code, 200);
    EXPECT_EQ(response.status_text, "OK");
    EXPECT_EQ(response.data, "Hello Aladdin!");
}

TEST_F(Client, ProcessRequestwithCustomHeaders) {
    auto response = client.Request({
        .url = "http://127.0.0.1:5000/secured",
        .method = Express::Method::Get,
        .headers = {{
            {"Authorization", "Basic YWxhZGRpbjpvcGVuc2VzYW1l"}
        }}
    }).get();

    EXPECT_EQ(response.status_code, 200);
    EXPECT_EQ(response.status_text, "OK");
    EXPECT_EQ(response.data, "Hello Aladdin!");
}

TEST_F(Client, ThrowsErrorIfRequestTimedOut) {
    EXPECT_THROW({
        try {
            auto response = client.Request({
                .url = "http://127.0.0.1:5000/slow",
                .method = Express::Method::Post,
                .timeout = 5ms
            }).get();
        } catch (Express::ResponseError e) {
            EXPECT_STREQ(
                e.what(),
                "Timeout error: Failed to receive data from the server"
            );
            throw;
        }
        
    }, Express::ResponseError);
}