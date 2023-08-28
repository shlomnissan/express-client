<h1 align="center">
   <b>
        <img style="margin: 1rem 0" src="https://github-production-user-asset-6210df.s3.amazonaws.com/3165988/253027928-4a160e7d-7a4b-46e4-a1e1-db10e96d83c1.svg" /> 
    </b>
</h1>


<p align="center">Promised-based HTTP client for modern C++ development.</p>

<div align="center">

[![Ubuntu](https://github.com/shlomnissan/express-client-rev/actions/workflows/ubuntu.yml/badge.svg)](https://github.com/shlomnissan/express-client-rev/actions/workflows/ubuntu.yml)
[![MacOS](https://github.com/shlomnissan/express-client-rev/actions/workflows/macos.yml/badge.svg)](https://github.com/shlomnissan/express-client-rev/actions/workflows/macos.yml)

</div>

## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Getting Started](#getting-started)
   - [Platform Support](#platform-support)
   - [Installation](#installation)
   - [Example](#example)
- [Express Client API](#express-client-api)
   - [Request](#request)
   - [Types](#types)
   - [Response](#response)
   - [Error Handling](#error-handling)
- [Licence](#licence)

## Overview
Express is a promise-based HTTP client for modern C++ development. It currently targets C++20 with no dependencies other than the standard library. The project aims to provide a modern interface for making HTTP requests in C++ applications.

## Features
- A simple interface.
- Asynchronous requests.
- Cross-platform support.
- Basic HTTP authentication.
- Zero dependencies.
- Comprehensive tests.

#### Upcoming Features
- HTTPS support.
- File upload and download with progress.
- Connection pooling.

## Getting Started

### Platform Support
For now, Express Client only supports Unix-based operating systems, but support for Windows is coming soon. The project is currently being tested on the following operating systems and compilers:

![Ubuntu](https://raw.githubusercontent.com/EgoistDeveloper/operating-system-logos/master/src/16x16/UBT.png) Ubuntu 22.04 and GCC 11.3.0

![macOS](https://raw.githubusercontent.com/EgoistDeveloper/operating-system-logos/master/src/16x16/MAC.png) macOS 12 and Clang 14.0.0

### Installation
Express Client uses [CMake](https://cmake.org/) as its default build system. There are multiple ways to import a CMake project into an existing codebase, but it is highly recommended to import the library using CMake Install following the steps below. This ensures that symbol visibility is maintained, which makes versioning and future updates more reliable.

#### 1. Installing CMake
The first step is to ensure that CMake is installed on your system. Follow the [CMake installation steps](https://cmake.org/install/) for your operating system and run the following command from the terminal to verify the installation. If CMake was installed successfully, you should see similar output:
<pre>
/$ <strong>cmake --version</strong>
-- cmake version 3.26.4
-- CMake suite maintained and supported by Kitware (kitware.com/cmake).
</pre>
_Express Client requires a minimum version of CMake 3.22._
#### 2. Download
The next step is downloading the Express Client source code. The easiest way to download Express Client is cloning the repository from GitHub:
<pre>
  /$ <strong>git clone https://github.com/shlomnissan/express-client.git</strong>
</pre>
You can also download an archive file directly from the GitHub UI.
#### 3. Build
To build Express Client using CMake, navigate to the project's root directory and execute the following commands:
<pre>
# Create the build directory
/$ <strong>mkdir build</strong>
/$ <strong>cd build</strong>

# Generate the build
/$ <strong>cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON -DBUILD_TESTS=OFF -DBUILD_EXAMPLES=OFF</strong>
</pre>
The CMake CLI used to generate the build specifies several modifiable options:

- `CMAKE_BUILD_TYPE` is set to `Release`, which is desirable for installation. However, if you are actively testing and modifying the project, you can change this value to `Debug`.
- `BUILD_SHARED_LIBS` is set to `ON`, which makes the build output a shared library. Omitting this option altogether results in building a static library.
- `BUILD_TESTS` and `BUILD_EXAMPLES` are self-explanatory.

The next step is building the project:
<pre>
# Build the project
/$ <strong>cmake --build . --config Release</strong>
</pre>
We are specifying the build configuration again which is required for multi-config build systems. If you changed the value to `Debug` during the build phase, make sure to change it here as well.

#### 4. Install
Finally, you can use CMake to install the library on your host.
<pre>
# Install the project
/$ <strong>cmake --install .</strong>
</pre>
Assuming everything went well, you should now have Express Client installed locally and can include it directly in your projects. If you're on a Unix-based system, running the following command will list the installed files and their respective paths:
<pre>
/$ <strong>cat install_manifest.txt</strong>
</pre>
The next section shows a simple example of using Express Client.

### Example
The following example shows how to perform a simple HTTP GET request in your C++ projects and print the output to `stdout`.
```cpp
#include <iostream>
#include <express/client.h>

auto main() -> int {
  Express::Client client;

  auto result = client.Request({
    .url = "http://example.com/",
    .method = Express::Method::Get,
  });

  auto response = result.get();

  if (response.status_code == 200) {
    std::cout << response.data << '\n';
  }

  return 0;
}
```
Now you can compile and run the executable.
<pre>
# Compile main.cc and link the express client library
/$ <strong>g++ main.cc -lexpress_client -std=c++20</strong>

# Run the executable
/$ <strong>./a.out</strong>
</pre>

- Note that all of the examples in this article are using [Designated Initializers](https://en.cppreference.com/w/cpp/language/aggregate_initialization), which is a feature introduced in C++20. This feature enables us to directly initialize members of a class type using their name.
- We include the `<express/client.h>` header file and link the executable with `libexpress_client`. If you have followed the installation steps, the header files and library should be available in your system's default search paths.

#### Troubleshooting
- If you have followed the steps to install the library locally and are seeing the error message "error while loading shared libraries" when running the executable, you may need to update your local shared library cache by running the following command:
<pre>
/$ <strong>ldconfig</strong>
</pre>

## Express Client API

### Request
The Express Client interface has a single operation, which is making an HTTP request. Therefore, every interaction with the library begins by calling the `Request()` method on the client's object and passing it a configuration object.

```cpp
#include <express/client.h>

namespace Express {
  class Client {
    auto Request(const Config& config) const -> std::future<Response>;
  };
}
```
- This method processes the request in the background without blocking execution. It returns a `std::future`, which lets you access the result of the asynchronous operation.
- `std::future` provides several methods for querying, waiting for, or extracting a value. The most commonly used method is `std::future<T>::get()`, which waits until the future has a valid result and retrieves it. If the future does not have a result, this method will block the execution, waiting until the result becomes available.
- Here is a code snippet from the example we showed earlier with clarifying comments:
```cpp
// Instantiate our HTTP client
Express::Client client;

// Send an asynchronous request to http://example.com/
auto result = client.Request({
  .url = "http://example.com/",
  .method = Express::Method::Get,
});

// Get the response if it's available, otherwise wait until it's available
auto response = result.get();
```
The following section will describe the different types provided by the Express Client. We will start with the configuration object that is used to make requests, which includes all the options that can be set when making an HTTP request.

### Types

#### Express::Config
Every request requires a configuration object of type `Express::Config`. This is a simple data type with fields that lets you configure your HTTP request. We’ve shown two fields in the example above, `url` which is required, and `method`. The following table lists all available fields, their types, and a description.


| Name | Type | Description |
| ------------- | ------------- | ------------- |
| **url**  | `std::string_view`  | A valid URL that includes the URL scheme. |
| **method**  | `Express::Method`  | An HTTP method supported by Express. |
| **headers**  | `Express::Headers`  | A collection of headers for the HTTP request. |
| **data**  | `std::string_view`  | Data to include with the request. |
| **auth**  | `Express::UserAuth`  | A username and password pair for authentication. |
| **timeout**  | `std::chrono::milliseconds`  | A request timeout in milliseconds. |

Before we delve into the nested types, let's take a look at an example of an HTTP request that uses all the fields in the configuration object:

```cpp
auto result = client.Request({
  .url = "http://example.com/user",
  .method = Express::Method::Post,
  .headers = {{
    {"Content-Type", "application/x-www-form-urlencoded"}
  }},
  .data = "age=22&email=myemail@hello.com",
  .auth = {
    .username = "aladdin",
    .password = "opensesame",
  },
  .timeout = 15s
});
```
Note that when using designated initializers, the initialization expression must have the same order of data members as in the class declaration. However, we are allowed to omit members.

#### Express::Method

`Express::Method` is an enum class used to specify the HTTP method within the request object. Express Client currently supports the following methods:

```
Express::Method::Delete
Express::Method::Get
Express::Method::Head
Express::Method::Options
Express::Method::Patch
Express::Method::Post
Express::Method::Put
```

#### Express::Headers

`Express::Headers` is a collection type used to store headers for a request or retrieve headers from a response. It can be initialized by passing a vector of string pairs, or using an initializer list:

```cpp
Express::Headers headers {{
  {"Header-Name", "Value"},
  {"Header-Name", "Value"}
}};
```

This collection object includes a couple of mutators (`Express::Headers::Add`, `Express::Headers::Remove`) and accessors (`Express::Headers::Contains`, `Express::Headers::Get`) to configure the object as part of an HTTP request and query the object as part of an HTTP response.

```cpp
auto Add(const std::string& name, std::string value) -> void;
auto Remove(const std::string& name) -> void;

[[nodiscard]] auto Contains(const std::string& name) const -> bool;
[[nodiscard]] auto Get(const std::string& name) -> std::string;
```
The headers collection is a simple wrapper around an unordered map. However, it accounts for case-insensitive header names and provides basic validation for allowed characters in the header names and values.

#### Express::UserAuth

`Express::UserAuth` is a simple aggregate type used for HTTP authentication. It can be initialized with designated initializers as shown below:

```cpp
Express::UserAuth user_auth {
  .username = "aladdin",
  .password = "opensesame"
};
```
This user-defined type is part of the request object. If the username or password are set, the request builder will add an Authorization header with this information encoded in base64, following the specification for Basic HTTP Authentication ([RFC 7617](https://datatracker.ietf.org/doc/html/rfc76170)).
### Response

The type of value we receive from the request method's future is `Express::Response`. This is another simple data structure that mostly includes standard library types. The table below lists all available fields, their types, and default values.

| Name | Type | Description |
| ------------- | ------------- | ------------- |
| **status_code**  | `int`  | An [HTTP response status code](https://developer.mozilla.org/en-US/docs/Web/HTTP/Status). |
| **status_text**  | `std::string`  | An HTTP status code text. |
| **data**  | `std::string`  | A string that includes the body’s data. |
| **headers**  | `Express::Headers`  | A collection of key/value headers from the HTTP response. |

The only user-defined type in this data structure is `Express::Headers`, which was covered in the previous section. Here is an example that makes a simple request and prints the entire response using all the fields in the response object.

```cpp
auto result = client.Request({
  .url = "http://example.com",
  .method = Express::Method::Get,
});

Express::Response response = result.get();

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
```
_This code snippet was taken from the `hello_client.cc` example_.

### Error Handling

Express Client uses exceptions as its primary error handling mechanism. Your application will need to handle three types of exceptions:

| Exception Type | Description |
| ------------- | ------------- |
| `std::system_error` | A standard library exception that indicates a low-level system error, typically in the networking APIs. This error can occur in cases of resource limitations, permission issues, or underlying network issues, such as when the destination is unreachable.|
| `Express::RequestError` | A library exception that indicates a configuration error preventing the library from making a request. This exception is derived from `std::logic_error`, and can occur in cases such as a bad URL format, invalid header values, or bad data formatting.|
| `Express::ResponseError` | A library exception that indicates an unexpected runtime error that happened after the library made a request to the server. This exception is derived from `std::runtime_error`, and can occur in cases such as failure to connect to the server, timeout,  unexpected server responses, and more. |

- The library exceptions are defined in `<express/exception.h>`.
- The three exceptions all include error messages that provide information about the source of the error and the issues that caused the exception to be thrown.

## Licence
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
