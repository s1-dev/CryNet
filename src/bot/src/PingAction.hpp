#ifndef PINGACTION_HPP
#define PINGACTION_HPP

#include "Action.hpp"

class PingAction : public Action {
public:
    PingAction();
    void execute();

    ActionType getActionType();
};

#endif // PINGACTION_HPP
