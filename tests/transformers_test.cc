// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)


#include <gtest/gtest.h>
#include <express/transformers.h>

using namespace Express::Transformers;

TEST(base64_encoding, basic_test) {
    EXPECT_EQ(base64_encode("a"), "YQ==");
    EXPECT_EQ(base64_encode("ab"), "YWI=");
    EXPECT_EQ(base64_encode("abc"), "YWJj");
    EXPECT_EQ(base64_encode("abcd"), "YWJjZA==");
    EXPECT_EQ(base64_encode("open:sesame"), "b3BlbjpzZXNhbWU=");
    EXPECT_EQ(base64_encode("aladdin:opensesame"), "YWxhZGRpbjpvcGVuc2VzYW1l");
}