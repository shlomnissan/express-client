// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <sstream>

#include <express/response.h>
#include <express/validators.h>

namespace Express::Http {
    using namespace std::string_literals;

    auto Response::feed(uint8_t* buffer, std::size_t size) -> void {
        data_.insert(end(data_), buffer, buffer + size);

        if (!parsing_body_) {
            processHeaders();
        }

        if (parsing_body_) {
            // TODO: processBody();
        }
    }

    auto Response::processHeaders() -> void {
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

        data_.erase(headers_begin, headers_end + 2);

        parsing_body_ = true;
    }

    auto Response::parseStatusLine(const std::string& status) -> void {
        auto token = "HTTP/"s;
        if (!status.starts_with(token)) {
            // TODO: raise an error bad status line
        }

        // TODO: generalize tokenize string
        std::stringstream data_stream {status.substr(token.size())};
        std::vector<std::string> tokens;
        while (getline(data_stream, token, ' ')) {
            tokens.emplace_back(token);
        }
        if (size(tokens) != 3) {
            // TODO: incomplete status line
        }

        if (tokens[0] != "1.0" || tokens[0] != "1.1") {
            // TODO: raise an error, invalid version
        }

        response_.status_code = stoi(tokens[1]);
        if (tokens[1].size() != 3 || response_.status_code < 100 || response_.status_code > 599) {
            // TODO: raise an error, invalid status code
        }

        response_.status_text = tokens[2];
        for (auto c : response_.status_text) {
            if (!Validators::is_valid_char(c)) {
                // TODO: invalid message
            }
        }
    }
}