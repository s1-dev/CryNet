#include <stdio.h>
#include <libircclient.h>
#include <libirc_rfcnumeric.h>
#include <sstream>
#include <iostream>
#include <cstring>
#include <csignal>
#include <thread>

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

void closeConnection(irc_session_t *session) {  // check if this actually works... I suspect not since any irc commands are only working within callbacks... threading issue?
    if (session) {
        irc_disconnect(session);
        irc_destroy_session(session);
        session = nullptr;
    }
}

void signalHandler(int signal) {
    std::cout << "Interrupt signal (" << signal << ") received.\n";
    //closeConnection(); 
    exit(signal);
}

void sendCommand(irc_session_t *session, const std::string &command) {
    if (session) {
        irc_send_raw(session, command.c_str());
    }
}

// Callback for connecting event
void event_connect(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count) {
    printf("Connected to server\n");

    // Custom CryNet IRC Server auth and connection
    sendCommand(session, "/VALIDATE test123 BOT");
    sendCommand(session, "/NICK bot");
    sendCommand(session, "/USER botNick 0 * botUser");
    sendCommand(session, "/JOIN #testChannel");
    sendCommand(session, "/LIST *");
    sendCommand(session, "/NAMES #testChannel");
}

// Callback that captures messages from CryNet custom IRC server
void event_unknown(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count) {
    std::string receivedMessage;
    if (event != NULL) {
        receivedMessage = concatenateParams(event, params, count);
    } else {
        receivedMessage = concatenateParams("", params, count);
    }

    printf("%s\n", receivedMessage.c_str());
}


int main(int argc, char *argv[]) {
    // Set up signal handlers
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    irc_callbacks_t callbacks;
    irc_session_t *session;
    const char *server = "192.168.1.127";
    int port = 6667;
    const char *placeholder = "IGNORE";

    memset(&callbacks, 0, sizeof(callbacks));

	callbacks.event_connect = event_connect;
	callbacks.event_unknown = event_unknown;

    session = irc_create_session(&callbacks);
    if (!session) {
        printf("Could not create IRC session\n");
        return 1;
    }

    if (irc_connect(session, server, 6667, nullptr, placeholder, placeholder, placeholder)) {
        printf("Failed to connect to %s\n", server);
        closeConnection(session);
        return 1;
    }

    if (irc_run(session)) {
        printf("Error while connected to %s\n", server);
        closeConnection(session);
        return 1;
    }

    closeConnection(session);
    return 0;
}
