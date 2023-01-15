// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <express_client/socket.h>

#include <sys/socket.h>
#include <unistd.h>

namespace EXPRESS {
    Socket::Socket(const Endpoint& endpoint)
        : address_len(endpoint.getAddressLength()),
          address(endpoint.getAddress()) {
        fd_socket = socket(
            endpoint.getFamily(),
            endpoint.getSocketType(),
            endpoint.getProtocol()
        );

        if (!fd_socket) {
            throw InvalidSocket();
        }
    }

    auto Socket::connect() const -> void {
        if (::connect(fd_socket, address, address_len)) {
            throw UnableToConnect();
        }
    }

    Socket::~Socket() {
        if (fd_socket) {
            close(fd_socket);
        }
    }
}