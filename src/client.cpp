// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <simple_http/client.h>
#include <simple_http/socket.h>

namespace simplehttp {
    auto Client::get() const -> int {
        Socket socket {{"example.com", "80"}};
        socket.connect();

        return 12;
    }
}