#include "ReportAction.hpp"

ReportAction::ReportAction(const std::vector<std::string>& actionParams) 
    : actionParams(actionParams) {
    this->msgToServer = "";
    this->checkParams();
}

void ReportAction::execute() {
    if (!isValid) {
        printf("isnt valid\n");
        return;
    }
    std::string result;
    result += "Host Information Report: "; 
    result += "Hostname: " + getHostname() + "\n";
    result += "OS Info: " + getOSInfo() + "\n";
    result += "CPU Info: " + getCPUInfo() + "\n";
    result += "Memory Info: " + getMemoryInfo() + "\n";

    this->msgToServer = result;
}

ActionType ReportAction::getActionType() {
    return ActionType::REPORT;
}

std::string ReportAction::getMessage() {
    return msgToServer;
}

std::string ReportAction::getHostname() {
    char hostname[256];
    gethostname(hostname, sizeof(hostname));
    return std::string(hostname);
}

std::string ReportAction::getOSInfo() {
    struct utsname buffer;
    if (uname(&buffer) != 0) {
        return "Unknown";
    }
    std::ostringstream os;
    os << buffer.sysname << " " << buffer.release << " " << buffer.version;
    return os.str();
}

std::string ReportAction::getCPUInfo() {
    std::ifstream cpuinfo("/proc/cpuinfo");
    std::string line;
    std::ostringstream cpu;
    if (cpuinfo.is_open()) {
        while (getline(cpuinfo, line)) {
            if (line.find("model name") != std::string::npos) {
                cpu << line << std::endl;
                break;
            }
        }
        cpuinfo.close();
    } else {
        return "Cannot read /proc/cpuinfo";
    }
    return cpu.str();
}

std::string ReportAction::getMemoryInfo() {
    std::ifstream meminfo("/proc/meminfo");
    std::string line;
    std::ostringstream mem;
    if (meminfo.is_open()) {
        while (getline(meminfo, line)) {
            if (line.find("MemTotal") != std::string::npos) {
                mem << line << std::endl;
            } else if (line.find("MemFree") != std::string::npos) {
                mem << line << std::endl;
                break;
            }
        }
        meminfo.close();
    } else {
        return "Cannot read /proc/meminfo";
    }
    return mem.str();
}


/*
    REPORT Command Syntax:
        REPORT
*/
void ReportAction::checkParams() {
    if (actionParams.size() - 3 != PARAM_COUNT) {
        isValid = false;
        return;
    }

    if (!GeneralUtils::cStrAreEqual(actionParams[COMMAND_TYPE_IDX].c_str(), "REPORT")) {
        isValid = false;
        return;
    }

    isValid = true;
}