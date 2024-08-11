import json
from scapy.all import sniff, IP
from datetime import datetime

# Configuration
FILTER_ADDRESSES = ["192.168.1.122", "192.168.1.131", "192.168.1.105"]  # Add the IP addresses you want to filter
CAPTURE_DURATION = 60  # Duration of packet capture in seconds
OUTPUT_FILE = "filtered_packets.json"

# Initialize data list
packet_data = []

def packet_callback(packet):
    if IP in packet:
        ip_src = packet[IP].src
        ip_dst = packet[IP].dst
        
        # Check if the packet matches the filter addresses
        if ip_src in FILTER_ADDRESSES or ip_dst in FILTER_ADDRESSES:
            packet_info = {
                "time": datetime.now().timestamp(),
                "packet_size": len(packet)
            }
            packet_data.append(packet_info)

# Start packet capture
print(f"Capturing packets for {CAPTURE_DURATION} seconds...")
sniff(prn=packet_callback, timeout=CAPTURE_DURATION, filter="ip", store=0)

# Save data to JSON file
with open(OUTPUT_FILE, 'w') as f:
    json.dump(packet_data, f, indent=4)

print(f"Packet capture complete. Data saved to {OUTPUT_FILE}")
