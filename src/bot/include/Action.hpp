#ifndef ACTION_HPP
#define ACTION_HPP

#include "ActionType.hpp"
#include <string>

class Action {
public:
    virtual ~Action() {}
    virtual void execute() = 0;
    virtual ActionType getActionType() = 0;
    virtual std::string getMessage() = 0;
};

#endif // ACTION_HPP