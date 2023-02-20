// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <gtest/gtest.h>

#include <express/client.h>
#include <express/http_defs.h>
#include <iostream>

using namespace Express;

TEST(client, simple_get) {
    auto reponse = ExpressClient::request({
        .url = "http://127.0.0.1:5000",
        .method = Http::Method::Get,
    });
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
}

TEST(client, throws_error_if_url_scheme_isnt_supported) {
    EXPECT_THROW({
        try {
            ExpressClient::request({
                .url = "https://localhost:5000",
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