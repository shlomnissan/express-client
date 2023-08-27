// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <memory>

#include "express/response.h"
#include "http/data_readers.h"

namespace Express::Http {
    using std::vector;
    using std::string;
    using std::string_view;

    using namespace std::string_literals;

    class ResponseParser {
    public:
        auto Feed(unsigned char* buffer, std::size_t size) -> void;

        [[nodiscard]] auto response() const -> Express::Response;
        [[nodiscard]] auto done_reading_data() const -> bool;

    private:
        bool done_reading_data_ {false};
        bool parsing_body_ {false};
        bool known_body_length_ {false};

        std::string data_;
        std::unique_ptr<DataReader> data_reader_;

        Response response_;

        auto ReadHeaders() -> void;
        auto ProcessHeaders(const std::vector<std::string>& headers) -> void;
        auto ReadBody() -> void;
        auto DataReaderFactory() -> std::unique_ptr<DataReader>;

        [[nodiscard]] auto Split(string_view headers) const -> vector<string>;
        [[nodiscard]] auto IsObsoleteLineFolding(std::string_view str) const -> bool;
        [[nodiscard]] auto IsDelimiter(std::string_view str) const -> bool;
    };
}