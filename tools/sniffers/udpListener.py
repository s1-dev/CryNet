import socket

# Configuration
LISTEN_IP = "0.0.0.0"  # Listen on all interfaces
LISTEN_PORT = 9999     # Port to listen on

def startUdpListner(sock):
    print(f"Listening on {LISTEN_IP}:{LISTEN_PORT}...")
    while True:
        data, addr = sock.recvfrom(4096) 
        print(f"Received packet from {addr}: {data}")

def main():
    # Create a UDP socket
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((LISTEN_IP, LISTEN_PORT))
    startUdpListner(sock)

if __name__ == "__main__":
    main()