#ifndef MESSAGEPARSER_H
#define MESSAGEPARSER_H

#include <string>
#include <vector>
#include <sstream>
#include <cstring>
#include "ActionInfo.hpp"

class MessageParser {
public:
    static ActionInfo parseMessage(const char* firstParam, const char** otherParams, unsigned int paramCount);

private:
    // Private constructor to prevent instantiation
    MessageParser() {}
};

#endif // MESSAGEPARSER_H
