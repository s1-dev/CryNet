#include "IrcClient.hpp"

#include <sstream>
#include <iostream>
#include <cstring>
#include <csignal>

IrcClient::IrcClient(const char* server, int port)
    : server(server), port(port), session(nullptr) {
    memset(&callbacks, 0, sizeof(callbacks));
    callbacks.event_connect = event_connect;
    callbacks.event_quit = dump_event;
    callbacks.event_kick = dump_event;
    callbacks.event_topic = dump_event;
    callbacks.event_part = dump_event;
    callbacks.event_privmsg = dump_event;
    callbacks.event_unknown = event_unknown;
    registrationStatus = false;

    session = irc_create_session(&callbacks);
    if (!session) {
        throw std::runtime_error("Could not create IRC session");
    }
    irc_set_ctx(session, this);
}

IrcClient* IrcClient::getInstance(irc_session_t* session) {
    return static_cast<IrcClient*>(irc_get_ctx(session));
}

IrcClient::~IrcClient() {
    closeConnection();
}

void IrcClient::connect() {
    const char* placeholder = "IGNORE";
    if (irc_connect(session, server, port, nullptr, placeholder, placeholder, placeholder)) {
        closeConnection();
        throw std::runtime_error("Failed to connect to server");
    }

    if (irc_run(session)) {
        closeConnection();
        throw std::runtime_error("Error while connected to server");
    }
}

void IrcClient::signalHandler(int signal) {
    std::cout << "Interrupt signal (" << signal << ") received.\n";
    exit(signal);
}

std::string IrcClient::concatenateParams(const char* firstParam, const char** params, unsigned int count) {
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

bool IrcClient::isRegistered() {
    return registrationStatus;
}

void IrcClient::sendCommand(const std::string& command) {
    if (session) {
        irc_send_raw(session, command.c_str());
    }
}

void IrcClient::closeConnection() {
    if (session) {
        irc_disconnect(session);
        irc_destroy_session(session);
        session = nullptr;
    }
}

void IrcClient::event_connect(irc_session_t* session, const char* event, const char* origin, const char** params, unsigned int count) {
    printf("Connected to server\n");

    IrcClient* instance = getInstance(session);
    if (instance) {
        instance->sendCommand("/VALIDATE test123 BOT");
        instance->sendCommand("/NICK bot");
        instance->sendCommand("/USER botNick 0 * botUser");
        instance->sendCommand("/JOIN #testChannel");
        instance->sendCommand("/LIST *");
        instance->sendCommand("/NAMES #testChannel");
    }
}

void IrcClient::event_unknown(irc_session_t* session, const char* event, const char* origin, const char** params, unsigned int count) {
    IrcClient::parseEvent(session, event, origin, params, count);
}

void IrcClient::dump_event(irc_session_t* session, const char* event, const char* origin, const char** params, unsigned int count) {
	IrcClient::parseEvent(session, event, origin, params, count);
}
void IrcClient::parseEvent(irc_session_t* session, const char* event, const char* origin, const char** params, unsigned int count) {
    IrcClient* instance = getInstance(session);
    if (!instance) {
        return;
    }

    std::string receivedMessage;
    if (event != nullptr) {
        receivedMessage = IrcClient::concatenateParams(event, params, count);
    } else {
        receivedMessage = IrcClient::concatenateParams("", params, count);
    }

    if (strcmp(event, "(REGISTERED)") == 0) {
        instance->registrationStatus = true;
    }

    if (!instance->isRegistered()) {
        return;
    }
    
    ActionInfo actionInfo = MessageParser::parseMessage(event, params, count);
    // Create Action
    Action* action = nullptr;
    // temp test
    action = new PingAction(42, false, 42);
    /*if (actionInfo.getActionType() == ActionType::PING) { // check if bot was compiled to support this
        action = new PingAction(42, std::string("test"), false, 42);
    }*/

    if (action) {
        action->execute();
        delete action;
    }
    //createAction(actionInfo);
}

