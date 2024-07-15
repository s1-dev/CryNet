#ifndef PINGACTION_HPP
#define PINGACTION_HPP

#include <string>
#include "Action.hpp"
#include "PacketLauncher.hpp"

class PingAction : public Action {
public:
    PingAction(int targetPort, bool isIndefinite, int nPackets);

    void execute() override;
    ActionType getActionType() override;

private:
    //PacketLauncher packetLauncher;
    int targetPort;
    //std::string targetAddress;  
    bool isIndefinite;
    int nPackets;
};

#endif // PINGACTION_HPP
