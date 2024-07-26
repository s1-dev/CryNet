from scapy.all import sniff, DNS, DNSQR, IP

def dnsPacketCallback(packet):
    try:
        if packet.haslayer(DNS) and packet.getlayer(DNS).qr == 0:  # DNS query
            queried_domain = packet[DNSQR].qname.decode('utf-8')
            print(f"Received DNS Query for {queried_domain} from {packet[IP].src}")
    except:
        print("Error reading packet")

def startDnsListner(interface):
    sniff(iface=interface, filter="udp port 53", prn=dnsPacketCallback, store=0)

def main():
    # Replace interface
    interface = 'Wi-Fi'
    print(f"Listening for DNS Queries on {interface}...")
    startDnsListner(interface)

if __name__ == "__main__":
    main()