#ifndef PINGACTION_HPP
#define PINGACTION_HPP

#include <string>
#include <cstdio>
#include <vector>
#include <cstdlib>
#include "Action.hpp"
#include "PacketLauncher.hpp"
#include "GeneralUtils.hpp"

class PingAction : public Action {
public:
    PingAction(std::vector<std::string> actionParams);

    void execute() override;
    ActionType getActionType() override;
    std::string getMessage() override;

private:
    std::vector<std::string> actionParams;
    bool isValid;

    const int PARAM_COUNT = 5;
    const int COMMAND_TYPE_IDX = 3;
    const int ALGO_IDX = 4;
    const int TARGET_URI_IDX = 5;
    const int TARGET_PORT_IDX = 6;
    const int ATTACK_CAP_IDX = 7;

    void checkParams();
};

#endif // PINGACTION_HPP
