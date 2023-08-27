#include <iostream>

#include <express/client.h>
#include <express/version.h>

auto main() -> int {
    std::cout << "Express Client version: "
        << Express::Version::Major << '.'
        << Express::Version::Minor << '.'
        << Express::Version::Patch << '\n';

    Express::Client client;
    
    auto result = client.Request({
        .url = "http://example.com",
        .method = Express::Method::Get,
    });

    auto response = result.get();

    std::cout << "Status: "
        << response.status_code << " "
        << response.status_text << "\n\n";

    std::cout << "Headers: \n";
    for (const auto& [_, header] : response.headers) {
    std::cout << header.first << ": " << header.second << '\n';
    }

    if (!response.data.empty()) {
        std::cout << '\n' << response.data;
    }

    return 0;
}