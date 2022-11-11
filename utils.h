#pragma once

#include <string>
#include <vector>

#include <unistd.h>
#include <getopt.h>

struct cmdopts {
    std::string host;
    int port = 0;
    std::string dir;
    int showHelp = 0;
};

bool parseParams(int argc, char** argv, cmdopts* opts);

std::string curPath();

int setNonBlock(int fd);

std::vector<std::string> split(const std::string& str, const std::string& delim = " ");