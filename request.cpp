#include "request.h"
#include "log.h"
#include "utils.h"

#include <sstream>
#include <vector>
#include <unordered_set>

static std::unordered_set<std::string> methods = { "GET", "POST", "PUT", "PATH", "DELETE" };

Request::Request(char* buf) {
    std::stringstream ss(buf);
    parseStartLine(ss);
    parseHeaders(ss);
}

std::string Request::GetMethod() const {
    return method;
}

std::string Request::GetPath() const {
    return path;
}

bool Request::HasHeader(const std::string& header) const {
    return headers.count(header);
}

std::string Request::GetHeader(const std::string& header) const {
    if (headers.count(header)) {
        return headers.at(header);
    }
    return "";
}

void Request::parseStartLine(std::stringstream& ss) {
    std::string s;
    std::getline(ss, s);
    std::vector<std::string> startLine = split(s);
    if (startLine.size() == 3) {
        method = startLine[0];
        if (methods.count(method) == 0) {
            Log::out << "invalid method" << std::endl;
        }
        uri = startLine[1];
        proto = startLine[2];

        std::vector<std::string> requestPath = split(uri, "?");
        if (requestPath.size() == 2) {
            path = requestPath[0];
            params = requestPath[1];
        } else if (requestPath.size() == 1) {
            path = requestPath[0];
        } else {
            path = "/";
        }
    } else {
        Log::out << "invalid http request" << std::endl;
    }
}

void Request::parseHeaders(std::stringstream& ss) {
    std::string s;
    while (std::getline(ss, s)) {
        if (!s.empty()) {
            std::vector<std::string> res = split(s, ": ");
            if (res.size() == 2) {
                headers[res[0]] = res[1];
            }
        }
    }
}