import socket
import threading

class IRCClient:
    def __init__(self, server, port, isMaster):
        self.server = server
        self.port = port
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        if isMaster == "MASTER":
            self.nickname = "MASTER_NICK"
            self.username = "MASTER_UNAME"
            self.realname = "MASTER_RNAME"
            self.passphrase = "test"
            self.identifier = "MASTER"
        else:
            self.nickname = "BOT_NICK"
            self.username = "BOT_UNAME"
            self.realname = "BOT_RNAME"
            self.passphrase = "test123"
            self.identifier = "BOT"
        self.channel = "#testChannel"
    
    def connect(self):
        self.sock.connect((self.server, self.port))
        self.send_command(f"/VALIDATE {self.passphrase} {self.identifier}")
        self.send_command(f"/NICK {self.nickname}")
        self.send_command(f"/USER {self.username} 0 * :{self.realname}")
        self.send_command(f"/JOIN {self.channel}")
        
        # Start a thread to receive messages
        threading.Thread(target=self.receive_messages, daemon=True).start()
        
        print(f"Connected to {self.server} on port {self.port} as {self.nickname}")
    
    def send_command(self, command):
        self.sock.sendall(f"{command}\r\n".encode('utf-8'))
    
    def receive_messages(self):
        while True:
            response = self.sock.recv(4096).decode('utf-8')
            if response.startswith("PING"):
                self.send_command(f"PONG {response.split()[1]}")
            else:
                print(response)
    
    def start(self):
        self.connect()
        while True:
            command = input("Enter command: ")
            self.send_command(command)

if __name__ == "__main__":
    server = "127.0.0.1"
    port = 6667  # Use 6667 for non-SSL, 6697 for SSL

    botOrMaster = input("bot or master: ")

    client = IRCClient(server, port, botOrMaster)
    client.start()
