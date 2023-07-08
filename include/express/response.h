// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <stdexcept>
#include <string>
#include <vector>

#include <express/header.h>
#include <express/validators.h>

namespace Express::Http {
    using namespace Validators;

    enum class MessageBodyParsingMethod {
        Undetermined,
        ChunkedTransfer,
        ContentLength,
        ConnectionClosed
    };

    struct Response {
        int statusCode;
        std::string statusText;
        std::vector<uint8_t> data;
        HeaderCollection headers;
    };

    class ResponseParser {
    public:
        ResponseParser() = default;

        auto feed(uint8_t* buffer, std::size_t size) -> void;
        
        [[nodiscard]] auto response() const -> Response;
        [[nodiscard]] auto doneReadingData() const { return done_reading_data_; }

    private:
        std::vector<uint8_t> data_ {};
        Response response_ {};
        MessageBodyParsingMethod body_parsing_method_ {MessageBodyParsingMethod::Undetermined};
        size_t content_length_ {0};
        size_t bytes_to_read {0};
        bool parsing_body_ {false};
        bool done_reading_data_ {false};
        bool finished_reading_chunk {false};

        auto parseStatusLine(const std::string& status_line);
        auto parseHeaders(const std::vector<std::string>& tokens);
        auto processHeaders();
        auto setMessageBodyLength();
        auto processBody();

        auto knownBodyLength() const -> bool {
            return body_parsing_method_ == MessageBodyParsingMethod::ChunkedTransfer ||
                body_parsing_method_ == MessageBodyParsingMethod::ContentLength;
        }

        auto isDelimiter(const auto begin, const auto end) {
            if (begin == end || begin + 1 == end) return false;
            return *begin == '\r' && *(begin + 1) == '\n';
        } 

        auto isObsoleteLineFolding(const auto begin, const auto end) {
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
}