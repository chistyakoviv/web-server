#pragma once

#include <string>
#include <fstream>

class Log {
public:
    static void Init(const std::string& path) {
        out.open(path);
    }

    static void Finish() {
        out.close();
    }
public:
    static std::ofstream out;
};