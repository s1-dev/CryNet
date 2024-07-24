#!/bin/bash

# Enable or disable features
ENABLE_PING_ACTION=1

# Initialize object file variables
PING_ACTION_OBJ=""
PACKET_LAUNCHER_OBJ=""

# Compile PingAction if enabled
if [ "$ENABLE_PING_ACTION" -eq 1 ]; then
    echo "Compiling PingAction.cpp"
    g++ -std=c++11 -Wall -Wextra -Iinclude -DENABLE_PING_ACTION -c PingAction.cpp -o PingAction.o
    PING_ACTION_OBJ="PingAction.o"
    echo "Compiling PacketLauncher.cpp"
    g++ -std=c++11 -Wall -Wextra -Iinclude -DENABLE_PACKET_LAUNCHER -c PacketLauncher.cpp -o PacketLauncher.o
    PACKET_LAUNCHER_OBJ="PacketLauncher.o"
fi

# Compile main source files with the same definitions
echo "Compiling main source files"
g++ -std=c++11 -Wall -Wextra -Iinclude -DENABLE_PING_ACTION -DENABLE_PACKET_LAUNCHER -c BotMain.cpp -o BotMain.o
g++ -std=c++11 -Wall -Wextra -Iinclude -DENABLE_PING_ACTION -DENABLE_PACKET_LAUNCHER -c IrcClient.cpp -o IrcClient.o
g++ -std=c++11 -Wall -Wextra -Iinclude -DENABLE_PING_ACTION -DENABLE_PACKET_LAUNCHER -c MessageParser.cpp -o MessageParser.o
g++ -std=c++11 -Wall -Wextra -Iinclude -DENABLE_PING_ACTION -DENABLE_PACKET_LAUNCHER -c GeneralUtils.cpp -o GeneralUtils.o

# Link object files
echo "Linking object files"
g++ BotMain.o IrcClient.o MessageParser.o GeneralUtils.o $PING_ACTION_OBJ $PACKET_LAUNCHER_OBJ -o irc_bot -lircclient -ltins -lpcap -s

echo "Build finished"
