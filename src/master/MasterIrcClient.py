import socket
import threading

USER_INPUT = "Enter command: "
QUIT_MSG = "QUIT_NOW"

class MasterIrcClient:
    def __init__(self, server, port, validatePass, buffer, clearLine):
        self.server = server
        self.port = port
        self.validatePass = validatePass
        self.buffer = buffer
        self.clearLine = clearLine
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.running = False
        self.receivedDataBuffer = []
        self.bufferData = False

    def isRunning(self):
        return self.running

    def sendCommand(self, command):
        self.sock.sendall((command + "\r\n").encode('utf-8'))

    def flushReceivedData(self):
        self.sock.setblocking(False)
        try:
            while True:
                response = self.sock.recv(4096).decode('utf-8')
                if response:
                    self.printServerResponse(response)
        except BlockingIOError:
            # No more data to read
            pass
        self.sock.setblocking(True)

    def receive(self, bufferData=False, isCommandCenter=False):
        self.bufferData = bufferData
        while self.running:
            try:
                response = self.sock.recv(4096).decode('utf-8')
                if response:
                    if self.bufferData:
                        self.receivedDataBuffer.append(response)
                    else:
                        if self.buffer.get_last_line() and self.buffer.get_last_line().startswith(USER_INPUT):
                            self.clearLine()
                        self.printServerResponse(response)
                        if not isCommandCenter:
                            print(USER_INPUT, end='')
            except Exception as e:
                print(f"Error receiving message: {e}")
                self.running = False
    
    def flushBuffer(self):
        if self.bufferData:
            for response in self.receivedDataBuffer:
                if self.buffer.get_last_line() and self.buffer.get_last_line().startswith(USER_INPUT):
                    self.clearLine()
                self.printServerResponse(response)
                print(USER_INPUT, end='')
            self.receivedDataBuffer = []

    def autoConnect(self):
        self.sendCommand(f"/VALIDATE {self.validatePass} MASTER")
        self.sendCommand(f"/NICK master")
        self.sendCommand(f"/USER masterUser 0 * :masterUser")

    def connect(self, attemptAutoConnect):
        self.sock.connect((self.server, self.port))
        if attemptAutoConnect:
            self.autoConnect()

    def printServerResponse(self, response):
        print(f"{response.strip()}") # Potentially add context to messages   

    def run(self):
        self.flushReceivedData()
        print(USER_INPUT, end='')
        self.running = True
        receiveThread = threading.Thread(target=self.receive)
        receiveThread.daemon = True
        receiveThread.start()
        try:
            while True:
                userInput = input()
                if userInput == QUIT_MSG:
                    self.running = False
                    print("Exiting IRC console...")
                    return
                self.sendCommand(userInput)
        except KeyboardInterrupt:
            self.running = False
            self.sendCommand("/QUIT")
            self.sock.close()
            exit()