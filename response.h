#pragma once

#include <unordered_map>
#include <string>

class Response {
public:
    Response() = default;
    ~Response() = default;

    Response& SetHeaders(std::unordered_map<std::string, std::string> headers);

    Response& SetBody(const std::string& body);

    Response& SetStatusCode(int code);

    std::string Create() const;
private:
    std::unordered_map<std::string, std::string> responseHeaders = {
        { "Content-Length", "0" },
        { "Content-Type", "text/html" }
    };
    std::string proto = "HTTP/1.0";
    std::string statusMessage;
    std::string data;
    int statusCode = 200;
};