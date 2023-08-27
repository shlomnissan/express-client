// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

namespace Express::Http::Validators {
    [[nodiscard]] auto constexpr IsDigit(auto c) noexcept {
        return c >= 0x30 && c <= 0x39;
    }

    [[nodiscard]] auto constexpr IsDigitRange(auto range) noexcept {
        for (auto c : range) if (!IsDigit(c)) return false;
        return true;
    }

    [[nodiscard]] auto constexpr IsAlpha(auto c) noexcept {
        return (c >= 0x61 && c <= 0x7A) || // a - z
               (c >= 0x41 && c <= 0x5A);   // A - Z
    }

    [[nodiscard]] auto constexpr IsAlnum(auto c) noexcept {
        return IsAlpha(c) || IsDigit(c);
    }

    [[nodiscard]] auto constexpr IsToken(auto c) noexcept {
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
               IsAlnum(c);
    }

    [[nodiscard]] auto constexpr IsTokenRange(auto range) noexcept {
        for (auto c : range) if (!IsToken(c)) return false;
        return true;
    }

    [[nodiscard]] auto constexpr IsWhiteSpace(auto c) noexcept {
        return c == 0x20 || c == 0x09;
    }

    [[nodiscard]] auto constexpr IsVisible(auto c) noexcept {
        return c >= 0x21 && c <= 0x7E;
    }

    [[nodiscard]] auto constexpr IsObsoleteText(auto c) noexcept {
        return static_cast<unsigned char>(c) >= 0x80 &&
               static_cast<unsigned char>(c) <= 0xFF;
    }

    [[nodiscard]] auto constexpr IsValidChar(auto c) noexcept {
        return IsVisible(c) || IsWhiteSpace(c) || IsObsoleteText(c);
    }

    [[nodiscard]] auto constexpr IsValidCharRange(auto range) noexcept {
        for (auto c : range) if (!IsValidChar(c)) return false;
        return true;
    }
}