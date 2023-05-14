// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <sstream>
#include <chrono>

#include <gtest/gtest.h>
#include <fff.h>

#include <express/socket_secure.h>
#include <express/endpoint.h>

#if defined(_WIN32)
#include <express/winsock.h>
#endif

using namespace Express;
using namespace Express::Net;
using namespace std::chrono_literals;

DEFINE_FFF_GLOBALS;

FAKE_VALUE_FUNC(unsigned long, OpenSSL_version_num);

class SocketSecureTest : public ::testing::Test {
#if defined(_WIN32)
    WinSock winsock;
#endif
    void SetUp() override {
        OpenSSL_version_num_fake.return_val = 0x1000200fL;
    }
};

TEST_F(SocketSecureTest, basic_test) {
    SocketSecure socket_sec {{"example.com", "443"}};
    socket_sec.connect();
    EXPECT_TRUE(socket_sec.get() > 0);

    std::stringstream ss;
    ss << "GET / HTTP/1.1\r\n";
    ss << "Host: example.com\r\n";
    ss << "Connection: close\r\n";
    ss << "User-Agent: express\r\n";
    ss << "\r\n";

    Express::Timeout timeout {0s};

    socket_sec.sendAll(ss.str(), timeout);

    uint8_t buffer[BUFSIZ];
    socket_sec.recv(buffer, timeout);
    std::string response {
        reinterpret_cast<const char*>(buffer)
    };

    EXPECT_TRUE(response.starts_with("HTTP/1.1 200 OK"));    
}

TEST_F(SocketSecureTest, throws_error_bad_openssl_version) {
    OpenSSL_version_num_fake.return_val = 0x1000000fL;

    EXPECT_THROW({
        try {
            SocketSecure socket_sec({"example.com", "443"});
        } catch (const SocketSecureError& e) {
            EXPECT_STREQ(
                "The minimum required version of OpenSSL is 1.0.2.",
                e.what()
            );
            throw e;
        }
    }, SocketSecureError);
}

// TODO: failed to init context
// TODO: failed to load trusted certificate authorities
// TODO: failed to connect
// TODO: failed to perform SNI
// TODO: failed to enable hostname verificiation
// TODO: failed to connect
// TODO: failed to verify certificate
// TODO: failed to obtain SSL certificate
// TODO: failed to send
// TODO: failed to receive

TEST_F(SocketSecureTest, throws_error_bad_ssl_certificate) {
    SocketSecure socket_sec {{"expired.badssl.com", "443"}};

    EXPECT_THROW({
        try {
            socket_sec.connect();
        } catch (const SocketSecureError& e) {
            EXPECT_STREQ(
                "Failed to verify SSL certificate.",
                e.what()
            );
            throw e;
        }
    }, SocketSecureError);
}