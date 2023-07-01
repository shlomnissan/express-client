// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <express/socket.h>

#include <openssl/ssl.h>

namespace Express::Net {
    struct SSL_CTX_deleter {
        auto operator()(SSL_CTX* ctx) const {
            SSL_CTX_free(ctx);
        }
    };

    struct SSL_deleter {
        auto operator()(SSL* ssl) const {
            SSL_shutdown(ssl);
            SSL_free(ssl);
        }
    };

    using SSL_CTX_ptr = std::unique_ptr<SSL_CTX, SSL_CTX_deleter>;
    using SSL_ptr = std::unique_ptr<SSL, SSL_deleter>;

    class SocketSecure : public Socket {
    public:
        explicit SocketSecure(Endpoint endpoint);

        void connect() override;
        size_t send(std::string_view buffer, const Timeout& timeout) const override;
        size_t recv(uint8_t* buffer, const size_t size, const Timeout& timeout) const override;

    private:
        SSL_ptr ssl_ {nullptr};
        SSL_CTX_ptr ctx_ {nullptr};
    };

    struct SocketSecureError : public std::runtime_error {
        using runtime_error::runtime_error;
    };
}