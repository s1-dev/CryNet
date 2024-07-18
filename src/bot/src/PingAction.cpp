#include "PingAction.hpp"

#include <string.h>

bool strCompare(const char* cString, const char* otherString) { // put string comp in utils file
    return strcmp(cString, otherString) == 0;
}

bool isNumber(const char* cString) {
    if (cString == nullptr || *cString == '\0') {
        return false;
    }

    while (*cString != '\0') {
        if (!std::isdigit(*cString)) {
            return false;
        }
        ++cString;
    }

    return true;
}

bool isValidAlgorithm(const char* cString) {
    if (strCompare(cString, "TCP-SYN")) {
        return true;
    } else if (strCompare(cString, "TCP-ACK")) {
        return true;
    } else if (strCompare(cString, "TCP-SYN-ACK")) {
        return true;
    } else if (strCompare(cString, "TCP-XMAS")) {
        return true;
    } else if (strCompare(cString, "UDP")) {
        return true;
    } else if (strCompare(cString, "ICMP-REQ")) {
        return true;
    } else if (strCompare(cString, "ICMP-REP")) {
        return true;
    } else if (strCompare(cString, "DNS")) {
        return true;
    } else if (strCompare(cString, "ARP-REQ")) {
        return true;
    } /*else if (strCompare(cString, "HTTP-GET")) {
         return true;
    } else if (strCompare(cString, "HTTP-POST")) {
        return true;
    } */  // to be completed!
    return false;
}

PingAction::PingAction(std::vector<std::string> actionParams) 
    : actionParams(actionParams) {
    this->checkParams();
}


ActionType PingAction::getActionType() {
    return ActionType::PING;
}


void PingAction::execute() {
    if (!isValid) {
        printf("isValid DID NOT PASS!\n");
        return;
    }
    const char* targetUri = actionParams[TARGET_URI_IDX].c_str();
    const int targetPort = atoi(actionParams[TARGET_PORT_IDX].c_str());
    const char* algo = actionParams[ALGO_IDX].c_str();

    // send info to PacketLauncher

    if (strCompare(actionParams[ATTACK_CAP_IDX].c_str(), "LOOP")) {
        while (true) { // ping until external forces act
            return; // temp
        }
        return;
    }

    const int nPackets = atoi(actionParams[ATTACK_CAP_IDX].c_str());

    for (int i = 0; i < nPackets; i++) {

    }


    printf("isValid passed!\n");
}


/*
    PING Command Syntax:
        PING <PROTOCOL/PING_TYPE> <TARGET_URI> <TARGET_PORT> <N-PACKETS|LOOP>
*/
void PingAction::checkParams() {
    if (actionParams.size() - 3 != PARAM_COUNT) {
        isValid = false;
        return;
    }

    if (!strCompare(actionParams[COMMAND_TYPE_IDX].c_str(), "PING")) {
        isValid = false;
        return;
    }

    if(!isValidAlgorithm(actionParams[ALGO_IDX].c_str())) {
        isValid = false;
        return;
    }

    if (!isNumber(actionParams[TARGET_PORT_IDX].c_str())) {
        isValid = false;
        return;
    }

    if (!strCompare(actionParams[ATTACK_CAP_IDX].c_str(), "LOOP") && !isNumber(actionParams[ATTACK_CAP_IDX].c_str())) {
        isValid = false;
        return;
    }

    isValid = true;
}