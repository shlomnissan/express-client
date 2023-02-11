// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <gtest/gtest.h>

#include <express/body.h>

using namespace Express::Http::Body;

TEST(body_test, initializes_form_fields) {
    FormFields fields {{
        {"firstName", "Fred"},
        {"lastName", "Flinstone"}
    }};

    EXPECT_EQ(fields.data(), "firstName=Fred&lastName=Flinstone");
}