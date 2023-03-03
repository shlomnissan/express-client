// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <express/socket.h>

#include <sys/socket.h>

namespace Express::Net {
    Socket::Socket(Endpoint endpoint) : endpoint_(std::move(endpoint)) {
        socket_fd = socket(
            endpoint_.family(),
            endpoint_.socketType(),
            endpoint_.protocol()
        );

        if (!socket_fd) {
            throw SocketError {"Failed to initialize socket."};
        }
    }

    auto Socket::connect() const -> void {
        if (::connect(socket_fd, endpoint_.address(), endpoint_.addressLength())) {
            throw SocketError {"Failed to connect."};
        }
    }

    auto Socket::send(std::string_view buffer) const -> long {
        return ::send(socket_fd, buffer.data(), buffer.size(), 0);
    }

    auto Socket::recv(uint8_t* buffer) const -> long {
        return ::recv(socket_fd, buffer, BUFSIZ, 0);
    }

    Socket::~Socket() {
        if (socket_fd) {
            close(socket_fd);
        }
    }
}
