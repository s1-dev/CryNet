#ifndef GENERALUTILS_H
#define GENERALUTILS_H

#include <string.h>
#include <string>

class GeneralUtils {
public:
    static bool cStrAreEqual(const char* cString, const char* otherString);
    static bool isNumber(const char* cString);

private:
    // Private constructor to prevent instantiation
    GeneralUtils() {}

};

#endif // GENERALUTILS_H
