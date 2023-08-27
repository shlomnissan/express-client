// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include "response_parser.h"

#include "client/error.h"
#include "http/data_readers.h"
#include "http/defs.h"
#include "http/status_line.h"
#include "http/validators.h"
#include "utils/string_transformers.h"

namespace Express::Http {
    auto ResponseParser::response() const -> Express::Response {
        if (known_body_length_ && !done_reading_data_) {
            Error::Runtime(
                "Response error",
                "Incomplete data transfer"
            );
        }
        return response_;
    }

    auto ResponseParser::done_reading_data() const -> bool {
        return done_reading_data_;
    }

    auto ResponseParser::Feed(unsigned char* buffer, std::size_t size) -> void {
        data_.append(buffer, buffer + size);
        if (!parsing_body_) ReadHeaders();
        if (parsing_body_) ReadBody();
    }

    auto ResponseParser::ReadHeaders() -> void {
        auto idx = data_.find({cbegin(HCRLF), cend(HCRLF)});
        if (idx == std::string::npos) {
            return;
        }
        auto headers_data = data_.substr(0, idx + 2);
        auto headers = Split(headers_data);

        StatusLine status {headers.front()};
        response_.status_code = status.code();
        response_.status_text = status.text();

        ProcessHeaders({headers.begin() + 1, headers.end()});
        data_.erase(begin(data_), begin(data_) + idx + 4);

        parsing_body_ = true;
    }

    auto ResponseParser::Split(string_view headers) const -> vector<string> {
        std::vector<std::string> output;
        std::string line;
        auto iter = headers.begin();
        while (iter != headers.end()) {
            if (IsObsoleteLineFolding({iter, 3})) {
                // A user agent that receives an obs-fold in a response
                // message that is not within a message/http container MUST
                // replace each received obs-fold with one or more SP octets
                // prior to interpreting the field value.
                line += ' ';
                iter += 3;
            } else if (IsDelimiter({iter, 2})) {
                output.emplace_back(std::move(line));
                line.clear();
                iter += 2;
            } else {
                line += *iter++;
            }
        }
        return output;
    }

    auto ResponseParser::IsObsoleteLineFolding(std::string_view str) const -> bool {
        if (str.size() < 3) return false;
        return Validators::IsWhiteSpace(str[2]) &&
               str[1] == 0xA &&
               str[0] == 0xD;
    }

    auto ResponseParser::IsDelimiter(std::string_view str) const -> bool {
        if (str.size() < 2) return false;
        return str[0] == 0xD && str[1] == 0xA;
    }

    auto ResponseParser::ProcessHeaders(const std::vector<std::string>& headers) -> void {
        using namespace StringTransformers;

        for (const std::string& header : headers) {
            auto separator = header.find(":");
            if (separator == std::string::npos) {
                Error::Runtime(
                    "Response error",
                    "Failed to process invalid response header"
                );
            }

            auto name = header.substr(0, separator);
            auto value = TrimLeadingWhiteSpaces(header.substr(separator + 1));

            auto lowercase_name = StringToLowerCase(name);
            if (lowercase_name == "content-length") {
                if (response_.headers.Contains("content-length") &&
                    response_.headers.Get("content-length") != value
                ) {
                    Error::Runtime(
                        "Response error",
                        "Received multiple content length fields "
                        "with different values"
                    );
                }
                if (response_.headers.Contains("transfer-encoding")) {
                    continue;
                }
            }

            if (lowercase_name == "transfer-encoding" && value == "chunked" &&
                response_.headers.Contains("content-length")) {
                response_.headers.Remove("content-length");
            }

            response_.headers.Add(name, value);
        }
    }

    auto ResponseParser::ReadBody() -> void {
        if (data_reader_ == nullptr) data_reader_ = DataReaderFactory();
        if (data_reader_->done_reading_data()) return;

        data_reader_->Feed(data_);
        data_.clear();

        done_reading_data_ = data_reader_->done_reading_data();
    }

    auto ResponseParser::DataReaderFactory() -> std::unique_ptr<DataReader> {
        if (response_.headers.Contains("transfer-encoding")) {
            auto value { response_.headers.Get("transfer-encoding") };
            if (value == "chunked") {
                known_body_length_ = true;
                return std::make_unique<ChunkedTransfer>(response_);
            } else {
                Error::Runtime(
                    "Response error",
                    "Unsupport transfer encoding (" + value + ")"
                );
            }
        }

        if (response_.headers.Contains("content-length")) {
            known_body_length_ = true;
            return std::make_unique<ContentLength>(response_);
        }

        return std::make_unique<ConnectionClose>(response_);
    }
}