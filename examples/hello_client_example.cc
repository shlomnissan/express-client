// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <iostream>
#include <string>

#include <express/client.h>

int main() {
    auto result = Express::Client::request({
        .url = "http://www.example.com",
        .method = Express::Method::Get,
    });

    Express::Response response = result.get();

    std::cout << "Status: "
              << response.statusCode << " "
              << response.statusText << "\n\n";

    std::cout << "Headers: \n";
    for (const auto& header : response.headers) {
        std::cout << header;
    }

    if (response.data.size()) {
        std::string data {cbegin(response.data), cend(response.data)};
        std::cout << '\n' << data << '\n';
    }

    return 0;
}