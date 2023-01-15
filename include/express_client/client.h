// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#ifndef EXPRESS_CLIENT_H
#define EXPRESS_CLIENT_H

namespace EXPRESS {
    class Client {
    public:
        auto get() const -> int;
    };
}

#endif  // EXPRESS_CLIENT_H