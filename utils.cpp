#include "utils.h"

#include <fcntl.h>

bool parseParams(int argc, char** argv, cmdopts* opts) {
    static struct option longOptions[] = {
        // flag
        { "help",       no_argument,       &(*opts).showHelp, 1 },
        // opts
        { "host",       required_argument, nullptr,       'h' },
        { "port",       required_argument, nullptr,       'p' },
        { "directory",  required_argument, nullptr,       'd' },
        { nullptr,      0,                 nullptr,        0  }
    };

    int c;
    // Use variable to determine the order in which the option was defined
    int optionIndex = 0;
    // Put colon after option if the option has an argument
    while ((c = getopt_long(argc, argv, "h:p:d:", longOptions, &optionIndex)) != -1) {
        switch (c) {
            case 0:
                // Here the showHelp flag is set
                break;
            case 'h':
                (*opts).host = optarg;
                break;
            case 'p':
                (*opts).port = std::stoi(optarg);
                break;
            case 'd':
                (*opts).dir = optarg;
                break;

            case '?':
                // We are here if specified option was not defined in the options string ("h:p:d" in this case)
                return false;
            
            default:
                return false;
        }
    }
    return true;
}

std::string curPath() {
    char buf[255];
    getcwd(buf, 255);
    return buf;
}

int setNonBlock(int fd) {
    int flags;
    #ifdef O_NONBLOCK
        if (-1 == (flags = fcntl(fd, F_GETFL, 0))) {
            flags = 0;
        }
        return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    #else
        flags = 1;
        return ioctl(fd, FIOBIO, &flags);
    #endif
}

std::vector<std::string> split(const std::string& str, const std::string& delim) {
    std::vector<std::string> res;
    int start = 0;
    int end = -delim.length();
    do {
        start = end + delim.length();
        end = str.find(delim, start);
        res.push_back(str.substr(start, end - start));
    } while (end != -1);
    return res;
}