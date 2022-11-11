#include "server.h"
#include "log.h"
#include "utils.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <unistd.h>

#define MAX_EVENTS 32
#define BUF_SIZE 4096

// Find out the server pid
// ps aux | grep <executable>
// Show open descriptors
// lsof -p <pid>
void Server::Listen(int port) {
    masterSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (masterSocket == -1) {
        Log::out << "can't create socket" << std::endl;
    }

    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(port);

    if (!host.empty()) {
        // Convert ip addr to binary representation and save it to the structure
        int res = inet_aton(host.c_str(), &sockAddr.sin_addr);
        if (res == 0) {
            Log::out << "invalid hostname" << std::endl;
        }
    } else {
        sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    
    if (bind(masterSocket, (sockaddr*) &sockAddr, sizeof(sockAddr)) != 0) {
        Log::out << "can't bind local address to socket" << std::endl;
    }

    setNonBlock(masterSocket);
    
    if (listen(masterSocket, SOMAXCONN) == -1) {
        Log::out << "can't start listening connections" << std::endl;
    }

    // Use epoll to monitor multiple file descriptors
    // Create epoll descriptor
    int ePoll = epoll_create1(0);

    epoll_event event;
    event.data.fd = masterSocket;
    // Monitor availability for reading
    event.events = EPOLLIN;
    // Add descriptor to epoll
    epoll_ctl(ePoll, EPOLL_CTL_ADD, masterSocket, &event);

    while (true) {
        epoll_event events[MAX_EVENTS];
        // Wait for events with no timeout (infinite waiting)
        int n = epoll_wait(ePoll, events, MAX_EVENTS, -1);

        // Handle events
        for (int i = 0; i < n; i++) {
            if (events[i].data.fd == masterSocket) {
                int slaveSocket = accept(masterSocket, nullptr, nullptr);
                setNonBlock(slaveSocket);
                // Register event for slave socket
                epoll_event event;
                event.data.fd = slaveSocket;
                event.events = EPOLLIN;
                epoll_ctl(ePoll, EPOLL_CTL_ADD, slaveSocket, &event);
            } else {
                epoll_event event = events[i];
                pool.Push([&, event]() {
                    static char buf[BUF_SIZE];
                    int received = recv(event.data.fd, buf, BUF_SIZE, MSG_NOSIGNAL);
                    if (received == 0 && errno != EAGAIN) {
                        // Some error ocurred
                        shutdown(event.data.fd, SHUT_RDWR);
                        close(event.data.fd);
                    } else if (received > 0) {
                        Request req(buf);
                        Response res;
                        handler(req, res);
                        std::string resData = res.Create();
                        send(event.data.fd, resData.c_str(), resData.length(), MSG_NOSIGNAL);
                    }
                });
            }
        }
    }
}