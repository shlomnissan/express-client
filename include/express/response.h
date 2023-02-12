// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <stdexcept>
#include <string>
#include <vector>

#include <express/header.h>

namespace Express::Http {
    struct Response {
        int status_code;
        std::string status_text;
        std::vector<uint8_t> body;
        HeaderCollection headers;
    };

    class ResponseParser {
    public:
        ResponseParser() = default;

        auto feed(uint8_t* buffer, std::size_t size) -> void;
        
        [[nodiscard]] auto response() const { return response_; };

    private:
        bool parsing_body_;
        std::vector<uint8_t> data_;
        Response response_;

        auto processHeaders() -> void;
        auto parseStatusLine(const std::string& status_line) -> void;

        template<class Iterator>
        auto tokenzieHeaders(const Iterator begin, const Iterator end) {
            std::string data {begin, end};
            std::vector<std::string> tokens;
            for (std::size_t i = 0, j = 0; i < size(data); ++i) {
                if (data[i] == '\r' && data[i + 1] == '\n') {
                    tokens.emplace_back(data.substr(j, i - j));
                    j = i += 2;
                }
            }
            return tokens;
        }
    };

    struct ResponseError : public std::runtime_error {
    public:
        using std::runtime_error::runtime_error;
    };
}