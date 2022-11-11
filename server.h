#pragma once

#include "thread_pool.h"
#include "response.h"
#include "request.h"

#include <string>
#include <vector>
#include <functional>
#include <netinet/in.h>

class Server {
public:
    Server() = delete;
    Server(const std::string& host_, std::function<void(Request&, Response&)> fn) : pool(5), handler(fn), host(host_) {}
    ~Server() = default;

    void Listen(int port);
private:
    ThreadPool pool;
    std::function<void(Request&, Response&)> handler;
    std::string host;
    sockaddr_in sockAddr;
    int masterSocket;
};