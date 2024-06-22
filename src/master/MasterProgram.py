import uuid
import sys
import socket
from colorama import init, Cursor

USER_INPUT = "Enter command: "
VALIDATE_PASSPHRASE = "test" # TODO read in from config

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

# END OF GLOBAL DECLARATIONS

class MasterIrcClient:
    def __init__(self, server, port, nickname, channel):
        self.server = server
        self.port = port
        self.nickname = nickname
        self.channel = channel
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    def sendCommand(self, command):
        self.sock.sendall((command + "\r\n").encode('utf-8'))

    def receive(self):
        # This check is to remove the line asking for user input if a command is received
        if (buffer.get_last_line()[0:len(USER_INPUT)] == USER_INPUT):
            clearLine() # probably change/remove in order to prevent incoming messages ruining user input
        response = self.sock.recv(4096).decode('utf-8')
        return response

    def autoConnect(self):
        self.sendCommand(f"/VALIDATE {VALIDATE_PASSPHRASE} MASTER")
        self.sendCommand(f"/NICK {self.nickname}")
        self.sendCommand(f"/USER {self.username} 0 * :{self.realname}")

    def connect(self):
        self.sock.connect((self.server, self.port))
        autoConnected = self.autoConnect()

        if autoConnected == False:
            print("Please address connection/validate/registration issues")

    def run(self): # TODO make thread to receive while getting user input
        try:
            while True:
                userInput = input(USER_INPUT)
                self.sendCommand(userInput)
                print(self.receive().strip())

        except KeyboardInterrupt:
            self.sendCommand("/QUIT")
            self.sock.close()


    



def main(): 
    serverHostname = "127.0.0.1"
    port = 6667  # Use 6667 for non-SSL, 6697 for SSL
    nickname = "Master"
    username = "MasterProgram" 
    realname = uuid.uuid4() # TEMP, will probably use "realname for IRC client ID"

    client = MasterIrcClient(serverHostname, port, nickname, username, realname)
    client.connect()
    client.run()

  
if __name__=="__main__":
    init() 
    main() 