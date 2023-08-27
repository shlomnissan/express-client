// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <string_view>

#include "response.h"

namespace Express::Http {
    class DataReader {
    public:
        virtual auto Feed(std::string_view data) -> void = 0;

        auto done_reading_data() const { return done_reading_data_; }

        virtual ~DataReader() = default;

    protected:
        auto setDoneReadingData(bool is_done) { done_reading_data_ = is_done; }

    private:
        bool done_reading_data_ {false};
    };

    /*
        Read data until the connection is closed.
    */
    class ConnectionClose : public DataReader {
    public:
        explicit ConnectionClose(Response& response) : response_(response) {};

        auto Feed(std::string_view data) -> void override;

    private:
        Response& response_;
    };

    /*
        Read "content-length" bytes.
    */
    class ContentLength : public DataReader {
    public:
        explicit ContentLength(Response& response);

        auto Feed(std::string_view data) -> void override;

    private: 
        size_t content_length_ {0};

        Response& response_;
    };

    /*
        Read data in chunks.
    */
    class ChunkedTransfer : public DataReader {
    public:
        explicit ChunkedTransfer(Response& response) : response_(response) {};

        auto Feed(std::string_view data) -> void override;

    private:
        bool done_reading_chunk_ {false};
        size_t bytes_to_read_ {0};

        std::string data_; 

        Response& response_;
    };
}