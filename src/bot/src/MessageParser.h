#ifndef MESSAGEPARSER_H
#define MESSAGEPARSER_H

#include <string>
#include <vector>
#include <sstream>
#include <cstring>
#include "ActionInfo.cpp"

class MessageParser {
public:
    static ActionInfo parseMessage(const std::string& rawMessage);

private:
    // Private constructor to prevent instantiation
    MessageParser() {}
};

#endif // MESSAGEPARSER_H
