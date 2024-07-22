#ifndef MESSAGEPARSER_H
#define MESSAGEPARSER_H

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cstring>
#include "GeneralUtils.hpp"
#include "ActionInfo.hpp"

class MessageParser {
public:
    static ActionInfo parseMessage(std::string receivedMessage, const char* expectedSender);

private:
    // Private constructor to prevent instantiation
    MessageParser() {}

    static const int MIN_PARAMS = 3;
    static const int SENDER_IDX = 0;
    static const int MSG_TYPE_IDX = 1;
    static const int ACTION_TYPE_IDX = 3;
};

#endif // MESSAGEPARSER_H
