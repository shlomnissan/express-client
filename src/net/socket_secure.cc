// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <express/socket_secure.h>

#include <openssl/x509.h>

namespace Express::Net {
    constexpr auto kOpenSSLMinVersion = 0x1000200fL; // v1.0.2

    struct X509_deleter {
        auto operator()(X509* cert) const {
            X509_free(cert);
        }
    };
    using X509_ptr = std::unique_ptr<X509, X509_deleter>;

    SocketSecure::SocketSecure(Endpoint endpoint)
      : Socket(std::move(endpoint)) {
        // Check version
        if (OpenSSL_version_num() < kOpenSSLMinVersion) {
            throw SocketSecureError {
                "The minimum required version of OpenSSL is 1.0.2"
            };
        }

        // Initialize OpenSSL
        SSL_library_init();
        SSL_load_error_strings();

        // Create a new OpenSSL context
        auto context = SSL_CTX_new(TLS_client_method());
        if (context == nullptr) {
            throw SocketSecureError {
                "Failed to initialize OpenSSL context"
            };
        }
        ctx_.reset(context);

        // Load trusted CAs
        if (!SSL_CTX_load_verify_locations(context, "ca-bundle.crt", nullptr)) {
            throw SocketSecureError {
                "Failed to load trusted certificate authorities"
            };
        }
    }

    void SocketSecure::connect(std::string_view host) {
        Socket::connect(host);

        // Create a new SSL object and bind it to the socket
        ssl_.reset(SSL_new(ctx_.get()));
        SSL_set_fd(ssl_.get(), static_cast<int>(fd_socket_));

        // Enable SNI
        if (!SSL_set_tlsext_host_name(ssl_.get(), host.data())) {
            throw SocketSecureError {
                "Servername Indication request failed"
            };
        }

        // Enable hostname verification
        if (!X509_VERIFY_PARAM_set1_host(SSL_get0_param(ssl_.get()), host.data(), 0)) {
            throw SocketSecureError {
                "Failed to enable hostname verification"
            };
        }

        // Perform SSL handshake
        if (SSL_connect(ssl_.get()) <= 0) {
            throw SocketSecureError {
                "Failed to connect SSL socket"
            };
        }

        // Verify certificate
        X509_ptr cert {SSL_get_peer_certificate(ssl_.get())};
        if (cert) {
            const auto res = SSL_get_verify_result(ssl_.get());
            if (res != X509_V_OK) {
                throw SocketSecureError {
                    "Failed to verify SSL certificate"
                };
            }
        } else {
            throw SocketSecureError {
                "Failed to obtain an SSL certificate from the server"
            };
        }
    }

    ssize_t SocketSecure::send(std::string_view buffer, const Timeout& timeout) const {
        wait(EventType::ToWrite, timeout);
        return SSL_write(ssl_.get(), buffer.data(), static_cast<int>(buffer.size()));
    }

    ssize_t SocketSecure::recv(uint8_t *buffer, const Timeout& timeout) const {
        wait(EventType::ToRead, timeout);
        return SSL_read(ssl_.get(), reinterpret_cast<char*>(buffer), BUFSIZ);
    }
}