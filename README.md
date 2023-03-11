# ⚡ express-client
Asynchronous HTTP client for modern C++ development.

[![express](https://github.com/shlomnissan/express-client/workflows/Express/badge.svg)](https://github.com/shlomnissan/express-client/actions/workflows/main.yml)

## Table of Contents

  - [Express API](#express-api)
  - [Request Config](#request-config)
  - [Response Schema](#response)

## Express API

HTTP requests are made by passing a configuration object to `ExpressClient::request`. This method returns a non-blocking future.

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

These are the available config options for making requests. Only the `url` is required. Requests will default to `GET` if `method` is not specified.

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
