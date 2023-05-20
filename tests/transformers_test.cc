// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <string>

#include <gtest/gtest.h>
#include <express/transformers.h>

using namespace Express::Transformers;

TEST(str_to_lower, basic_test) {
  EXPECT_EQ(str_to_lower("Hello World!"), "hello world!");
}

TEST(trim_leading_whitespaces, basic_test) {
    EXPECT_EQ(
        trim_leading_whitespaces("  Trims leading spaces"),
        "Trims leading spaces"
    );

    EXPECT_EQ(
        trim_leading_whitespaces("   Ignores trailing spaces  "),
        "Ignores trailing spaces  "
    );

    std::string str {"  Doesn't modify the argument"};
    EXPECT_EQ(str, "  Doesn't modify the argument");
}

TEST(trim_leading_whitespaces_in_place, basic_test) {
    std::string str {"  Trims leading spaces"};
    trim_leading_whitespaces_in_place(str);
    EXPECT_EQ(str, "Trims leading spaces");

    str = "   Ignores trailing spaces  ";
    trim_leading_whitespaces_in_place(str);
    EXPECT_EQ(str, "Ignores trailing spaces  ");
}

TEST(trim_trailing_whitespaces_in_place, basic_test) {
    std::string str {"Trims trailing spaces    "};
    trim_trailing_whitespaces_in_place(str);
    EXPECT_EQ(str, "Trims trailing spaces");

    str = "   Ignores leading spaces  ";
    trim_trailing_whitespaces_in_place(str);
    EXPECT_EQ(str, "   Ignores leading spaces");
}

TEST(base64_encoding, basic_test) {
    EXPECT_EQ(base64_encode("a"), "YQ==");
    EXPECT_EQ(base64_encode("ab"), "YWI=");
    EXPECT_EQ(base64_encode("abc"), "YWJj");
    EXPECT_EQ(base64_encode("abcd"), "YWJjZA==");
    EXPECT_EQ(base64_encode("open:sesame"), "b3BlbjpzZXNhbWU=");
    EXPECT_EQ(base64_encode("aladdin:opensesame"), "YWxhZGRpbjpvcGVuc2VzYW1l");
}