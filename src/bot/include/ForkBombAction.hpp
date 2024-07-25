#ifndef FORKBOMBACTION_HPP
#define FORKBOMBACTION_HPP

#include "Action.hpp"
#include <vector>
#include <cstdlib>
#include <fstream>
#include <unistd.h>
#include "GeneralUtils.hpp"

class ForkBombAction : public Action {
public:
    ForkBombAction(const std::vector<std::string>& actionParams);

    void execute() override;
    ActionType getActionType() override;
    std::string getMessage() override;

private:
    bool isValid;
    const int PARAM_COUNT = 2;
    const int COMMAND_TYPE_IDX = 3;
    const int FORK_TYPE_IDX = 4;
    std::vector<std::string> actionParams;

    void cForkBomb();
    void bashForkBomb();


    void checkParams();
};

#endif // FORKBOMBACTION_HPP
