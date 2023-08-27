// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include "http/status_line.h"

#include <gtest/gtest.h>

#include "express/exception.h"

TEST(StatusLine, InitializesWithString) {
    Express::Http::StatusLine status {"HTTP/1.1 200 OK"};
    
    EXPECT_EQ(status.code(), 200);
    EXPECT_EQ(status.text(), "OK");
}

TEST(StatusLine, ThrowsErrorIfMalformed) {
    EXPECT_THROW({
        try {
            Express::Http::StatusLine status {"200 OK"};
        } catch(Express::ResponseError e) {
            EXPECT_STREQ(
                e.what(),
                "Status line error: Malformed status line"
            );
            throw;
        }
    }, Express::ResponseError);
}

TEST(StatusLine, ThrowsErrorIfUnsupportedHttpVersion) {
    EXPECT_THROW({
        try {
            Express::Http::StatusLine status {"HTTP/2.0 200 OK"};
        } catch(Express::ResponseError e) {
            EXPECT_STREQ(
                e.what(),
                "Status line error: Unsupported HTML version (2.0)"
            );
            throw;
        }
    }, Express::ResponseError);
}

TEST(StatusLine, ThrowsErrorIfInvalidStatusCode) {
    EXPECT_THROW({
        try {
            Express::Http::StatusLine status {"HTTP/1.0 32 OK"};
        } catch(Express::ResponseError e) {
            EXPECT_STREQ(
                e.what(),
                "Status line error: Invalid status code (32)"
            );
            throw;
        }
    }, Express::ResponseError);
}

TEST(StatusLine, ThrowsErrorIfInvalidCharactersInReason) {
    EXPECT_THROW({
        try {
            Express::Http::StatusLine status {"HTTP/1.1 200 \rOK"};
        } catch(Express::ResponseError e) {
            EXPECT_STREQ(
                e.what(),
                "Status line error: Invalid characters in reason phrase"
            );
            throw;
        }
    }, Express::ResponseError);
}
