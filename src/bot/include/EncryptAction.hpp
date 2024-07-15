#ifndef ENCRYPTACTION_HPP
#define ENCRYPTACTION_HPP

#include "Action.hpp"

class EncryptAction : public Action {
public:
    EncryptAction();
    void execute();

    ActionType getActionType();
};

#endif // ENCRYPTACTION_HPP
