#ifndef CRYPTOMANAGER_HPP
#define CRYPTOMANAGER_HPP

#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/osrng.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>

#include <filesystem>
#include <fstream>
#include <string>

#include "GeneralUtils.hpp"

using namespace CryptoPP;

class CryptoManager {
public:
    CryptoManager(const char* algo);

    void encryptFile(const std::string& fileToEncrypt);

    void decryptFile(const std::string& fileToDecrypt);

private:
    const std::string ENC_FILE_EXT = ".cry";
    bool alreadyEncrypted;
    const char* encAlgo;
    const std::string keyStr;


    SecByteBlock hexToSecByteBlock(const std::string& hex);
    void aesCbcEncryptFile(const std::string& fileToEncrypt, const std::string& encryptedFile, const SecByteBlock& key);
    void aesCbcDecryptFile(const std::string& fileToDecrypt, const std::string& decryptedFile, const SecByteBlock& key);
};

#endif // CRYPTOMANAGER_HPP
