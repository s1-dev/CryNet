#include "EncryptAction.hpp"

bool isValidAlgorithm(const char* cString) {
    if (GeneralUtils::cStrAreEqual(cString, "AES-192")) {
        return true;
    } else if(GeneralUtils::cStrAreEqual(cString, "AES-256")) {
        return true;
    }
    return false;
}

EncryptAction::EncryptAction(std::vector<std::string> actionParams) 
    : actionParams(actionParams) {
    this->checkParams();
}

ActionType EncryptAction::getActionType() {
    return ActionType::ENCRYPT;
}

void EncryptAction::execute() { 
    if (!isValid) {
        printf("isValid DID NOT PASS!\n");
        return;
    }
    const char* algo = actionParams[ALGO_IDX].c_str();
    
}

/*
    ENCRYPT Command Syntax:
        ENCRYPT <ENC_ALGORITHM>
*/
void EncryptAction::checkParams() {
    if (actionParams.size() - 3 != PARAM_COUNT) {
        isValid = false;
        return;
    }

    if (!GeneralUtils::cStrAreEqual(actionParams[COMMAND_TYPE_IDX].c_str(), "ENCRYPT")) {
        isValid = false;
        return;
    }

    if(!isValidAlgorithm(actionParams[ALGO_IDX].c_str())) {
        isValid = false;
        return;
    }

    isValid = true;
}
