// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include "client/timeout.h"

#include <chrono>
#include <thread>

#include <gtest/gtest.h>

using namespace std::chrono_literals;

TEST(Timeout, IsSet) {
    Express::Timeout timeout_0 {1ms};
    EXPECT_TRUE(timeout_0.has_timeout());

    std::this_thread::sleep_for(2ms);
    EXPECT_TRUE(timeout_0.has_timeout());

    Express::Timeout timeout_1 {0ms};
    EXPECT_FALSE(timeout_1.has_timeout());
}

TEST(Timeout, Updates) {
    Express::Timeout timeout {40ms};

    std::this_thread::sleep_for(1ms);
    auto x = timeout.Get();

    EXPECT_TRUE(x < 40 && x > 0);

    std::this_thread::sleep_for(1ms);
    auto y = timeout.Get();
    EXPECT_TRUE(y < x && y > 0);
}

TEST(Timeout, Expires) {
    Express::Timeout timeout {40ms};

    std::this_thread::sleep_for(41ms);
    EXPECT_EQ(timeout.Get(), 0);

    std::this_thread::sleep_for(5ms);
    EXPECT_EQ(timeout.Get(), 0);
}