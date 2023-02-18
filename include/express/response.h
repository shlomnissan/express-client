// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <stdexcept>
#include <string>
#include <vector>

#include <express/header.h>
#include <express/validators.h>

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
        std::vector<uint8_t> data_;
        Response response_;
        bool parsing_body_;

        auto parseStatusLine(const std::string& status_line);
        auto parseHeaders(const std::vector<std::string>& tokens);
        auto processHeadersSection();

        auto isDelimiter(const auto begin, const auto end) {
            if (begin == end || begin + 1 == end) return false;
            return *begin == '\r' && *(begin + 1) == '\n';
        } 

        auto isObsoleteLineFolding(const auto begin, const auto end) {
            using namespace Validators;
            if (begin == end || begin + 1 == end || begin + 2 == end) return false;
            return *begin == '\r' && *(begin + 1) == '\n' && is_whitespace(*(begin + 2));
        }

        auto tokenzieHeaders(const auto begin, const auto end) {
            std::vector<std::string> tokens;
            std::string token;
            auto iter = begin;
            while (iter != end) {
                if (isObsoleteLineFolding(iter, end)) {
                    // Obsolete Line Folding RFC 7230, 3.2.4. Field Parsing
                    // A user agent that receives an obs-fold in a response
                    // message that is not within a message/http container MUST
                    // replace each received obs-fold with one or more SP octets
                    // prior to interpreting the field value.
                    token += ' ';
                    iter += 3;
                } else if (isDelimiter(iter, end)) {
                    tokens.emplace_back(std::move(token));
                    iter += 2;
                } else {
                    token += *iter++;
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