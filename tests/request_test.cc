// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)


#include <gtest/gtest.h>
#include <express/request.h>

#include <express/exception.h>

using namespace Express::Http;

TEST(request, creates_valid_request_object) {
    Express::Net::URL url {"http://example.com"};
    Request request {url, {
        .url = url.source(),
        .method = Method::Get
    }};

    EXPECT_EQ(
        request.str(),
        "GET / HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "User-Agent: express/0.1\r\n"
        "Connection: close\r\n"
        "\r\n"
    );
}

TEST(request, creates_valid_request_object_with_raw_string) {
    Express::Net::URL url {"http://example.com"};
    Request request {url, {
        .url = url.source(),
        .method = Method::Post,
        .data = {"firstName=Fred&lastName=Flintstone"},
        .headers = {{
            {"Content-Type", "application/x-www-form-urlencoded"}
        }}
    }};

    EXPECT_EQ(
        request.str(),
        "POST / HTTP/1.1\r\n"
        "Content-Type: application/x-www-form-urlencoded\r\n"
        "Host: example.com\r\n"
        "User-Agent: express/0.1\r\n"
        "Content-Length: 34\r\n"
        "Connection: close\r\n"
        "\r\n"
        "firstName=Fred&lastName=Flintstone"
    );
}

TEST(request, creates_valid_request_object_with_form_data) {
    Express::Net::URL url {"http://example.com"};
    Request request {url, {
        .url = url.source(),
        .method = Method::Post,
        .data = {{
            {"firstName", "Fred"},
            {"lastName", "Flintstone"}
        }}
    }};

    EXPECT_EQ(
        request.str(),
        "POST / HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "User-Agent: express/0.1\r\n"
        "Content-Type: application/x-www-form-urlencoded\r\n"
        "Content-Length: 34\r\n"
        "Connection: close\r\n"
        "\r\n"
        "firstName=Fred&lastName=Flintstone"
    );
}

TEST(request, request_object_content_type_overrides_hint) {
    Express::Net::URL url {"http://example.com"};
    Request request {url, {
        .url = url.source(),
        .method = Method::Post,
        .data = {{
            {"firstName", "Fred"},
            {"lastName", "Flintstone"}
        }},
        .headers = {{
            {"Content-Type", "text/html"}
        }}
    }};

    EXPECT_EQ(
        request.str(),
        "POST / HTTP/1.1\r\n"
        "Content-Type: text/html\r\n"
        "Host: example.com\r\n"
        "User-Agent: express/0.1\r\n"
        "Content-Length: 34\r\n"
        "Connection: close\r\n"
        "\r\n"
        "firstName=Fred&lastName=Flintstone"
    );
}

TEST(request, passes_authorization_header_correctly) {
    Express::Net::URL url {"http://example.com"};
    Request request {url, {
        .url = url.source(),
        .method = Method::Get,
        .headers = {{
            {"Authorization", "Basic b3BlbjpzZXNhbWU="}
        }}
    }};

    EXPECT_EQ(
        request.str(),
        "GET / HTTP/1.1\r\n"
        "Authorization: Basic b3BlbjpzZXNhbWU=\r\n"
        "Host: example.com\r\n"
        "User-Agent: express/0.1\r\n"
        "Connection: close\r\n"
        "\r\n"
    );
}

TEST(request, creates_valid_request_with_auth_through_url) {
    Express::Net::URL url {"http://aladdin:opensesame@example.com"};
    Request request {url, {
        .url = url.source(),
        .method = Method::Get,
    }};

    EXPECT_EQ(
        request.str(),
        "GET / HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "User-Agent: express/0.1\r\n"
        "Authorization: Basic YWxhZGRpbjpvcGVuc2VzYW1l\r\n"
        "Connection: close\r\n"
        "\r\n"
    );
}

TEST(request, creates_valid_request_with_auth_through_config) {
    Express::Net::URL url {"http://example.com"};
    Request request {url, {
        .url = url.source(),
        .method = Method::Get,
        .auth = {
            .username = "aladdin",
            .password = "opensesame"
        }
    }};

    EXPECT_EQ(
        request.str(),
        "GET / HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "User-Agent: express/0.1\r\n"
        "Authorization: Basic YWxhZGRpbjpvcGVuc2VzYW1l\r\n"
        "Connection: close\r\n"
        "\r\n"
    );
}

TEST(request, config_auth_overrides_url_auth) {
    Express::Net::URL url {"http://fred:flintstone@example.com"};
    Request request {url, {
        .url = url.source(),
        .method = Method::Get,
        .auth = {
            .username = "aladdin",
            .password = "opensesame"
        }
    }};

    EXPECT_EQ(
        request.str(),
        "GET / HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "User-Agent: express/0.1\r\n"
        "Authorization: Basic YWxhZGRpbjpvcGVuc2VzYW1l\r\n"
        "Connection: close\r\n"
        "\r\n"
    );
}

TEST(request, config_auth_overrides_custom_header) {
    Express::Net::URL url {"http://example.com"};
    Request request {url, {
        .url = url.source(),
        .method = Method::Get,
        .headers = {{
            {"Authorization", "Basic ZnJlZDpmbGludHN0b25l"}
        }},
        .auth = {
            .username = "aladdin",
            .password = "opensesame"
        }
    }};

    EXPECT_EQ(
        request.str(),
        "GET / HTTP/1.1\r\n"
        "User-Agent: express/0.1\r\n"
        "Host: example.com\r\n"
        "Authorization: Basic YWxhZGRpbjpvcGVuc2VzYW1l\r\n"
        "Connection: close\r\n"
        "\r\n"
    );
}

TEST(request, url_auth_overrides_custom_header) {
    Express::Net::URL url {"http://fred:flintstone@example.com"};
    Request request {url, {
        .url = url.source(),
        .method = Method::Get,
        .headers = {{
            {"Authorization", "Basic YWxhZGRpbjpvcGVuc2VzYW1l"}
        }}
    }};

    EXPECT_EQ(
        request.str(),
        "GET / HTTP/1.1\r\n"
        "User-Agent: express/0.1\r\n"
        "Host: example.com\r\n"
        "Authorization: Basic ZnJlZDpmbGludHN0b25l\r\n"
        "Connection: close\r\n"
        "\r\n"
    );
}

TEST(request, throws_when_content_type_not_set_for_raw_string) {
    Express::Net::URL url {"http://example.com"};

    EXPECT_THROW({
        try {
            Request request(url, {
                .url = url.source(),
                .method = Method::Post,
                .data = {"firstName=Fred&lastName=Flintstone"},
            });
        } catch (const Express::RequestError& e) {
            EXPECT_STREQ(e.what(),
                "Request error: Data was provided for this request, "
                "but the Content-Type header isn't set."
            );
            throw;
        }
    }, Express::RequestError);
}

TEST(request, throws_if_http_method_should_not_include_data) {
    Express::Net::URL url {"http://example.com"};

    EXPECT_THROW({
        try {
            Request request(url, {
                .url = url.source(),
                .method = Method::Get,
                .data = {{
                    {"firstName", "Fred"},
                    {"lastName", "Flintstone"}
                }},
            });
        } catch (const Express::RequestError& e) {
            EXPECT_STREQ(e.what(),
                "Request error: Request data can only be added "
                "for PUT, POST, DELETE, and PATCH."
            );
            throw;
        }
    }, Express::RequestError);
}