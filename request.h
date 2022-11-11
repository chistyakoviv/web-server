#pragma once

#include <string>
#include <unordered_map>

class Request {
public:
    Request(char* buf);
    ~Request() = default;

    std::string GetMethod() const;

    std::string GetPath() const;

    bool HasHeader(const std::string& header) const;

    std::string GetHeader(const std::string& header) const;
private:
    void parseStartLine(std::stringstream& ss);
    void parseHeaders(std::stringstream& ss);
private:
    std::unordered_map<std::string, std::string> headers;
    std::string method;
    std::string uri;
    std::string path;
    std::string params;
    std::string proto;
};