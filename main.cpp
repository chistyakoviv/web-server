#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <getopt.h>

#include "utils.h"
#include "server.h"
#include "request.h"
#include "response.h"
#include "log.h"

int main(int argc, char** argv) {
    std::string workDir = curPath();

    cmdopts opts;
    // Set default params
    opts.host = "0.0.0.0";
    opts.port = 12345;
    opts.dir = workDir;

    if (!parseParams(argc, argv, &opts)) {
        std::cout << "missing required params" << std::endl;
        return 1;
    }

    if (opts.showHelp) {
        std::cout << "web server version 1.0" << std::endl;
        return 0;
    }

    daemon(0, 0);

    Log::Init(workDir + "/server.log");
    Log::out << "start server on " << opts.host << ":" << opts.port << std::endl;
    Log::out << "working directory is " << opts.dir << std::endl;

    Server server(opts.host, [&opts](Request& req, Response& res) {
        std::ifstream f(opts.dir + req.GetPath());
        if (f.good()) {
            std::stringstream ss;
            ss << f.rdbuf();
            res
                .SetStatusCode(200)
                .SetBody(ss.str());
        } else {
            res.SetStatusCode(404);
        }
        f.close();
    });
    server.Listen(opts.port);

    Log::out << "terminate server" << std::endl;
    Log::Finish();
}