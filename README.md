<h1 align="center">
   <b>
        <img style="margin: 1rem 0" src="https://github-production-user-asset-6210df.s3.amazonaws.com/3165988/253027928-4a160e7d-7a4b-46e4-a1e1-db10e96d83c1.svg" /> 
    </b>
</h1>


<p align="center">Promised-based HTTP client for modern C++ development</p>

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
- Express Client API
   - Request
   - Types
     - Express::Config
     - Express::Method
     - Express::Headers
     - Express::UserAuth
   - Response
   - Error Handling
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

We include the `<express/client.h>` header file and link the executable with `libexpress_client`. If you have followed the installation steps, the header files and library should be available in your system's default search paths.

#### Troubleshooting
- If you have followed the steps to install the library locally and are seeing the error message "error while loading shared libraries" when running the executable, you may need to update your local shared library cache by running the following command:
<pre>
/$ <strong>ldconfig</strong>
</pre>

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
