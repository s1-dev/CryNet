from scapy.all import sniff, ARP

def arpPacketCallback(packet):
    try:
        if packet.haslayer(ARP) and packet[ARP].op == 1:  # ARP Request
            src_ip = packet[ARP].psrc
            src_mac = packet[ARP].hwsrc
            print(f"Received ARP Request from {src_ip} ({src_mac})")
    except:
        print("Error reading packet")

def startArpListner(interface):
    sniff(iface=interface, filter="arp", prn=arpPacketCallback, store=0)

def main():
    # Replace interface
    interface = 'Wi-Fi'
    print(f"Listening for ARP Requests on {interface}...")
    startArpListner(interface)

if __name__ == "__main__":
    main()
