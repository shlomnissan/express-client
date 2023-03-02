// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <cstdint>
#include <express/socket.h>

#include <sys/socket.h>
#include <unistd.h>

namespace Express::Net {
    Socket::Socket(const Endpoint& endpoint)
        : address_len_(endpoint.addressLength()),
          address_(endpoint.address()) {
        fd_socket_ = socket(
            endpoint.family(),
            endpoint.socketType(),
            endpoint.protocol()
        );

        if (!fd_socket_) {
            throw InvalidSocket();
        }
    }

    auto Socket::connect() const -> void {
        if (::connect(fd_socket_, address_, address_len_)) {
            throw UnableToConnect();
        }
    }

    auto Socket::send(std::string_view buffer) const -> long {
        return ::send(fd_socket_, buffer.data(), buffer.size(), 0);
    }

    auto Socket::recv(uint8_t* buffer) const -> long {
        return ::recv(fd_socket_, buffer, BUFSIZ, 0);
    }

    Socket::~Socket() {
        if (fd_socket_) {
            close(fd_socket_);
        }
    }
}
