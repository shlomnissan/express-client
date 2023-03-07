// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

namespace Express::Http::Validators {
    [[nodiscard]] auto constexpr is_digit(auto c) noexcept {
        return c >= 0x30 && c <= 0x39;
    }

    [[nodiscard]] auto constexpr is_digit_range(auto range) noexcept {
        for (auto c : range) if (!is_digit(c)) return false;
        return true;
    }

    [[nodiscard]] auto constexpr is_alpha(auto c) noexcept {
        return (c >= 0x61 && c <= 0x7A) || // a - z
               (c >= 0x41 && c <= 0x5A);   // A - Z
    }

    [[nodiscard]] auto constexpr is_alnum(auto c) noexcept {
        return is_alpha(c) || is_digit(c);
    }

    [[nodiscard]] auto constexpr is_token(auto c) noexcept {
        return c == 0x21 || // =!
               c == 0x23 || // #
               c == 0x24 || // $
               c == 0x25 || // %
               c == 0x26 || // &
               c == 0x27 || // '
               c == 0x2A || // *
               c == 0x2B || // +
               c == 0x2D || // -
               c == 0x2E || // .
               c == 0x5E || // ^
               c == 0x5F || // _
               c == 0x60 || // `
               c == 0x7C || // |
               c == 0x7E || // ~
               is_alnum(c);
    }

    [[nodiscard]] auto constexpr is_token_range(auto range) noexcept {
        for (auto c : range) if (!is_token(c)) return false;
        return true;
    }

    [[nodiscard]] auto constexpr is_whitespace(auto c) noexcept {
        return c == 0x20 || c == 0x09;
    }

    [[nodiscard]] auto constexpr is_visible(auto c) noexcept {
        return c >= 0x21 && c <= 0x7E;
    }

    [[nodiscard]] auto constexpr is_obsolete_text(auto c) noexcept {
        return static_cast<unsigned char>(c) >= 0x80 &&
               static_cast<unsigned char>(c) <= 0xFF;
    }

    [[nodiscard]] auto constexpr is_valid_char(auto c) noexcept {
        return is_visible(c) || is_whitespace(c) || is_obsolete_text(c);
    }

    [[nodiscard]] auto constexpr is_valid_char_range(auto range) noexcept {
        for (auto c : range) if (!is_valid_char(c)) return false;
        return true;
    }
}