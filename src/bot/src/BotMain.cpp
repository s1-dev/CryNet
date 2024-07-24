#include "IrcClient.hpp"
#include <csignal>
#include <iostream>

int main(int argc, char* argv[]) {
    // Set up signal handlers
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
