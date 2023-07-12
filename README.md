<h1 align="center">
   <b>
        <img style="margin: 1rem 0" src="https://github-production-user-asset-6210df.s3.amazonaws.com/3165988/253027269-9e753a73-cafa-43cd-952a-21cad2650966.svg" /> 
    </b>
</h1>

<p align="center">Promised-based HTTP client for modern C++</p>

<div align="center">

[![Ubuntu](https://github.com/shlomnissan/express-client/actions/workflows/ubuntu.yml/badge.svg)](https://github.com/shlomnissan/express-client/actions/workflows/ubuntu.yml)
[![MacOS](https://github.com/shlomnissan/express-client/actions/workflows/macos.yml/badge.svg)](https://github.com/shlomnissan/express-client/actions/workflows/macos.yml)

</div>

## Table of Contents

  - [Features](#features)
  - [Platform Support](#platform-support)
  - [Getting Started with CMake](#getting-started-with-cmake)
  - [HTTPS and Certificate Verification](#https-and-certificate-verification)
  - [Express API](#express-api)
  - [Request Config](#request-config)
  - [Response Schema](#response)
  - [Licence](#mit-licence)

## Features

- Written in modern C++
- Simple interface (loosely modeled after JavaScript Axios)
- HTTPS support
- Uses std::future for async requests
- Single optional dependency (OpenSSL)
- Basic HTTP authentication
- Comprehensive tests

## Platform Support

For now, Express Client only supports Unix-based operating systems. If there is demand, I will add support for Windows. The project is currently being tested on the following operating systems and compilers:

![Ubuntu](https://raw.githubusercontent.com/EgoistDeveloper/operating-system-logos/master/src/16x16/UBT.png) Ubuntu 22.04 and GCC 11.3.0

![macOS](https://raw.githubusercontent.com/EgoistDeveloper/operating-system-logos/master/src/16x16/MAC.png) macOS 12 and Clang 14.0.0

## Getting Started: with CMake

### Setting up a project

CMake uses a file named `CMakeLists.txt` to configure the build system for a project. You’ll use this file to set up your project and declare a dependency on Express Client.

First, create a directory for your project:

```
$ mkdir my_project && cd my_project
```
Next, you’ll create the `CMakeLists.txt` file and declare a dependency on Express Client. There are many ways to express dependencies in the CMake ecosystem; in this guide, you’ll use the [`FetchContent` CMake module](https://cmake.org/cmake/help/latest/module/FetchContent.html). To do this, in your project directory (`my_project`), create a file named `CMakeLists.txt` with the following contents:

```cmake
cmake_minimum_required(VERSION 3.20)

project(my_project)

# ExpressClient requires C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

include(FetchContent)
FetchContent_Declare(
    express 
    GIT_REPOSITORY https://github.com/shlomnissan/express-client.git
    GIT_TAG origin/main
)

set(BUILD_TESTS OFF CACHE BOOL "")
FetchContent_MakeAvailable(express)

add_executable(app main.cc)

target_link_libraries(app PRIVATE Express::Client)

```
The configuration above declares a dependency on Express Client, which is downloaded from GitHub. It also adds a target executable called app with a single source file that we can use to test Express Client.

### Create and run a binary

Now that we have the project set up, create a file named `main.cc` in your `my_project` directory with the following contents:
```cpp
#include <iostream>
#include <string>
#include <express/client.h>

auto main() -> int {
    // make a simple HTTP GET request to example.com
    auto result = Express::ExpressClient::request({
        .url = "http://example.com/",
        .method = Express::Http::Method::Get,
    });

    // request() returns a std::future. calling get will block until the
    // future has valid results
    auto response = result.get();

    // check if the request was successful and print the data
    if (response.statusCode == 200) {
        std::string data {cbegin(response.data), cend(response.data)};
        std::cout << '\n' << data << '\n';
    }

    return 0;
}
```
Now you can build and run the application:

<pre>
<strong>my_project$ cmake -S . -B build</strong>
-- The C compiler identification is GNU 10.2.1
-- The CXX compiler identification is GNU 10.2.1
...
-- Build files have been written to: .../my_project/build

<strong>my_project$ cmake --build build</strong>
-- Scanning dependencies of target gtest
...
-- [100%] Built target app

<strong>my_project$ ./build/app</strong>
-- HTML output
</pre>

Congratulations! You have successfully added the Express Client library to your project and performed a simple HTTP request.

## HTTPS and certificate verification

Every operating system provides a list of trusted Certificate Authorities (CAs) that can be used to verify server certificates. However, there is no general way to import these lists.

Mozilla maintains its own trusted CA store, which is frequently used by HTTP clients. [curl](https://curl.se/) has developed tools to extract these certificates from Firefox and convert them to a file containing the CAs' digital signatures suitable for server certificate verification. This file can be downloaded directly from their servers at https://curl.se/docs/caextract.html.

The **express-client** library executable requires a trusted CAs PEM file named `ca-bundle.crt` to be placed in the same directory. This project includes a CMake option called `FETCH_MOZ_TRUSTED_CA` (enabled by default) that downloads the Mozilla CA store into the build's binary directory. If you prefer to use a different store, turn off this option and place your own `ca-bundle.crt` file alongside the **express-client** library executable.

## Express API

HTTP requests are made by passing a configuration object to `ExpressClient::request`. This method returns a non-blocking `std::future`.

```cpp
using namespace Express;

// A simple GET request
auto result = ExpressClient::request({
    .url = "http://example.com/user/12345",
    .method = Http::Method::Get,
});

auto response = result.get();
```

```cpp
using namespace Express;

// A simple POST request
auto result = ExpressClient::request({
    .url = "http://example.com/user/12345",
    .method = Http::Method::Post,
    .data = {{
        {"firstName", "Fred"},
        {"lastName", "Flintstone"}
    }}
});

auto response = result.get();
```

## Request Config

These are the available configuration options for making requests. The `url` is the only required field. Requests will default to `GET` if `method` is not specified.

```cpp
struct RequestConfig {
    // `url` is the server URL used for the request.
    std::string_view url;
    
    // `method` is the request method to be used when making the request.
    // The client supports 'OPTIONS', 'GET', 'POST', 'HEAD', 'PUT', 'PATCH', and 'DELETE'.
    Method method {Http::Method::Get};
    
    // `data` is the data to be sent as the request body.
    // Only applicable for request methods 'POST', 'PUT', PATCH', and 'DELETE'.
    Data data {};
    
    // `headers` are custom headers to be sent.
    HeaderCollection headers {};
    
    // `timeout` specifies the number of milliseconds before the request times out.
    // If the request takes longer than `timeout`, the request will be aborted.
    std::chrono::milliseconds timeout {0}; // default is `0` (no timeout)
};
```

The configuration object is an aggregate type; defining it inline it using Designated Initializers (C++20) is recommended.

```cpp
auto result = ExpressClient::request({
    .url = "http://example.com/user/12345",
    .method = Http::Method::Post,
    .data = {{
        {"firstName", "Fred"},
        {"lastName", "Flintstone"}
    }}
});
```

The data field can be constructed using key/value pairs (like the example above) or a raw string. If you use key/value pairs, the default content type will be `application/x-www-form-urlencoded`. Using a raw string, you must specify the content type yourself. For example:

```cpp
auto result = ExpressClient::request({
    .url = "http://example.com/user/12345",
    .method = Http::Method::Post,
    .data = {"firstName=Fred&lastName=Flintstone"},
    .headers = {{
        {"ContentType", "application/x-www-form-urlencoded"}
    }}
});
```


## Response

The response for a request contains the following information.

```cpp
struct Response {
    // `statusCode` is the HTTP status code from the server response
    int statusCode;
    
    // `statusText` is the HTTP status message from the server response
    std::string statusText;
    
    // `data` is the response that was provided by the server
    std::vector<uint8_t> data;
    
    // `headers` the HTTP headers that the server responded with
    HeaderCollection headers;
};
```

Here's an example that inspects the response headers and data:

```cpp
auto response = ExpressClient::request({
    .url = "http://example.com",
    .method = Http::Method::Get,
}).get();

std::cout << "Headers: \n";
for (const auto& header : response.headers) {
    std::cout << header.name() << ": " << header.value() << '\n';
}

std::string data {cbegin(response.data), cend(response.data)};
std::cout << '\n' << data << '\n';
    
```

The `HeaderCollection` type has public case-insensitive lookup methods for inspecting headers.

```cpp
auto response = ExpressClient::request({
    .url = "http://example.com",
    .method = Http::Method::Get,
}).get();

if (response.headers.has("cache-control")) {
    std::cout << response.headers.get("cache-control");
}

```

## MIT Licence
```
    ____       __                             __  
   / __ )___  / /_____ _____ ___  ____ ______/ /__
  / __  / _ \/ __/ __ `/ __ `__ \/ __ `/ ___/ //_/
 / /_/ /  __/ /_/ /_/ / / / / / / /_/ / /  / ,<   
/_____/\___/\__/\__,_/_/ /_/ /_/\__,_/_/  /_/|_|  
                                                  
Copyright (c) 2023-present Shlomi Nissan
https://betamark.com

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
```
