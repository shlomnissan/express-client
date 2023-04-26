// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <chrono>
#include <thread>

#include <gtest/gtest.h>
#include <express/timeout.h>

using namespace Express;
using namespace std::chrono_literals;

class TimeoutTest : public ::testing::Test {
//protected:
    // void SetUp() override { }
    // void void TearDown() override { }
};

TEST_F(TimeoutTest, basic_test) {
    Timeout timeout {5ms};

    std::this_thread::sleep_for(1ms);
    auto x = timeout.get();
    EXPECT_TRUE(x < 5 && x > 0);

    std::this_thread::sleep_for(1ms);
    auto y = timeout.get();
    EXPECT_TRUE(y < x && y > 0);
}

TEST_F(TimeoutTest, test_timeout_expired) {
    Timeout timeout {5ms};

    std::this_thread::sleep_for(1ms);
    auto x = timeout.get();
    EXPECT_TRUE(x < 5 && x > 0);

    std::this_thread::sleep_for(5ms);
    EXPECT_EQ(timeout.get(), 0);

    std::this_thread::sleep_for(5ms);
    EXPECT_EQ(timeout.get(), 0);
}

TEST_F(TimeoutTest, test_has_timeout) {
    Timeout timeout_0 {5ms};
    EXPECT_TRUE(timeout_0.hasTimeout());

    std::this_thread::sleep_for(6ms);
    EXPECT_EQ(timeout_0.get(), 0);
    EXPECT_TRUE(timeout_0.hasTimeout());

    Timeout timeout_1 {0ms};
    EXPECT_FALSE(timeout_1.hasTimeout());

    std::this_thread::sleep_for(6ms);
    EXPECT_EQ(timeout_1.get(), 0);
    EXPECT_FALSE(timeout_1.hasTimeout());
}
