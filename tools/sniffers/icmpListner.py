from scapy.all import sniff, ICMP, IP

def icmpPacketCallback(packet):
    if packet.haslayer(ICMP):
        icmp_type = packet[ICMP].type
        if icmp_type == 8:  # Echo request
            print(f"Received ICMP Echo Request from {packet[IP].src} to {packet[IP].dst}")
        elif icmp_type == 0:  # Echo reply
            print(f"Received ICMP Echo Reply from {packet[IP].src} to {packet[IP].dst}")

def startIcmpListner(interface):
    sniff(iface=interface, filter="icmp", prn=icmpPacketCallback, store=0)

if __name__ == "__main__":
    # Replace interface
    interface = 'Wi-Fi'
    print(f"Listening for ICMP Echo Requests and Replies on {interface}...")
    startIcmpListner(interface)
