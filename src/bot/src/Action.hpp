#ifndef ACTION_HPP
#define ACTION_HPP

#include "ActionType.hpp"

class Action {
public:
    virtual ~Action() {}
    virtual void execute() = 0;
    virtual ActionType getActionType() const = 0;
};

#endif // ACTION_HPP