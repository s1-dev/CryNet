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