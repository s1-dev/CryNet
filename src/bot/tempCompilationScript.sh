
g++ -std=c++11 -Wall -Wextra -Iinclude -c BotMain.cpp -o BotMain.o
g++ -std=c++11 -Wall -Wextra -Iinclude -c IrcClient.cpp -o IrcClient.o
g++ -std=c++11 -Wall -Wextra -Iinclude -c MessageParser.cpp -o MessageParser.o
g++ -std=c++11 -Wall -Wextra -Iinclude -c PingAction.cpp -o PingAction.o

g++ BotMain.o IrcClient.o MessageParser.o -o irc_bot -lircclient
