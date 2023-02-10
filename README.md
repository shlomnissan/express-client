# ⚡ express-client
Promise based HTTP client for modern C++ development

### Express API

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
    .data = Body::FormFields {{
        {"firstName", "Fred"},
        {"lastName", "Flintstone"}
    }}
});
```
