#include "PacketLauncher.hpp"

#include <tins/tins.h>

#include <string.h>

using namespace Tins;

bool checkPacketType(const char* cString, const char* otherString) { // put string comp in utils file
    return strcmp(cString, otherString) == 0;
}

PacketLauncher::PacketLauncher(const char* targetAddress, const int targetPort, const char* algo) 
 : targetPort(targetPort), packetType(algo) {
    this->iface = NetworkInterface::default_interface();
    this->ifaceInfo = iface.addresses();
    this->srcAddress = ifaceInfo.ip_addr;
    printf("targetAddress: %s\n", targetAddress);
    IPv4Address convert(targetAddress);
    this->dstAddress = convert;
    printf("targetAddress new: %s\n", dstAddress.to_string().c_str());
 }


 void PacketLauncher::launchPacket() {
    try {
        printf("iface name: %s\n", iface.name().c_str());
        sender.send(pkt, iface);
        printf("Success!\n");
    } catch (std::exception& ex) {
        printf("Failure!\n");
    }
    printf("Packet sent!\n");
 }

void PacketLauncher::craftPacket() {
    if (checkPacketType(packetType, "TCP-SYN")) {
        this->pkt = craftTcpSynPacket();
    } else if (checkPacketType(packetType, "TCP-ACK")) {
        this->pkt = craftTcpAckPacket();
    } else if (checkPacketType(packetType, "TCP-SYN-ACK")) {
        this->pkt = craftTcpSynAckPacket();
    } else if (checkPacketType(packetType, "TCP-XMAS")) {
        this->pkt = craftTcpXmasPacket();
    } else if (checkPacketType(packetType, "UDP")) {
        this->pkt = craftUdpPacket();
    } else if (checkPacketType(packetType, "ICMP-REQ")) {
       this->pkt = craftIcmpRequestPacket();
    } else if (checkPacketType(packetType, "ICMP-REP")) {
        this->pkt = craftIcmpReplyPacket();
    } else if (checkPacketType(packetType, "DNS")) {
        this->pkt = craftDnsQueryPacket();
    } else if (checkPacketType(packetType, "ARP-REQ")) {
        this->pkt = craftArpQueryPacket();
    }
}

EthernetII PacketLauncher::craftTcpSynPacket() {
    EthernetII newPkt = EthernetII(EthernetII::BROADCAST, ifaceInfo.hw_addr) / 
                     IP(dstAddress, srcAddress) /
                     TCP();
    TCP& tcp = newPkt.rfind_pdu<TCP>(); 
    tcp.set_flag(TCP::SYN, 1);               
    tcp.sport(srcPort); 
    tcp.dport(targetPort);

    return newPkt;
}

EthernetII PacketLauncher::craftTcpAckPacket() {
    EthernetII newPkt = EthernetII(EthernetII::BROADCAST, ifaceInfo.hw_addr) / 
                     IP(dstAddress, srcAddress) /
                     TCP();
    TCP& tcp = newPkt.rfind_pdu<TCP>(); 
    tcp.set_flag(TCP::ACK, 1);               
    tcp.sport(srcPort);
    tcp.dport(targetPort);

    return newPkt;
}

EthernetII PacketLauncher::craftTcpSynAckPacket() {
    EthernetII newPkt = EthernetII(EthernetII::BROADCAST, ifaceInfo.hw_addr) / 
                     IP(dstAddress, srcAddress) /
                     TCP();
    TCP& tcp = newPkt.rfind_pdu<TCP>(); 
    tcp.set_flag(TCP::SYN, 1);    
    tcp.set_flag(TCP::ACK, 1);               
    tcp.sport(srcPort);
    tcp.dport(targetPort);

    return newPkt;
}

EthernetII PacketLauncher::craftTcpXmasPacket() {
    EthernetII newPkt = EthernetII(EthernetII::BROADCAST, ifaceInfo.hw_addr) / 
                     IP(dstAddress, srcAddress) /
                     TCP();
    TCP& tcp = newPkt.rfind_pdu<TCP>(); 
    tcp.set_flag(TCP::URG, 1);
    tcp.set_flag(TCP::ACK, 1);
    tcp.set_flag(TCP::PSH, 1);
    tcp.set_flag(TCP::RST, 1);
    tcp.set_flag(TCP::SYN, 1);
    tcp.set_flag(TCP::FIN, 1);        
    tcp.sport(srcPort);
    tcp.dport(targetPort);

    return newPkt;
}

EthernetII PacketLauncher::craftUdpPacket() {
    std::string payload = "DEADBEEF";
    RawPDU raw(payload);
    EthernetII newPkt = EthernetII(EthernetII::BROADCAST, ifaceInfo.hw_addr) / 
                        IP(dstAddress, srcAddress) /
                        UDP(targetPort, srcPort) / // random port for now
                        raw;
    return newPkt; 
}

EthernetII PacketLauncher::craftIcmpRequestPacket() {
    EthernetII newPkt = EthernetII(EthernetII::BROADCAST, ifaceInfo.hw_addr) / 
                        IP(dstAddress, srcAddress) /
                        ICMP();
    ICMP& icmp = newPkt.rfind_pdu<ICMP>();
    icmp.type(ICMP::ECHO_REQUEST);

    return newPkt;
}

EthernetII PacketLauncher::craftIcmpReplyPacket() {
    EthernetII newPkt = EthernetII(EthernetII::BROADCAST, ifaceInfo.hw_addr) / 
                     IP(dstAddress, srcAddress) /
                     ICMP();
    ICMP& icmp = newPkt.rfind_pdu<ICMP>();
    icmp.type(ICMP::ECHO_REPLY);

    return newPkt;
}

EthernetII PacketLauncher::craftDnsQueryPacket() {
    EthernetII newPkt = EthernetII(EthernetII::BROADCAST, ifaceInfo.hw_addr) / 
                        IP(dstAddress, srcAddress) /
                        UDP(53, srcPort) / 
                        DNS();
    DNS& dns = newPkt.rfind_pdu<DNS>();
    dns.add_query({ dstAddress.to_string().c_str(), DNS::A, DNS::IN }); // TEMP, expand upon

    return newPkt;
}

EthernetII PacketLauncher::craftArpQueryPacket() {
    EthernetII newPkt = EthernetII("ff:ff:ff:ff:ff:ff", ifaceInfo.hw_addr) / 
                        ARP();
    
    ARP& arp = newPkt.rfind_pdu<ARP>();
    arp.opcode(ARP::REQUEST);
    arp.sender_ip_addr(srcAddress);
    arp.target_ip_addr(dstAddress);
    arp.sender_hw_addr(ifaceInfo.hw_addr);
    arp.target_hw_addr("ff:ff:ff:ff:ff:ff"); 

    return newPkt;
}
