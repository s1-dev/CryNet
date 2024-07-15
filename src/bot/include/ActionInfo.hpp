#ifndef ACTION_INFO_HPP
#define ACTION_INFO_HPP

#include "ActionType.hpp"
#include <string>
#include <vector>

class ActionInfo {
public:
    // Constructor
    ActionInfo(ActionType actionType, const std::vector<std::string>& actionParams)
        : actionType(actionType), actionParams(actionParams) {}

    // Getter methods
    ActionType getActionType() const { return actionType; }
    const std::vector<std::string>& getActionParams() const { return actionParams; }

private:
    ActionType actionType;
    std::vector<std::string> actionParams;
};

#endif // ACTION_INFO_HPP
