#ifndef GENDECRYPTIONSCRIPT_HPP
#define GENDECRYPTIONSCRIPT_HPP

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include <algorithm>
#include "GeneralUtils.hpp"

class GenDecryptionScript {
public:
    static void generateScript();
private:
    // Private constructor to prevent instantiation
    GenDecryptionScript() {}
};

#endif // GENDECRYPTIONSCRIPT_HPP
