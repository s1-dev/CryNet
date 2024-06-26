import socket
import ssl

class SimpleIRCClient:
    def __init__(self, server, port, nickname, username, realname, use_ssl=False):
        self.server = server
        self.port = port
        self.nickname = nickname
        self.username = username
        self.realname = realname
        self.use_ssl = use_ssl
        self.sock = None

    def connect(self):
        raw_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        if self.use_ssl:
            self.sock = ssl.wrap_socket(raw_socket)
        else:
            self.sock = raw_socket

        self.sock.connect((self.server, self.port))
        #self.send_command(f"NICK {self.nickname}")
        #self.send_command(f"USER {self.username} 0 * :{self.realname}")

    def send_command(self, command):
        self.sock.sendall((command + "\r\n").encode('utf-8'))

    def receive(self):
        response = self.sock.recv(4096).decode('utf-8')
        return response

    def run(self):
        try:
            while True:
                test123 = input("Input: ")
                self.send_command(test123)
                response = self.receive()
                print(response.strip())
                if "PING" in response:
                    self.send_command("PONG " + response.split()[1])
        except KeyboardInterrupt:
            self.send_command("QUIT")
            self.sock.close()

if __name__ == "__main__":
    server = "127.0.0.1"
    port = 6667  # Use 6667 for non-SSL, 6697 for SSL
    nickname = "testnick"
    username = "testuser"
    realname = "Test User"

    client = SimpleIRCClient(server, port, nickname, username, realname)
    client.connect()
    client.run()
