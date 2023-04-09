// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <gtest/gtest.h>

#include <express/socket.h>
#include <express/endpoint.h>

#if defined(_WIN32)
#include <express/winsock.h>
#endif

using namespace Express::Net;

class SocketTest : public ::testing::Test {
protected:
#if defined(_WIN32)
    WinSock winsock;
#endif
};

TEST_F(SocketTest, basic_test) {
    Socket socket {{"example.com", "80"}};

    EXPECT_TRUE(socket.get() > 0);
}

TEST_F(SocketTest, test_socket_move_constructor) {
    Socket socket {{"example.com", "80"}};
    EXPECT_NE(socket.get(), INVALID_SOCKET);

    auto fd_socket = socket.get();
    auto another_socket = std::move(socket);
    EXPECT_EQ(another_socket.get(), fd_socket);
    EXPECT_EQ(socket.get(), INVALID_SOCKET);
}

TEST_F(SocketTest, test_socket_move_assignment) {
    Socket socket {{"example.com", "80"}};
    EXPECT_NE(socket.get(), INVALID_SOCKET);

    Socket another_socket {{"example.com", "80"}};
    EXPECT_NE(another_socket.get(), INVALID_SOCKET);

    auto fd_socket = socket.get();
    another_socket = std::move(socket);
    EXPECT_EQ(another_socket.get(), fd_socket);
    EXPECT_EQ(socket.get(), INVALID_SOCKET);
}

// create stubs for C library functions
// TODO: failed to connect
// TODO: failed to initialize
// TODO: timeout