#include <iostream>
#include <tins/tins.h>

using namespace Tins;

int main() {
    // Destination IPv4 address and port
    IPv4Address dst_ip("10.17.20.35");
    uint16_t dst_port = 12345;

    // Create a UDP packet
    UDP udp = UDP(dst_port, dst_port);

    // Set the payload data (optional)
    const char* payload = "Hello, world!";
    udp.inner_pdu(PDU::from_data(payload, std::strlen(payload)));

    // Create an IP packet with the UDP payload, destination IP address, and TTL value
    IP ip = IP(dst_ip) / udp;
    ip.ttl(64); // Set the TTL value to 64 (adjust as needed)

    // Create an Ethernet frame with the IP packet
    EthernetII eth = EthernetII() / ip;

    // Send the packet
    PacketSender sender;
    sender.send(eth, "enp0s3");

    std::cout << "UDP packet sent successfully." << std::endl;

    return 0;
}