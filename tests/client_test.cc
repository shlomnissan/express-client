// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <gtest/gtest.h>

#include <express/client.h>

TEST(client_test, get) {
    Express::Client client;
    client.get("http://localhost:5000");
}

TEST(client_test, post) {
    Express::Client client;
    client.post("http://localhost:5000", {
        "Country=Brasil&City=Belo Horizonte"
    });
}