from scapy.all import sniff, TCP, IP

def tcpPacketCallback(packet):
    if packet.haslayer(TCP):
        flags = packet[TCP].flags
        if 'S' in flags or 'A' in flags: # Modify flags to capture
            print(f"Received TCP packet from {packet[IP].src}:{packet[TCP].sport} to {packet[IP].dst}:{packet[TCP].dport} with flags {flags}")

def startTcpListener(interface):
    sniff(iface=interface, filter="tcp", prn=tcpPacketCallback, store=0)

def main():
    # Replace interface
    interface = 'Wi-Fi'
    print(f"Listening for TCP SYN/ACK packets on {interface}...")
    startTcpListener(interface)


if __name__ == "__main__":
    main()
