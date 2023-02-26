// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <express/response.h>
#include <express/transformers.h>
#include <express/http_defs.h>

namespace Express::Http {
    using namespace std::string_literals;
    using namespace Transformers;

    auto ResponseParser::response() const -> Response {
        if (knownBodyLength() && !done_reading_data_) {
            throw ResponseError {"Incomplete data transfer."};
        }
        return response_;
    }

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

        // Status code
        auto separator = status.find(0x20);
        auto status_code = status.substr(0, separator);
        if (separator != 3 || !is_digit_range(status_code)) {
            throw ResponseError {"Invalid status code (" + status_code + ")"};
        }
        response_.status = std::stoi(status_code);
        
        // Reason phrase
        auto response_phrase = status.substr(separator + 1);
        if (!is_valid_char_range(response_phrase)) {
            throw ResponseError {"Invalid characters in reason phrase"};
        }
        response_.status_text = status.substr(separator + 1);
    }

    // RFC 7230, 3.2. Header Fields
    auto ResponseParser::parseHeaders(const std::vector<std::string>& tokens) {
        using namespace Transformers;

        for (std::string_view header : tokens) {
            auto separator = header.find(':');
            if (separator == std::string::npos) {
                throw ResponseError {"Failed to process invalid response header"};
            }

            auto name = header.substr(0, separator);
            auto lowercase_name = str_to_lower(name);

            auto value = trim_leading_whitespaces(header.substr(separator + 1));
            auto lowercase_value = str_to_lower(value);

            if (lowercase_name == "content-length") {
                // If a message is received with multiple Content-Length 
                // header fields having differing field-values, then the 
                // message framing is invalid.
                if (response_.headers.has("content-length"))
                    throw ResponseError {"Received multiple content length fields."};
                if (response_.headers.has("transfer-encoding")) {
                    continue;
                }
            }

            if (lowercase_name == "transfer-encoding" && value == "chunked" &&
                response_.headers.has("content-length")) {
                response_.headers.remove("content-length");
            }

            // The tokenizer handles the obsolete fold,
            // and the Header constructor handles the header name/value validation. 
            response_.headers.add({name, value});
        }
    }

    auto ResponseParser::processHeaders() {
        auto iter = std::search(
            begin(data_), end(data_),
            begin(HCRLF), end(HCRLF)
        );
        if (iter == end(data_)) return;

        auto headers_begin = begin(data_);
        auto headers_end = iter + 2;
        auto tokens = tokenzieHeaders(headers_begin, headers_end);

        parseStatusLine(tokens.front());
        parseHeaders({begin(tokens) + 1, end(tokens)});
        data_.erase(headers_begin, headers_end + 2);

        parsing_body_ = true;
    }

    // RFC 7230, 3.3.3. Message Body Length
    auto ResponseParser::setMessageBodyLength() {
        using enum MessageBodyParsingMethod;

        if (response_.headers.has("transfer-encoding")) {
            auto value { response_.headers.get("transfer-encoding") };
            if (value == "chunked") {
                body_parsing_method_ = ChunkedTransfer;
                return;
            }
        }

        if (response_.headers.has("content-length")) {
            auto length = response_.headers.get("content-length");
            if (!is_digit_range(length)) {
                throw ResponseError {"Invalid content length value"};
            }
            content_length_ = std::stoul(length);
            response_.data.reserve(content_length_);
            body_parsing_method_ = ContentLength;
            return;
        }
        
        // In a response message without a declared message body length,
        // the message body length is determined by the number of octets
        // received prior to the server closing the connection.
        body_parsing_method_ = ConnectionClosed;
    }

    // TODO: extract this to a separate object
    auto ResponseParser::processBody() {
        using enum MessageBodyParsingMethod;

        if (done_reading_data_) return;
        if (body_parsing_method_ == Undetermined) { setMessageBodyLength(); }

        if (body_parsing_method_ == ChunkedTransfer) {
            while (true) {
                if (finished_reading_chunk) {
                    if (data_.size() < 2) break;
                    if (data_[0] != CRLF[0] || data_[1] != CRLF[1]) {
                        // Every chunk must end with crlf
                        throw ResponseError {"Invalid chunk delimiter."};
                    }
                    data_.erase(begin(data_), begin(data_) + 2);
                    finished_reading_chunk = false;
                }

                if (bytes_to_read == 0) {
                    auto iter {std::search(
                        begin(data_), end(data_),
                        begin(CRLF), end(CRLF)
                    )};

                    auto chunk_size = std::string(begin(data_), iter);
                    if (chunk_size.empty()) {
                        throw ResponseError {"Invalid chunk size."};
                    }
                    bytes_to_read = std::stoul(chunk_size, nullptr, 16);

                    data_.erase(begin(data_), iter + 2);

                    if (bytes_to_read == 0) {
                        done_reading_data_ = true;
                        break;
                    }
                } else {
                    const auto to_read {std::min(bytes_to_read, data_.size())};
                    response_.data.insert(
                        end(response_.data),
                        begin(data_),
                        begin(data_) + to_read
                    );

                    data_.erase(begin(data_), begin(data_) + to_read);

                    bytes_to_read -= to_read;
                    if (bytes_to_read == 0) {
                        finished_reading_chunk = true;
                    }

                    if (data_.empty()) break;
                }
            }
        }

        if (body_parsing_method_ == ContentLength) {
            response_.data.insert(end(response_.data), begin(data_), end(data_));
            data_.clear();
            if (response_.data.size() >= content_length_) {
                response_.data.resize(content_length_);
                done_reading_data_ = true;
            }
        }

        if (body_parsing_method_ == ConnectionClosed) {
            response_.data.insert(end(response_.data), begin(data_), end(data_));
            data_.clear();
        }
    }

    auto ResponseParser::feed(uint8_t* buffer, std::size_t size) -> void {
        data_.insert(end(data_), buffer, buffer + size);
        if (!parsing_body_) processHeaders();
        if (parsing_body_) processBody();
    }
}