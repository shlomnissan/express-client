// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include "express/method.h"

#include <array>
#include <sstream>

#include <gtest/gtest.h>

TEST(Method, ConvertsToString) {
    EXPECT_EQ(Express::MethodToString(Express::Method::Delete), "DELETE");
    EXPECT_EQ(Express::MethodToString(Express::Method::Get), "GET");
    EXPECT_EQ(Express::MethodToString(Express::Method::Head), "HEAD");
    EXPECT_EQ(Express::MethodToString(Express::Method::Options), "OPTIONS");
    EXPECT_EQ(Express::MethodToString(Express::Method::Patch), "PATCH");
    EXPECT_EQ(Express::MethodToString(Express::Method::Post), "POST");
    EXPECT_EQ(Express::MethodToString(Express::Method::Put), "PUT");
}

TEST(Method, SendToStream) {
    std::array<Express::Method, 7> methods {
        Express::Method::Delete,
        Express::Method::Get,
        Express::Method::Head,
        Express::Method::Options,
        Express::Method::Patch,
        Express::Method::Post,
        Express::Method::Put
    };

    for (const auto& method : methods) {
        std::stringstream ss;
        ss << method;
        EXPECT_EQ(ss.str(), Express::MethodToString(method));
    }
}