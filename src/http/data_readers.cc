// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include "data_readers.h"

#include <algorithm>
#include <iterator>
#include <stdexcept>

#include "client/error.h"
#include "http/validators.h"
#include "http/defs.h"

namespace Express::Http {
    /*
        ConnectionClose
    */ 
    auto ConnectionClose::Feed(std::string_view data) -> void {
        response_.data.append(data);
        if (response_.data.back() == '\0') {
            response_.data.pop_back();
        }
    }

    /*
        ContentLength
    */
    ContentLength::ContentLength(Response& response) : response_(response) {
        auto length_str = response_.headers.Get("content-length");
        if (!Validators::IsDigitRange(length_str)) {
            Error::Runtime("Data reader error", "Invalid contentlength value");
        }
        content_length_ = std::stoul(length_str);
        response_.data.reserve(content_length_);
    };

    auto ContentLength::Feed(std::string_view data) -> void {
        response_.data.append(data);
        if (response_.data.size() >= content_length_) {
            response_.data.resize(content_length_);
            setDoneReadingData(true);
        }
    }

    /*
        ChunkedTransfer
    */
    auto ChunkedTransfer::Feed(std::string_view data) -> void {
        data_.append(data.cbegin(), data.cend());

        while (true) {
            if (bytes_to_read_ > 0) {
                const auto reading {std::min(bytes_to_read_, data_.size())};
                response_.data.append(data_.begin(), data_.begin() + reading);

                data_.erase(0, reading);
                bytes_to_read_ -= reading;

                if (bytes_to_read_ == 0) {
                    done_reading_chunk_ = true;
                } else if (data_.empty()) {
                    break;
                }
            } else {
                if (done_reading_chunk_) {
                    if (data_.size() < 2) break;
                    if (data_[0] != CRLF[0] || data_[1] != CRLF[1]) {
                        Error::Runtime(
                            "Response error",
                            "Every chunk must end with a delimiter"
                        );
                    }
                    data_.erase(cbegin(data_), cbegin(data_) + 2);
                    done_reading_chunk_ = false;
                }

                auto iter {std::search(
                    cbegin(data_), cend(data_),
                    cbegin(CRLF), cend(CRLF)
                )};

                if (iter == data_.end()) break;

                auto chunk_size = std::string(cbegin(data_), iter);

                try {
                    bytes_to_read_ = std::stoul(chunk_size, nullptr, 16);
                } catch(const std::invalid_argument& _) {
                    Error::Runtime("Response error", "Invalid chunk size");
                }

                data_.erase(cbegin(data_), iter + 2);

                if (bytes_to_read_ == 0) {
                    setDoneReadingData(true);
                    break;
                }
            }
        }
    }
}