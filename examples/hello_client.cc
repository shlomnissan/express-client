#include <iostream>

#include <express/client.h>
#include <express/version.h>

auto main() -> int {
    std::cout << "Express Client version: "
        << Express::Version::Major << '.'
        << Express::Version::Minor << '.'
        << Express::Version::Patch << '\n';

    Express::Client client;
    
    auto response = client.Request({
        .url = "http://example.com"
    });

    std::cout << response.get().data;

    return 0;
}