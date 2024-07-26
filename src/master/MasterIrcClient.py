import socket
import threading
import hashlib
import queue

USER_INPUT = "Enter command: "
QUIT_MSG = "QUIT_NOW"

def computeSha256Hex(inputStr):
    encodedString = inputStr.encode("utf-8")
    sha256sum = hashlib.sha256()
    sha256sum.update(encodedString)
    return sha256sum.hexdigest().lower()

class MasterIrcClient:
    def __init__(self, server, port, validatePassHash, buffer, clearLine):
        self.server = server
        self.port = port
        self.validatePassHash = validatePassHash
        self.buffer = buffer
        self.clearLine = clearLine
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.running = False
        self.receivedDataBuffer = []
        self.bufferData = False
        self.inputQueue = queue.Queue()
        self.initialPromptSet = False

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
                        self.printServerResponse("\n" + response)
            except Exception as e:
                print(f"Error receiving message: {e}")
                self.running = False

    def flushBuffer(self):
        if self.bufferData:
            for response in self.receivedDataBuffer:
                self.printServerResponse(response)
            self.receivedDataBuffer = []

    def autoConnect(self):
        validatePass = input("Enter master validation passphrase: ")
        tempHexDigest = computeSha256Hex(validatePass)
        if tempHexDigest != self.validatePassHash:
            print("Incorrect validation passphrase, please ensure that you are entering in the same passphrase that is stored in the master config file.")
            return
        self.sendCommand(f"/VALIDATE {validatePass} MASTER")
        self.sendCommand(f"/NICK master")
        self.sendCommand(f"/USER masterUser 0 * :masterRealName")

    def connect(self, attemptAutoConnect):
        self.sock.connect((self.server, self.port))
        if attemptAutoConnect:
            self.autoConnect()

    def printServerResponse(self, response):
        if self.buffer.getLastLine() and self.buffer.getLastLine().startswith(USER_INPUT):
            self.clearLine()
        print(f"{response.strip()}")
        if not response.endswith("\n"):
            print()  # Add a newline if not present
        if self.initialPromptSet:
            print(USER_INPUT, end='', flush=True)  # Reprint the command prompt

    def userInputHandler(self):
        while self.running:
            if not self.initialPromptSet:
                userInput = input()
                self.inputQueue.put(userInput)
                continue
            userInput = input(USER_INPUT)
            self.inputQueue.put(userInput)

    def run(self):
        self.flushReceivedData()
        self.running = True

        receiveThread = threading.Thread(target=self.receive)
        receiveThread.daemon = True
        receiveThread.start()

        inputThread = threading.Thread(target=self.userInputHandler)
        inputThread.daemon = True
        inputThread.start()

        try:
            print(USER_INPUT, end='', flush=True)  # Initial prompt
            self.initialPromptSet = True
            while self.running:
                if not self.inputQueue.empty():
                    userInput = self.inputQueue.get()
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