#include "MessageParser.h"
#include "ActionInfo.hpp"
#include "ActionType.hpp"

std::vector<std::string> splitMessage(const std::string &message) {
        std::vector<std::string> result;
    std::istringstream stream(message);
    std::string word;

    while (stream >> word) { 
        result.push_back(word);
    }

    return result;
}


ActionInfo MessageParser::parseMessage(const std::string& rawMessage) {
    std::vector<std::string> messageParams = splitMessage(rawMessage);
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
