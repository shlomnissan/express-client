// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <express/response.h>
#include <express/transformers.h>

namespace Express::Http {
    using namespace std::string_literals;
    using namespace Transformers;

    // RFC7230, 3.1.2. Status Line
    auto ResponseParser::parseStatusLine(const std::string& status_line) {
        using namespace Validators;
        auto status = status_line;
        if (!status.starts_with("HTTP/"s)) {
            throw ResponseError {"Malformed status line"};
        }

        // HTML version
        status = status.substr(status.find('/') + 1);
        auto version = status.substr(0, status.find(0x20));
        if (version != "1.0" && version != "1.1") {
            throw ResponseError {"Unsupported HTML version (" + version + ")"};
        }
        status = status.substr(4);

        // status code
        auto separator = status.find(0x20);
        auto status_code = status.substr(0, separator);
        if (separator != 3 || !is_digit_range(status_code)) {
            throw ResponseError {"Invalid status code (" + status_code + ")"};
        }
        response_.status_code = std::stoi(status_code);
        
        // reason phrase
        auto response_phrase = status.substr(separator + 1);
        if (!is_valid_char_range(response_phrase)) {
            throw ResponseError {"Invalid characters in reason phrase"};
        }
        response_.status_text = status.substr(separator + 1);
    }

    // RFC 7230, 3.2. Header Fields
    auto ResponseParser::parseHeaders(const std::vector<std::string>& tokens) {
        for (std::string_view header : tokens) {
            auto separator = header.find(':');
            if (separator == std::string::npos) {
                throw ResponseError {"Failed to process invalid response header"};
            }
            // The tokenizer handles the obsolete fold,
            // and the Header constructor handles the header name/value validation. 
            response_.headers.add({
                header.substr(0, separator),
                header.substr(separator + 1)
            });
        }
    }

    auto ResponseParser::processHeaders() {
        const std::array<uint8_t, 4> header_separator = {0xD, 0xA, 0xD, 0xA};
        auto iter = std::search(
            begin(data_), end(data_),
            begin(header_separator), end(header_separator)
        );
        if (iter == end(data_)) return;

        auto headers_begin = begin(data_);
        auto headers_end = iter + 2;
        auto tokens = tokenzieHeaders(headers_begin, headers_end);

        parseStatusLine(tokens.front());
        parseHeaders({begin(tokens) + 1, end(tokens)});
        data_.erase(headers_begin, headers_end + 2);

        if (response_.headers.has("transfer-encoding")) {
            auto value = response_.headers.get("transfer-encoding");
            if (value == "chunked") {
                has_chunked_response_ = true;
            } else {
                throw ResponseError {"Unsupported transfer encoding (" + value + ")"};
            }
        } else if (response_.headers.has("content-length")) {
            auto length = response_.headers.get("content-length");
            if (!is_digit_range(length)) {
                throw ResponseError {"Invalid content length value"};
            }
            has_content_length_ = true;
            response_.body.reserve(std::stoul(length));
        }

        parsing_body_ = true;
    }

    auto ResponseParser::processBody() {}

    auto ResponseParser::feed(uint8_t* buffer, std::size_t size) -> void {
        data_.insert(end(data_), buffer, buffer + size);
        !parsing_body_ ? processHeaders() : processBody();
    }
}