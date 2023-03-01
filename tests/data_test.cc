// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <gtest/gtest.h>

#include <express/data.h>

using namespace Express::Http;

TEST(data, initializes_data_with_raw_string) {
    Data requestData {"firstName=Fred&lastName=Flintstone"};

    EXPECT_EQ(requestData.data(), "firstName=Fred&lastName=Flintstone");
}

TEST(data, initalizes_data_with_form_fields) {
    Data requestData {{
        {"firstName", "Fred"},
        {"lastName", "Flintstone"},
    }};

    EXPECT_EQ(requestData.data(), "firstName=Fred&lastName=Flintstone");
    EXPECT_EQ(requestData.contentType(), "application/x-www-form-urlencoded");
}

TEST(data, throws_if_content_type_is_not_set_for_raw_string) {
    Data requestData {"firstName=Fred&lastName=Flintstone"};

    EXPECT_THROW({
        try {
            auto contentType = requestData.contentType();
        } catch (const DataError& e) {
            EXPECT_STREQ(e.what(),
                "Data was provided for this request, "
                "but the ContentType header isn't set."
            );
            throw;
        }
    }, DataError);
}