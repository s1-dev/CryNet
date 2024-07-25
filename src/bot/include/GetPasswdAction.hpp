#ifndef GETPASSWDACTION_HPP
#define GETPASSWDACTION_HPP

#include "Action.hpp"
#include <vector>
#include <fstream>
#include <string>
#include "GeneralUtils.hpp"

class GetPasswdAction : public Action {
public:
    GetPasswdAction(const std::vector<std::string>& actionParams);

    void execute() override;
    ActionType getActionType() override;
    std::string getMessage() override;

private:
    bool isValid;
    const int PARAM_COUNT = 2;
    const int COMMAND_TYPE_IDX = 3;
    const int FILE_TYPE_IDX = 4;
    std::vector<std::string> actionParams;
    std::string msgToServer;

    std::string getFileContents(std::string& filePath);

    void checkParams();
};

#endif // GETPASSWDACTION_HPP
