// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#ifndef EXPRESS_SOCKET_H
#define EXPRESS_SOCKET_H

#include <express_client/endpoint.h>
#include <stdexcept>

namespace EXPRESS {
    class Socket {
    public:
        explicit Socket(const Endpoint& endpoint);

        // Delete copy and move operations
        Socket(const Socket&) = delete;
        auto operator=(const Socket&) -> Socket& = delete;
        Socket(Socket&& rhs) = delete;
        auto operator=(Socket&& rhs) -> Socket& = delete;

        auto connect() const -> void;

        ~Socket();

    private:
        int fd_socket = 0;
        int address_len = 0;
        sockaddr* address = nullptr;
    };

    struct InvalidSocket : public std::runtime_error {
        InvalidSocket()
            : std::runtime_error(
                "Failed to initialize a socket. Check your endpoint and "
                "try again."
            ) {}
    };

    struct UnableToConnect : public std::runtime_error {
        UnableToConnect()
            : std::runtime_error(
                "Failed to connect to endpoint. Make sure "
                "your endpoint's port is not already in use."
            ) {}
    };
}

#endif  // EXPRESS_SOCKET_H