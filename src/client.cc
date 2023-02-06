// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <iostream>
#include <sstream>
#include <string>

#include <express/client.h>
#include <express/request.h>
#include <express/socket.h>
#include <express/url.h>

namespace Express {
    auto Client::request(const Http::RequestConfig& config) const -> void {
        Net::URL url {config.url};

        Net::Socket socket {{url.host(), url.port()}};
        socket.connect();

        // TODO: normalize config file

        Http::Request request {config, url};
        std::stringstream request_buffer;
        request.writeRequest(request_buffer);

        socket.send(request_buffer.str());

        std::stringstream response_buffer;
        char buffer[BUFSIZ];
        std::streamsize bytes = 0;
        while ((bytes = socket.recv(buffer)) > 0) {
            response_buffer.write(buffer, bytes);
            response_buffer << Http::crlf;
        }

        auto output = response_buffer.str();
        std::cout << output;
    }

    auto Client::prepareRequestWithNoData(
        const Http::Method method,
        std::string_view url
    ) const -> void {
        request({
            .method = method,
            .url = url,
            .headers = {},
        });
    }

    auto Client::prepareRequestWithData(
        const Http::Method method,
        std::string_view url
    ) const -> void {
        request({
            .method = method,
            .url = url,
            .headers = {{
                {"Content-Type", "multipart/form-data"},
            }},
        });
    }

    auto Client::get(std::string_view url) const -> void {
        prepareRequestWithNoData(Http::Method::Get, url); 
    }

    auto Client::post(std::string_view url) const -> void {
        prepareRequestWithData(Http::Method::Post, url);
    }
}
