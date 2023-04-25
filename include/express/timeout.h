// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <chrono>

namespace Express {
    using namespace std::chrono_literals;

    class Timeout {
    public:
        explicit Timeout(std::chrono::milliseconds timeout) {
            if (timeout > kNoTimeout) {
                stop_time_ = std::chrono::steady_clock::now() + timeout;
                has_timeout_ = true;
            }
        }

        [[nodiscard]] auto hasTimeout() const -> bool { return has_timeout_; }

        [[nodiscard]] auto get() const -> std::int64_t {
            if (!has_timeout_) { return 0; }
            const auto curr_time = std::chrono::steady_clock::now();
            const auto diff =
                std::chrono::duration_cast<std::chrono::milliseconds>(
                    stop_time_ - curr_time
                );
            return diff.count() >= 0 ? diff.count() : 0;
        }

    private:
        static constexpr auto kNoTimeout = 0s;

        std::chrono::steady_clock::time_point stop_time_;
        bool has_timeout_ = false;
    };
}