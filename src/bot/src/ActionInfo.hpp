#ifndef Action_HPP
#define Action_HPP

#include "ActionType.hpp"
#include <string>
#include <vector>

class ActionInfo {
public:
    ActionInfo(ActionType actionType,  std::vector<std::string> actionParams);

private:
    ActionType actionType;
    std::vector<std::string> actionParams;
};

#endif // ACTIONINFO_HPP
