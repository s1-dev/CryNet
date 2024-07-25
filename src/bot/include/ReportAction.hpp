#ifndef REPORTACTION_HPP
#define REPORTACTION_HPP

#include "GeneralUtils.hpp"
#include "Action.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <sys/utsname.h>

class ReportAction : public Action {
public:
    ReportAction(const std::vector<std::string>& actionParams);

    void execute() override;
    ActionType getActionType() override;
    std::string getMessage() override;

private:
    bool isValid;
    const int PARAM_COUNT = 1;
    const int COMMAND_TYPE_IDX = 3;
    std::vector<std::string> actionParams;

    std::string msgToServer;
    std::string getHostname();
    std::string getOSInfo();
    std::string getCPUInfo();
    std::string getMemoryInfo();

    void checkParams();
};

#endif // REPORTACTION_HPP
