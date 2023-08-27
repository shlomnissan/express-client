// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include "http/defs.h"

#include <sstream>

#include <gtest/gtest.h>

TEST(HttpDefs, PrintsCrlf) {
    std::stringstream ss;
    ss << Express::Http::crlf;

    EXPECT_EQ(ss.str().size(), 2);
    EXPECT_EQ(ss.str()[0], 0xD);
    EXPECT_EQ(ss.str()[1], 0xA);
}

TEST(HttpDefs, PrintsHcrlf) {
    std::stringstream ss;
    ss << Express::Http::hcrlf;

    EXPECT_EQ(ss.str().size(), 4);
    EXPECT_EQ(ss.str()[0], 0xD);
    EXPECT_EQ(ss.str()[1], 0xA);
    EXPECT_EQ(ss.str()[2], 0xD);
    EXPECT_EQ(ss.str()[3], 0xA);
}