// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#ifndef SIMPLEHTTP_CLIENT_H
#define SIMPLEHTTP_CLIENT_H

namespace simplehttp {
    class Client {
    public:
        auto get() const -> int;
    };
}

#endif  // SIMPLEHTTP_CLIENT_H