// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include "utils/string_transformers.h"

#include <gtest/gtest.h>

using namespace Express::StringTransformers;

TEST(StringTransformers, StringToLowerCase) {
    EXPECT_EQ(StringToLowerCase("Hello World"), "hello world");
    EXPECT_EQ(StringToLowerCase("H!32Lo W0rl? "), "h!32lo w0rl? ");
    EXPECT_EQ(StringToLowerCase("  "), "  ");
    EXPECT_EQ(StringToLowerCase(""), "");
}

TEST(StringTransformers, TrimLeadingWhiteSpacesInPlace) {
    std::string input;

    input = "  Trims leading spaces";
    TrimLeadingWhiteSpacesInPlace(input);
    EXPECT_EQ(input, "Trims leading spaces");

    input = "   Ignores trailing spaces  ";
    TrimLeadingWhiteSpacesInPlace(input);
    EXPECT_EQ(input, "Ignores trailing spaces  ");

    input = "  ";
    TrimLeadingWhiteSpacesInPlace(input);
    EXPECT_EQ(input, "");

    input = "";
    TrimLeadingWhiteSpacesInPlace(input);
    EXPECT_EQ(input, "");
}

TEST(StringTransformers, TrimLeadingWhiteSpaces) {
    std::string input;

    EXPECT_EQ(
        TrimLeadingWhiteSpaces("  Trims leading spaces"),
        "Trims leading spaces"
    );

    EXPECT_EQ(
        TrimLeadingWhiteSpaces("   Ignores trailing spaces  "),
        "Ignores trailing spaces  "
    );

    EXPECT_EQ(TrimLeadingWhiteSpaces("  "), "");
    EXPECT_EQ(TrimLeadingWhiteSpaces(""), "");
}

TEST(StringTransformers, TrimTrailingWhiteSpacesInPlace) {
    std::string input;

    input = "Trims trailing spaces    ";
    TrimTrailingWhiteSpacesInPlace(input);
    EXPECT_EQ(input, "Trims trailing spaces");

    input = "   Ignores leading spaces  ";
    TrimTrailingWhiteSpacesInPlace(input);
    EXPECT_EQ(input, "   Ignores leading spaces");

    input = "  ";
    TrimTrailingWhiteSpacesInPlace(input);
    EXPECT_EQ(input, "");

    input = "";
    TrimTrailingWhiteSpacesInPlace(input);
    EXPECT_EQ(input, "");
}

TEST(StringTransformers, Base64Encoding) {
    EXPECT_EQ(Base64Encoding("a"), "YQ==");
    EXPECT_EQ(Base64Encoding("ab"), "YWI=");
    EXPECT_EQ(Base64Encoding("abc"), "YWJj");
    EXPECT_EQ(Base64Encoding("abcd"), "YWJjZA==");
    EXPECT_EQ(Base64Encoding("open:sesame"), "b3BlbjpzZXNhbWU=");
    EXPECT_EQ(Base64Encoding("aladdin:opensesame"), "YWxhZGRpbjpvcGVuc2VzYW1l");
}