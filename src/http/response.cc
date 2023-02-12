// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <express/response.h>
#include <express/validators.h>

namespace Express::Http {
    using namespace std::string_literals;

    auto ResponseParser::feed(uint8_t* buffer, std::size_t size) -> void {
        data_.insert(end(data_), buffer, buffer + size);

        if (!parsing_body_) {
            processHeaders();
        }

        if (parsing_body_) {
            // TODO: processBody();
        }
    }

    auto ResponseParser::processHeaders() -> void {
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
        // parse headers

        data_.erase(headers_begin, headers_end + 2);

        parsing_body_ = true;
    }

    // RFC7230, 3.1.2. Status Line
    auto ResponseParser::parseStatusLine(const std::string& status_line) -> void {
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
}