# CryNet

Initial plan:

* 3 major components: IRC client/bot, IRC server, master program

*bot*:
-Written in C++
-Bots generated/compiled from master program 
-Authenticates and communicates with IRC server
-Does actions based on server commands
-...

*ircserver*:
-Written in Java, maven project
-Minimal IRC server
-Master program is operator and sends commands to bots
-Bots report result of action and server logs
-...

*master program*
-Written in Python 
-Sends commands to IRC server for bots to parse
-Reads logs from IRC server
-Generates/compiles bots with user customization
-...
