// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <express/socket_secure.h>

#include <filesystem>
#include <express/error.h>
#include <openssl/ssl.h>
#include <openssl/x509.h>
#include <openssl/err.h>

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
                "The minimum required version of OpenSSL is 1.0.2."
            };
        }

        // Initialize OpenSSL
        SSL_library_init();
        SSL_load_error_strings();

        // Create a new OpenSSL context
        auto context = SSL_CTX_new(TLS_client_method());
        if (context == nullptr) {
            throw SocketSecureError {
                "Failed to initialize OpenSSL context."
            };
        }
        ctx_.reset(context);

        if (!std::filesystem::exists("ca-bundle.crt")) {
            Error::logic("OpenSSL error", "The ca-bundle.crt file is missing. "
                "You can download it from https://curl.se/docs/caextract.html, "
                "rename it to ca-bundle.crt, and place it in the executable directory. "
                "Alternatively, you can provide your own certificate authority store, or disable "
                "SSL altogether during compilation by using CMake's -DBUILD_SSL=OFF option."
            );
        }

        if (!SSL_CTX_load_verify_locations(context, "ca-bundle.crt", nullptr)) {
            throw SocketSecureError {
                "Failed to load trusted certificate authorities."
            };
        }
    }

    auto SocketSecure::handleOpenSSLError(int rval, const Timeout& timeout) const -> void {
        auto error = SSL_get_error(ssl_.get(), rval);
        if (error == SSL_ERROR_WANT_READ || error == SSL_ERROR_WANT_WRITE) {
            auto event_type = error == SSL_ERROR_WANT_READ
                ? EventType::ToRead
                : EventType::ToWrite;
            auto select_result = select(event_type, timeout);
            if (select_result == 0) {
                Error::runtime("OpenSSL error", "Request timed out");
            }
        } else {
            // TODO: replace with error message
            throw SocketSecureError {"Failed to connect SSL socket."};
        }
    }

    void SocketSecure::connect(const Timeout& timeout) {
        Socket::connect(timeout);

        // Create a new SSL object and bind it to the socket
        ssl_.reset(SSL_new(ctx_.get()));
        SSL_set_fd(ssl_.get(), sock_);

        // Enable SNI
        if (!SSL_set_tlsext_host_name(ssl_.get(), ep_.host().c_str())) {
            throw SocketSecureError {
                "Servername Indication request failed."
            };
        }

        // Enable hostname verification
        if (!X509_VERIFY_PARAM_set1_host(SSL_get0_param(ssl_.get()), ep_.host().c_str(), 0)) {
            throw SocketSecureError {
                "Failed to enable hostname verification."
            };
        }

        // Perform SSL handshake
        auto res = 0;
        while (res <= 0) {
            res = SSL_connect(ssl_.get());
            if (res <= 0) {
                handleOpenSSLError(res, timeout);
            }
        }

        // Verify certificate
        X509_ptr cert {SSL_get_peer_certificate(ssl_.get())};
        if (cert) {
            if (SSL_get_verify_result(ssl_.get()) != X509_V_OK) {
                throw SocketSecureError {
                    "Failed to verify SSL certificate."
                };
            }
        } else {
            throw SocketSecureError {
                "Failed to obtain an SSL certificate from the server."
            };
        }
    }

    size_t SocketSecure::send(std::string_view buffer, const Timeout& timeout) const {
        auto data_ptr = buffer.data();
        auto bytes_left = buffer.size();

        while (bytes_left > 0) {
            auto bytes_written = SSL_write(
                ssl_.get(),
                data_ptr,
                static_cast<int>(bytes_left)
            );

            if (bytes_written < 0) {
                handleOpenSSLError(bytes_written, timeout);
            }

            bytes_left -= bytes_written;
            data_ptr += bytes_written;
        }
        return buffer.size();
    }

    size_t SocketSecure::recv(uint8_t* buffer, const size_t size, const Timeout& timeout) const {
        auto bytes_read = -1;
        while (bytes_read < 0) {
            bytes_read = SSL_read(ssl_.get(), buffer, static_cast<int>(size));
            if (bytes_read < 0) {
                handleOpenSSLError(bytes_read, timeout);
            }
        }

        return bytes_read;
    }
}