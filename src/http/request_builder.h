// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <sstream>

#include "express/config.h"
#include "net/url.h"

namespace Express::Http {
    class RequestBuilder {
    public:
        explicit RequestBuilder(const Config& config);

        [[nodiscard]] auto GetData() const { return stream_.str(); } 

    private:
        Config config_;
        Net::Url url_;
        std::stringstream stream_;

        auto SetHeaders() -> void;
        auto WriteToStream() -> void;
        auto IsDataAllowed() const -> bool;
    };
}