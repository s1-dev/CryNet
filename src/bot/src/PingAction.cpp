#include "PingAction.hpp"
#include <cstdio>

PingAction::PingAction(int targetPort, bool isIndefinite, int nPackets) 
    : targetPort(targetPort), isIndefinite(isIndefinite), nPackets(nPackets) {
        // instantiate packet launcher

}


ActionType PingAction::getActionType() {
    return ActionType::PING;
}


void PingAction::execute() {
    printf("testing\n");
}