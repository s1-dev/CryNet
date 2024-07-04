#include <stdio.h>
#include <libircclient.h>
#include <libirc_rfcnumeric.h>
#include <sstream>
#include <iostream>
#include <cstring>
#include <csignal>


irc_session_t *session;

std::string concatenateParams(const char *firstParam, const char **params, unsigned int count) {
    std::ostringstream oss;
    oss << firstParam << " ";
    for (unsigned int i = 0; i < count; ++i) {
        if (i > 0) {
            oss << " ";
        }
        oss << params[i];
    }
    return oss.str();
}

void closeConnection() {
    if (session) {
        irc_send_raw(session, "/QUIT");
        irc_disconnect(session);
        irc_destroy_session(session);
        session = nullptr;
    }
}

void signalHandler(int signal) {
    std::cout << "Interrupt signal (" << signal << ") received.\n";
    closeConnection();
    exit(signal);
}

// Callback for connecting event
void event_connect(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count) {
    printf("Connected to server\n");

    // Custom CryNet IRC Server auth and connection
    irc_send_raw(session, "/VALIDATE test123 BOT");
    irc_send_raw(session, "/NICK bot");
    irc_send_raw(session, "/USER botNick 0 * botUser");
    irc_send_raw(session, "/JOIN #testChannel");
    irc_send_raw(session, "/LIST *");
}

// Callback for unknown events
void event_unknown(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count) {
    std::string receivedMessage;
    if (event != NULL) {
        receivedMessage = concatenateParams(event, params, count);
    } else {
        receivedMessage = concatenateParams("", params, count);
    }

    printf("Received message: %s\n", receivedMessage.c_str());
}


int main(int argc, char *argv[]) {
    // Set up signal handlers
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    irc_callbacks_t callbacks;
    irc_session_t *session;
    const char *server = "<INSERT_IP>";
    int port = 6667;

    memset(&callbacks, 0, sizeof(callbacks));

	callbacks.event_connect = event_connect;
	callbacks.event_unknown = event_unknown;

    session = irc_create_session(&callbacks);
    if (!session) {
        printf("Could not create IRC session\n");
        return 1;
    }

    try {
        if (irc_connect(session, "<INSERT_IP>", 6667, nullptr, "nick", "user", "realname")) {
            std::cerr << "Failed to connect to IRC server." << std::endl;
            closeConnection();
            return 1;
        }

        irc_run(session);
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        closeConnection();
    }


    closeConnection();
    return 0;
}
