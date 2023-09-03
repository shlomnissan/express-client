// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <winsock2.h>
#include <windef.h>
#include <ws2tcpip.h>

#include "client/error.h"

#pragma comment(lib, "ws2_32.lib")

namespace Express::Net {
    class WinSock {
    public:
        WinSock() {
            WSADATA wsa_data;
            const auto error = WSAStartup(MAKEWORD(2, 2), &wsa_data);

            if (error != 0) {
                Error::System("WSAStartup failed");
            }

            if (LOBYTE(wsa_data.wVersion) != 2 || HIBYTE(wsa_data.wVersion) != 2) {
                WSACleanup();
                Error::System("Invalid WinSock version");
            } 

            started = true;
        }

        WinSock(const WinSock& rhs) = delete;
        WinSock(WinSock&& rhs) = delete;

        WinSock& operator=(const WinSock&) = delete;
        WinSock& operator=(WinSock&&) = delete;

        ~WinSock() {
            if (started) WSACleanup();
        }

    private:
        bool started = false;
    };
}