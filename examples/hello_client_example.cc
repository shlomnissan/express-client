#include <iostream>
#include <string>

#include <express/client.h>

using namespace Express;

int main() {
    auto response = ExpressClient::request({
        .url = "http://example.com",
        .method = Http::Method::Get,
    });

    std::cout << "Status: "
              << response.status << " "
              << response.status_text << "\n\n";

    std::cout << "Headers: \n";
    for (const auto& header : response.headers) {
        std::cout << header;
    }

    std::string data {cbegin(response.data), cend(response.data)};
    std::cout << '\n' << data << '\n';

    return 0;
}