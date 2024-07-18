#include <iostream>
#include <string>
#include <tins/tins.h>

using namespace Tins;

void send_packet(const std::string& destination_ip, int destination_port) {
    PacketSender sender;
    IPv4Address dst_ip(destination_ip);

    // Create a raw PDU (payload)
    std::string payload = "Hello, this is a test packet!";
    RawPDU raw(payload);

    // Create a UDP packet
    IP ip = IP(dst_ip) / UDP(destination_port, 55555) / raw;

    // Send the packet
    try {
        sender.send(ip);
        std::cout << "Packet sent successfully!" << std::endl;
    } catch (std::exception& ex) {
        std::cerr << "Packet send failed: " << ex.what() << std::endl;
    }
}

void send_tcp_syn_packet(const std::string& destination_ip, int destination_port) {
    try {
        // Create a TCP SYN packet
        // Create a PacketSender object
        PacketSender sender;

        // Craft the Ethernet, IP, and TCP layers
        IP ip = IP("192.168.1.127") / TCP();

        TCP& tcp = ip.rfind_pdu<TCP>(); 
        tcp.set_flag(TCP::SYN, 1);               
        tcp.sport(1337);
        tcp.dport(80);

        sender.send(ip);

        std::cout << "TCP SYN packet sent successfully." << std::endl;;
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void send_tcp_xmas_packet() {
    try {
        // Create a TCP SYN packet
        // Create a PacketSender object
        PacketSender sender;

        // Craft the Ethernet, IP, and TCP layers
        IP ip = IP("192.168.1.127") / TCP();

        TCP& tcp = ip.rfind_pdu<TCP>(); 
        tcp.set_flag(TCP::URG, 1);
        tcp.set_flag(TCP::ACK, 1);
        tcp.set_flag(TCP::PSH, 1);
        tcp.set_flag(TCP::RST, 1);
        tcp.set_flag(TCP::SYN, 1);
        tcp.set_flag(TCP::FIN, 1);

        tcp.sport(1337);
        tcp.dport(80);

        sender.send(ip);

        std::cout << "TCP SYN packet sent successfully." << std::endl;;
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void send_tcp_ack_packet() {
    try {
        // Create a TCP SYN packet
        // Create a PacketSender object
        PacketSender sender;

        // Craft the Ethernet, IP, and TCP layers
        IP ip = IP("192.168.1.127") / TCP();

        TCP& tcp = ip.rfind_pdu<TCP>(); 
        tcp.set_flag(TCP::ACK, 1);

        tcp.sport(1337);
        tcp.dport(80);

        sender.send(ip);

        std::cout << "TCP SYN packet sent successfully." << std::endl;;
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void send_icmp_echo_request() {
    IP ip = IP("192.168.1.127") / 
            ICMP();

    ICMP& icmp = ip.rfind_pdu<ICMP>();
    icmp.type(ICMP::ECHO_REQUEST);

    PacketSender sender;
    sender.send(ip);

    std::cout << "ICMP Echo Request sent" << std::endl;
}

void send_icmp_echo_reply() {
    IP ip = IP("192.168.1.127") / 
            ICMP();

    ICMP& icmp = ip.rfind_pdu<ICMP>();
    icmp.type(ICMP::ECHO_REPLY);

    PacketSender sender;
    sender.send(ip);

    std::cout << "ICMP Echo reply sent" << std::endl;
}

void send_dns_query() {
    IP ip = IP("192.168.1.127") / 
            UDP(53, 1337) / 
            DNS();
    DNS& dns = ip.rfind_pdu<DNS>();
    dns.add_query({ "www.google.com", DNS::A, DNS::IN });

    PacketSender sender;
    sender.send(ip);

    std::cout << "DNS query sent" << std::endl;
}

void send_arp_request() {
    NetworkInterface::Info info = NetworkInterface::default_interface().addresses();

    EthernetII eth = EthernetII("ff:ff:ff:ff:ff:ff", info.hw_addr) / ARP();

    ARP& arp = eth.rfind_pdu<ARP>();
    arp.sender_ip_addr(info.ip_addr);
    arp.target_ip_addr("192.168.1.127");
    arp.sender_hw_addr(info.hw_addr); // Sender MAC address
    arp.target_hw_addr("ff:ff:ff:ff:ff:ff"); // Broadcast MAC address


    // Create a PacketSender
    // Create a PacketSender
    PacketSender sender;

    // Send the ARP request
    sender.send(eth, NetworkInterface::default_interface());


    std::cout << "ARP req sent" << std::endl;
}

IP craftPdu() {
    IP test = IP("192.168.1.127") /
              TCP();

    TCP& tcp = test.rfind_pdu<TCP>(); 
    tcp.set_flag(TCP::URG, 1);
    tcp.set_flag(TCP::ACK, 1);
    tcp.set_flag(TCP::PSH, 1);
    tcp.set_flag(TCP::RST, 1);
    tcp.set_flag(TCP::SYN, 1);
    tcp.set_flag(TCP::FIN, 1);

    tcp.sport(1337);
    tcp.dport(80);

    return test;
}

void sendPacket(PDU& pdu) {
    PacketSender sender;
    IP& packetToSend = pdu.rfind_pdu<IP>();

    sender.send(packetToSend);

    std::cout << "Packet sent" << std::endl;;
}

int main() {
    std::string destination_ip = "192.168.1.127";  // IP of the listener
    int destination_port = 9999;               // Port of the listener

    //send_tcp_syn_packet(destination_ip, destination_port);
    //send_tcp_xmas_packet();

    //send_packet(destination_ip, destination_port);

    //IP test = craftPdu();
    //sendPacket(test);

    send_icmp_echo_request();
    //send_icmp_echo_reply();

    //send_dns_query();

    //send_arp_request();
    return 0;
}
