#ifndef GENERALUTILS_H
#define GENERALUTILS_H

#include <string.h>
#include <cstring>
#include <string>
#include <csignal>
#include <iostream>
#include <sstream>
#include <vector>
#include <unistd.h>
#include "libgen.h"

class GeneralUtils {
public:
    static bool cStrAreEqual(const char* cString, const char* otherString);
    static bool isNumber(const char* cString);
    static const char* getFileBaseName(const std::string& filePath);
    static std::vector<std::string> splitByNewLine(const std::string& input);

private:
    // Private constructor to prevent instantiation
    GeneralUtils() {}

};

#endif // GENERALUTILS_H
