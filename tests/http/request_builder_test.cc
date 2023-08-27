// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include "http/request_builder.h"

#include <gtest/gtest.h>

#include "express/exception.h"
#include "express/method.h"

TEST(RequestBuilder, CreatesRequest) {
    Express::Http::RequestBuilder request {{
        .url = "http://example.com"
    }};

    EXPECT_EQ(request.GetData(),
        "GET / HTTP/1.1\r\n"
        "Connection: close\r\n"
        "Host: example.com\r\n"
        "User-Agent: express/0.1\r\n"
        "\r\n"
    );
}

TEST(RequestBuilder, CreateRequestWithHeaders) {
    Express::Http::RequestBuilder request {{
        .url = "http://example.com",
        .headers = {{
            {"Authorization", "xxxxx"}
        }}
    }};

    EXPECT_EQ(request.GetData(),
        "GET / HTTP/1.1\r\n"
        "Authorization: xxxxx\r\n"
        "Connection: close\r\n"
        "Host: example.com\r\n"
        "User-Agent: express/0.1\r\n"
        "\r\n"
    );
}

TEST(RequestBuilder, CreatesRequestWithData) {
    Express::Http::RequestBuilder request {{
        .url = "http://example.com",
        .method = Express::Method::Post,
        .data = "firstName=Fred&lastName=Flintstone",
    }};

    EXPECT_EQ(request.GetData(),
        "POST / HTTP/1.1\r\n"
        "Connection: close\r\n"
        "Content-Length: 34\r\n"
        "Host: example.com\r\n"
        "User-Agent: express/0.1\r\n"
        "\r\n"
        "firstName=Fred&lastName=Flintstone"
    );
}

TEST(RequestBuilder, ThrowsErrorIfDataIsUnsupportedByMethod) {
    EXPECT_THROW({
        try {
            Express::Http::RequestBuilder request({
                .url = "http://example.com",
                .method = Express::Method::Get,
                .data = "firstName=Fred&lastName=Flintstone" 
            });
        } catch (Express::RequestError e) {
            EXPECT_STREQ(e.what(),
                "Request error: "
                "Data can only be added for "
                "PUT, POST, DELETE, and PATCH requests"
            );
            throw;
        }
    }, Express::RequestError);
}

TEST(RequestBuilderAuth, CreatesRequestWithConfigAuth) {
    Express::Http::RequestBuilder request({
        .url = "http://example.com",
        .auth = {
            .username = "aladdin",
            .password = "opensesame"
        }
    });

    EXPECT_EQ(request.GetData(),
        "GET / HTTP/1.1\r\n"
        "Authorization: Basic YWxhZGRpbjpvcGVuc2VzYW1l\r\n"
        "Connection: close\r\n"
        "Host: example.com\r\n"
        "User-Agent: express/0.1\r\n"
        "\r\n"
    );
}

TEST(RequestBuilderAuth, CreatesRequestWithUrlAuth) {
    Express::Http::RequestBuilder request({
        .url = "http://aladdin:opensesame@example.com",
    });

    EXPECT_EQ(request.GetData(),
        "GET / HTTP/1.1\r\n"
        "Authorization: Basic YWxhZGRpbjpvcGVuc2VzYW1l\r\n"
        "Connection: close\r\n"
        "Host: example.com\r\n"
        "User-Agent: express/0.1\r\n"
        "\r\n"
    );
}

TEST(RequestBuilderAuth, ConfigAuthOverridesUrlAuth) {
    Express::Http::RequestBuilder request({
        .url = "http://fred:flintstone@example.com",
        .auth = {
            .username = "aladdin",
            .password = "opensesame"
        }
    });

    EXPECT_EQ(request.GetData(),
        "GET / HTTP/1.1\r\n"
        "Authorization: Basic YWxhZGRpbjpvcGVuc2VzYW1l\r\n"
        "Connection: close\r\n"
        "Host: example.com\r\n"
        "User-Agent: express/0.1\r\n"
        "\r\n"
    );
}

TEST(RequestBuilderAuth, UrlAuthOverridesCustomAuthHeader) {
    Express::Http::RequestBuilder request({
        .url = "http://aladdin:opensesame@example.com",
        .headers = {{
            {"Authorization", "xxxx"}
        }}
    });

    EXPECT_EQ(request.GetData(),
        "GET / HTTP/1.1\r\n"
        "Authorization: Basic YWxhZGRpbjpvcGVuc2VzYW1l\r\n"
        "Connection: close\r\n"
        "Host: example.com\r\n"
        "User-Agent: express/0.1\r\n"
        "\r\n"
    );
}