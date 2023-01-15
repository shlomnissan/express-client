// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <express_client/client.h>
#include <express_client/socket.h>

namespace EXPRESS {
    auto Client::get() const -> int {
        Socket socket {{"example.com", "80"}};
        socket.connect();
        return 12;
    }
}
