#ifndef IRCCLIENT_H
#define IRCCLIENT_H

#include <libircclient.h>
#include <string>

class IrcClient {
public:
    IrcClient(const char* server, int port);
    ~IrcClient();
    void connect();
    void sendCommand(const std::string& command);

    static void signalHandler(int signal);

private:
    bool registrationStatus;
    const char* server;
    int port;
    irc_session_t* session;
    irc_callbacks_t callbacks;

    void closeConnection();
    static std::string concatenateParams(const char* firstParam, const char** params, unsigned int count);
    bool isRegistered();

    static void event_connect(irc_session_t* session, const char* event, const char* origin, const char** params, unsigned int count);
    static void event_unknown(irc_session_t* session, const char* event, const char* origin, const char** params, unsigned int count);

    static IrcClient* getInstance(irc_session_t* session);
};

#endif // IRCCLIENT_H
