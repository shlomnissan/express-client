// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

namespace Express::Http::Validators {
    template <typename TChar>
    [[nodiscard]] auto constexpr is_digit(TChar c) noexcept {
        return c >= 0x30 && c <= 0x39;
    }

    template <typename TChar>
    [[nodiscard]] auto constexpr is_alpha(TChar c) noexcept {
        return (c >= 0x61 && c <= 0x7A) || // a - z
               (c >= 0x41 && c <= 0x5A);   // A - Z
    }

    template <typename TChar>
    [[nodiscard]] auto constexpr is_alnum(TChar c) noexcept {
        return is_alpha(c) || is_digit(c);
    }

    template <typename TChar>
    [[nodiscard]] auto constexpr is_token(TChar c) noexcept {
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

    template <typename TChar>
    [[nodiscard]] auto constexpr is_whitespace(TChar c) noexcept {
        return c == 0x20 || c == 0x09;
    }

    template <typename TChar>
    [[nodiscard]] auto constexpr is_visible(TChar c) noexcept {
        return c >= 0x21 && c <= 0x7E;
    }

    template <typename TChar>
    [[nodiscard]] auto constexpr is_obsolete_text(TChar c) noexcept {
        return static_cast<unsigned char>(c) >= 0x80 &&
               static_cast<unsigned char>(c) <= 0xFF;
    }
}