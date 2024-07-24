#ifndef FILESYSTEMDECRYPTOR_HPP
#define FILESYSTEMDECRYPTOR_HPP

#include <string>
#include <cstdio>
#include <vector>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <sstream> 

#include "CryptoManager.hpp"

namespace fs = std::filesystem;

class FileSystemDecryptor {
public:
    static void decryptFileSystem(const char* exeName);

private:
    // Private constructor to prevent instantiation
    FileSystemDecryptor() {}
};

#endif // FILESYSTEMDECRYPTOR_HPP
