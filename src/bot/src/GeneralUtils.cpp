#include "GeneralUtils.hpp"

bool GeneralUtils::cStrAreEqual(const char* cString, const char* otherString) {
    return strcmp(cString, otherString) == 0; 
}

bool GeneralUtils::isNumber(const char* cString) {
    if (cString == nullptr || *cString == '\0') {
        return false;
    }

    while (*cString != '\0') {
        if (!std::isdigit(*cString)) {
            return false;
        }
        ++cString;
    }

    return true;
}

const char* GeneralUtils::getFileBaseName(const std::string& filePath) {
    std::string fullPath = filePath;
    char* pathCopy = new char[fullPath.length() + 1];
    strcpy(pathCopy, fullPath.c_str());
    char* baseName = basename(pathCopy);
    std::string baseNameStr(baseName);
    delete[] pathCopy; 
    return strdup(baseNameStr.c_str()); 
}

std::vector<std::string> GeneralUtils::splitByNewLine(const std::string& input) {
    std::vector<std::string> lines;
    std::stringstream ss(input);
    std::string line;
    
    while (std::getline(ss, line)) {
        lines.push_back(line);
    }
    
    return lines;
}

std::string GeneralUtils::generateRandomAlphanumericString(size_t length) {
    const std::string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, chars.size() - 1);

    std::string randomString;
    for (size_t i = 0; i < length; ++i) {
        randomString += chars[dis(gen)];
    }
    return randomString;
}