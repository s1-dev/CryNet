import uuid
import sys
import socket
import threading
from colorama import init, Cursor

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

# END OF GLOBAL DECLARATIONS

class MasterIrcClient:
    def __init__(self, server, port, nickname, channel):
        self.server = server
        self.port = port
        self.nickname = nickname
        self.channel = channel
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.running = True
    
    def sendCommand(self, command):
        self.sock.sendall((command + "\r\n").encode('utf-8'))

    def receive(self):
        while self.running:
            response = self.sock.recv(4096).decode('utf-8')
            if response:
                # Clear the current line, print the message, and then reprint the user input prompt
                clearLine()
                print(response.strip())
                print(USER_INPUT, end='', flush=True)

    def autoConnect(self):
        self.sendCommand(f"/VALIDATE {VALIDATE_PASSPHRASE} MASTER")
        #self.sendCommand(f"/VALIDATE test123 BOT")
        self.sendCommand(f"/NICK {self.nickname}")
        self.sendCommand(f"/USER masterUser 0 * :masterUser")
        self.sendCommand(f"/JOIN #testChannel")

        return True

    def connect(self):
        self.sock.connect((self.server, self.port))
        autoConnected = self.autoConnect()

        if not autoConnected:
            print("Please address connection/validate/registration issues")

    def run(self):
        receiver_thread = threading.Thread(target=self.receive)
        receiver_thread.daemon = True  # Allows thread to exit when main program exits
        receiver_thread.start()

        try:
            while True:
                userInput = input(USER_INPUT)
                self.sendCommand(userInput)
        except KeyboardInterrupt:
            self.running = False
            self.sendCommand("/QUIT")
            self.sock.close()
            receiver_thread.join()

def main(): 
    serverHostname = "192.168.1.127"
    port = 6667  # Use 6667 for non-SSL, 6697 for SSL
    nickname = "Master"
    username = "MasterProgram" 
    realname = uuid.uuid4()  # TEMP, will probably use "realname for IRC client ID"

    client = MasterIrcClient(serverHostname, port, nickname, "#testChannel")
    client.connect()
    client.run()

if __name__ == "__main__":
    init()
    main()
