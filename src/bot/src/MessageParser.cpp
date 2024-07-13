#include "MessageParser.h"
#include "ActionType.hpp"

std::vector<std::string> buildParams(const char* firstParam, const char** otherParams, unsigned int paramCount) {
    std::vector<std::string> newParams;
    newParams.push_back(std::string(firstParam));

    for (unsigned int i = 0; i < paramCount; i++) {
        newParams.push_back(std::string(otherParams[i]));
    }

    return newParams;
}


ActionInfo MessageParser::parseMessage(const char* firstParam, const char** otherParams, unsigned int paramCount) {
    std::vector<std::string> messageParams = buildParams(firstParam, otherParams, paramCount);
    std::string strActionType = messageParams[0];
    ActionType actionType;
    if (strActionType == "PING") {
        actionType = ActionType::PING;
    } else if (strActionType == "ENCRYPT") {
        actionType = ActionType::ENCRYPT;
    } else {
        actionType = ActionType::UNKNOWN;
    }

    return ActionInfo(actionType, messageParams);
}
