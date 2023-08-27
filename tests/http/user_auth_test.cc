// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include "express/user_auth.h"

#include <gtest/gtest.h>

TEST(UserAuth, InitializesAsAggregateType) {
    Express::UserAuth auth {"aladdin", "opensesame"};

    EXPECT_EQ(auth.username, "aladdin");
    EXPECT_EQ(auth.password, "opensesame");
}

TEST(UserAuth, InitializesWithDesignatedInitializers) {
    Express::UserAuth auth_0 {
        .username = "aladdin",
        .password = "opensesame",
    };

    EXPECT_EQ(auth_0.username, "aladdin");
    EXPECT_EQ(auth_0.password, "opensesame");

    Express::UserAuth auth_1 { .username = "aladdin" }; 

    EXPECT_EQ(auth_1.username, "aladdin");
    EXPECT_TRUE(auth_1.password.empty());

    Express::UserAuth auth_2 { .password = "opensesame" };

    EXPECT_TRUE(auth_2.username.empty());
    EXPECT_EQ(auth_2.password, "opensesame");
}

TEST(UserAuth, EmptyWhenUsernameAndPasswordAreEmpty) {
    Express::UserAuth auth_0;

    EXPECT_TRUE(auth_0.Empty());

    Express::UserAuth auth_1 { .username = "aladdin" };

    EXPECT_FALSE(auth_1.Empty());
}

TEST(UserAuth, NotEmptyWhenUsernameOrPasswordAreSet) {
    Express::UserAuth auth_0 { .username = "aladdin" };
    Express::UserAuth auth_1 { .password = "opensesame" };
    Express::UserAuth auth_2 {
        .username = "aladdin",
        .password = "opensesame"
    };

    EXPECT_FALSE(auth_0.Empty());
    EXPECT_FALSE(auth_1.Empty());
    EXPECT_FALSE(auth_2.Empty());
}