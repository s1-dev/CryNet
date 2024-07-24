import threading

USER_INPUT = "CC Option: "

class CommandCenter:
    def __init__(self, ircClient):
        self.ircClient = ircClient
        self.autoFlush = False

    def listChannels(self):
        allChannels = input("List all channels? (Y/N) ")
        allChannels = allChannels == "y" or allChannels == "Y"
        if allChannels:
            self.ircClient.sendCommand("/LIST *")
        else:
            channelList = input("Enter a comma seperated list of channels (no spaces): ")
            self.ircClient.sendCommand(f"/LIST {channelList}")
        if self.autoFlush == False:
            self.ircClient.flushBuffer()

    def handleOption(self, option):
        if option == "1":
            print("case1")
        elif option == "2":
            print("case2")
        elif option == "3":
            print("gsdgd")
        elif option == "4":
            return False if not self.autoFlush else print("test")
        elif option == "5":
             return False if self.autoFlush else print("test")
        else:
            print("Invalid option")
        return True

    def displayOptions(self):
        optionsMenu = "~~~~~~~~~~~~~~~~~~~~~~~Command Center Options~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
        optionsMenu += "1. LIST channels\n"
        optionsMenu += "2. Show NAMES of all connected bots of specified channel(s)\n"
        optionsMenu += "3. Send command\n"
        if self.autoFlush:
            optionsMenu += "4. Flush server responses, if any\n"
            optionsMenu += "5. Exit command center\n"
        else:
            optionsMenu += "4. Exit command center\n"
        return optionsMenu


    def startCommandCenter(self):
        print("----------------------CryNet Command Center----------------------")
        print("Welcome to the Command Center. Seamless CryNet commands!")
        autoFlush = input("Would you like to auto-flush server responses to the terminal? (Y/N) ")
        autoFlush = autoFlush == "y" or autoFlush == "Y"
        self.autoFlush = autoFlush

        self.ircClient.running = True
        receiveThread = threading.Thread(target=self.ircClient.receive(not autoFlush, True)) # Change to specific receiving
        receiveThread.daemon = True
        receiveThread.start()
        try:
            while True:
                print(self.displayOptions())
                selectedOption = input(USER_INPUT)
                cont = self.handleOption(selectedOption)
                if not cont:
                    return
        except KeyboardInterrupt:
            self.ircClient.running = False
            self.ircClient.sendCommand("/QUIT")
            self.ircClient.sock.close()
            exit()

