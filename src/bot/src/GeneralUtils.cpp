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