import json
import time
from scapy.all import sniff
from collections import defaultdict

def packet_counter(pkt, packet_counts, start_time):
    current_time = int(time.time() - start_time)
    packet_counts[current_time] += 1

def capture_packets(duration, iface):
    packet_counts = defaultdict(int)
    start_time = time.time()

    sniff(prn=lambda pkt: packet_counter(pkt, packet_counts, start_time), timeout=duration, iface=iface)

    return packet_counts

def save_packet_counts_to_json(packet_counts, filename):
    with open(filename, 'w') as json_file:
        json.dump(packet_counts, json_file)

def main():
    duration = 60  
    interface = 'Wi-Fi'  
    num_bots = input("Num bots: ")  
    print(f"Capturing packets for {num_bots} bot(s) pinging...")
    packet_counts = capture_packets(duration, interface)
    
    json_filename = f'packet_counts_{num_bots}_bots.json'
    save_packet_counts_to_json(packet_counts, json_filename)
    print(f"Packet count data saved to {json_filename}")

if __name__ == '__main__':
    main()
