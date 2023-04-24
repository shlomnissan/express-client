// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#if !defined(_WIN32)

#include <cerrno>
#include <unistd.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

#define SYS_EINTR EINTR
#define CLOSE(s) close(s)
#define ERRNO() (errno)

using SOCKET = int;

constexpr auto INVALID_SOCKET = -1;

#else

#include <winsock2.h>
#include <windef.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define SYS_EINTR WSAEINTR
#define CLOSE(s) closesocket(s)
#define ERRNO() (WSAGetLastError())

using ssize_t = SSIZE_T;

#endif