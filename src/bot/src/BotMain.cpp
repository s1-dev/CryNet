#include "IrcClient.hpp"
#include <csignal>
#include <iostream>
#include <unistd.h>

void ensureRootPrivileges() {
    if (geteuid() != 0) {
        std::cerr << "This program must be run as root. Please use sudo." << std::endl;
        exit(1);
    }
}

int main(int argc, char* argv[]) {
    ensureRootPrivileges();
    signal(SIGINT, IrcClient::signalHandler);
    signal(SIGTERM, IrcClient::signalHandler);
    while(true) {
        try {
            IrcClient client("192.168.1.17", 6667);
            client.connect();
        } catch (const std::exception& ex) {
            std::cerr << "Error: " << ex.what() << std::endl;
            //return 1;
        }
    }

    return 0;
}
