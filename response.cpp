#include "response.h"

#include <sstream>

static std::unordered_map<int, std::string> statusMessages = {
    { 200, "OK" },
    { 404, "NOT FOUND" }
};

Response& Response::SetHeaders(std::unordered_map<std::string, std::string> headers) {
    for (auto it = headers.begin(); it != headers.end(); it++) {
        responseHeaders[(*it).first] = (*it).second;
    }
    return *this;
}

Response& Response::SetBody(const std::string& body) {
    data = body;
    responseHeaders["Content-Length"] = std::to_string(body.length());
    return *this;
}

Response& Response::SetStatusCode(int code) {
    statusCode = code;
    return *this;
}

std::string Response::Create() const {
    std::stringstream ss;
    ss << proto << " " << statusCode;
    ss << " " << (statusMessage.empty() ? statusMessages[statusCode] : statusMessage);
    ss << "\r\n";
    for (auto it = responseHeaders.begin(); it != responseHeaders.end(); it++) {
        ss << (*it).first << ": " << (*it).second;
        ss << "\r\n";
    }
    ss << "\r\n";
    ss << data;
    return ss.str();
}