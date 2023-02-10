// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <gtest/gtest.h>

#include <express/client.h>
#include <express/http_defs.h>

using namespace Express;

TEST(client_test, simple_get) {
    ExpressClient::request({
        .url = "http://localhost:5000",
        .method = Http::Method::Get,
    });
}

TEST(client_test, simple_post) {
    ExpressClient::request({
        .url = "http://localhost:5000",
        .method = Http::Method::Post,
        .body = {"Country=Brasil&City=Belo Horizonte"},
        .headers = {{
            {"Content-Type", "application/x-www-form-urlencoded"}
        }}
    });
}