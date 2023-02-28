# ⚡ express-client
Promise based HTTP client for modern C++ development

[![express](https://github.com/shlomnissan/express-client/workflows/Express/badge.svg)](https://github.com/shlomnissan/express-client/actions/workflows/main.yml)

## Table of Contents

  - [Express API](#express-api)
  - [Request Config](#request-config)
  - [Response Schema](#response-schema)

## Express API

Requests can be made by passing the relevant config to `ExpressClient::request`.

##### ExpressClient::request(RequestConfig)

```cpp
using namespace Express;

// A simple GET request
auto response = ExpressClient::request({
    .url = "http://example.com/user/12345",
    .method = Http::Method::Get,
});
```

```cpp
using namespace Express;

// A simple POST request
auto response = ExpressClient::request({
    .url = "http://example.com/user/12345",
    .method = Http::Method::Post,
    .data = {{
        {"firstName", "Fred"},
        {"lastName", "Flintstone"}
    }}
});
```

## Request Config

These are the available config options for making requests. Only the `url` is required. Requests will default to `GET` if `method` is not specified.

```cpp
struct RequestConfiguration {
    // `url` is the server URL used for the request.
    // `std::string_view` (C++17) is a non-owning reference to a char sequence.
    std::string_view url;
    
    // `method` is the request method to be used when making the request.
    // The client supports 'OPTIONS', 'GET', 'POST', 'HEAD', 'PUT', 'PATCH', and 'DELETE'.
    Method method {Http::Method::Get};
    
    // `data` is the data to be sent as the request body.
    // Only applicable for request methods 'POST', 'PUT', PATCH', and 'DELETE'.
    Data data {};
    
    // `headers` are custom headers to be sent.
    HeaderCollection headers {};
};
```

The configuration object is an aggregate type; defining it inline it using Designated Initializers (C++20) is recommended.

```cpp
auto response = ExpressClient::request({
    .url = "http://example.com/user/12345",
    .method = Http::Method::Post,
    .data = {{
        {"firstName", "Fred"},
        {"lastName", "Flintstone"}
    }},
    .headers = {{
        {"X-Requested-With", "XMLHttpRequest"}
    }}
});
```

## Response Schema

The response for a request contains the following information.

```cpp
struct Response {
    // `status` is the HTTP status code from the server response
    int status_code;
    
    // `status_text` is the HTTP status message from the server response
    std::string status_text;
    
    // `data` is the response that was provided by the server
    std::vector<uint8_t> data;
    
    // `headers` the HTTP headers that the server responded with
    HeaderCollection headers;
};
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
