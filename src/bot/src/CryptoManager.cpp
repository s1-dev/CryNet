#include "CryptoManager.hpp"

CryptoManager::CryptoManager(const char* algo) 
 : encAlgo(algo) {
    // TEMP: reading in encryption key until secure network transmission has been setup
    this->keyStr = "00112233445566778899AABBCCDDEEFF00112233445566778899AABBCCDDEEFF";
 }

SecByteBlock CryptoManager::hexToSecByteBlock(const std::string& hex) {
    SecByteBlock byteBlock(hex.size() / 2);
    HexDecoder decoder;
    decoder.Put((byte*)hex.data(), hex.size());
    decoder.MessageEnd();
    decoder.Get(byteBlock, byteBlock.size());
    return byteBlock;
}

void CryptoManager::encryptFile(const std::string& fileToEncrypt) { // change to string to pass info to irc client?
    std::string encryptedFile = fileToEncrypt + ".cry";
    SecByteBlock key;
    if (GeneralUtils::cStrAreEqual(encAlgo, "AES-128")) {
        key = hexToSecByteBlock(keyStr.substr(0, 32)); // 32 hex chars -> 128 bit
    } else if(GeneralUtils::cStrAreEqual(encAlgo, "AES-192")) {
        key = hexToSecByteBlock(keyStr.substr(0, 48)); // 48 hex chars -> 192 bit
    } else if(GeneralUtils::cStrAreEqual(encAlgo, "AES-256")) {
        key = hexToSecByteBlock(keyStr); // 64 chars -> 128 bit
    }
    aesCbcEncryptFile(fileToEncrypt, encryptedFile, key);
}


void CryptoManager::decryptFile(const std::string& fileToDecrypt) { // change to string to pass info to irc client?
    std::string decryptedFile = fileToDecrypt.substr(0, fileToDecrypt.length() - 4);
    SecByteBlock key;
    if (GeneralUtils::cStrAreEqual(encAlgo, "AES-128")) {
        key = hexToSecByteBlock(keyStr.substr(0, 32)); // 32 hex chars -> 128 bit
    } else if(GeneralUtils::cStrAreEqual(encAlgo, "AES-192")) {
        key = hexToSecByteBlock(keyStr.substr(0, 48)); // 48 hex chars -> 192 bit
    } else if(GeneralUtils::cStrAreEqual(encAlgo, "AES-256")) {
        key = hexToSecByteBlock(keyStr); // 64 chars -> 128 bit
    }
    aesCbcDecryptFile(fileToDecrypt, decryptedFile, key);
}


void CryptoManager::aesCbcEncryptFile(const std::string& fileToEncrypt, const std::string& encryptedFile, const SecByteBlock& key) {
    AutoSeededRandomPool prng;
    byte iv[AES::BLOCKSIZE];
    prng.GenerateBlock(iv, sizeof(iv));

    CBC_Mode<AES>::Encryption encryption;
    encryption.SetKeyWithIV(key, key.size(), iv);

    std::ifstream inputFile(fileToEncrypt, std::ios::binary);
    std::string plaintext((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    inputFile.close();

    std::string ciphertext;
    StringSource(plaintext, true,
        new StreamTransformationFilter(encryption,
            new StringSink(ciphertext)
        )
    );

    std::ofstream outputFile(encryptedFile, std::ios::binary);
    outputFile.write(reinterpret_cast<const char*>(iv), sizeof(iv));
    outputFile.write(ciphertext.data(), ciphertext.size());
    outputFile.close();

    changeFileOwnership(fileToEncrypt, encryptedFile);
}


void CryptoManager::aesCbcDecryptFile(const std::string& fileToDecrypt, const std::string& decryptedFile, const SecByteBlock& key) {
    std::ifstream inputFile(fileToDecrypt, std::ios::binary);
    byte iv[AES::BLOCKSIZE];
    inputFile.read(reinterpret_cast<char*>(iv), sizeof(iv));

    std::string ciphertext((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    inputFile.close();

    CBC_Mode<AES>::Decryption decryption;
    decryption.SetKeyWithIV(key, key.size(), iv);

    std::string decryptedtext;
    StringSource(ciphertext, true,
        new StreamTransformationFilter(decryption,
            new StringSink(decryptedtext)
        )
    );

    std::ofstream outputFile(decryptedFile, std::ios::binary);
    outputFile.write(decryptedtext.data(), decryptedtext.size());
    outputFile.close();

    changeFileOwnership(fileToDecrypt, decryptedFile);
}


void CryptoManager::changeFileOwnership(const std::string& sourceFile, const std::string& targetFile) {
    struct stat fileStat;
    if (stat(sourceFile.c_str(), &fileStat) != 0) {
        std::cerr << "Failed to retrieve file status for " << sourceFile << ": " << strerror(errno) << std::endl;
        return;
    }

    if (chown(targetFile.c_str(), fileStat.st_uid, fileStat.st_gid) != 0) {
        std::cerr << "Failed to change ownership of " << targetFile << ": " << strerror(errno) << std::endl;
    }
}
