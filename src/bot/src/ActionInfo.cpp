#include "ActionInfo.hpp"
#include "ActionType.hpp"

// Keeping like this incase extra logic is required in future development
ActionInfo::ActionInfo(ActionType actionType, std::vector<std::string> actionParams)
    : actionType(actionType), actionParams(actionParams) {}