// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <chrono>

namespace Express {
    using namespace std::chrono_literals;
    using std::chrono::milliseconds;
    using std::chrono::steady_clock;

    class Timeout {
    public:
        explicit Timeout(milliseconds timeout) {
            if (timeout > kNoTimeout) {
                expiry_timestamp_ = steady_clock::now() + timeout;
                has_timeout_ = true;
            }
        }

        [[nodiscard]] auto has_timeout() const -> bool { return has_timeout_; }

        [[nodiscard]] auto Get() const -> std::int64_t {
            if (!has_timeout_) { return 0; }

            const auto curr_time = steady_clock::now();
            const auto delta = std::chrono::duration_cast<milliseconds>(
                expiry_timestamp_ - curr_time
            );

            return delta.count() > 0 ? delta.count() : 0;
        }

    private:
        static constexpr auto kNoTimeout = 0s;

        bool has_timeout_ = false;
        std::chrono::steady_clock::time_point expiry_timestamp_;
    };
}