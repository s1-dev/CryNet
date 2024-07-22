#ifndef PACKETLAUNCHER_HPP
#define PACKETLAUNCHER_HPP

#include <tins/tins.h>

#include <string>
#include <cstdio>
#include <vector>
#include <cstdlib>

#include "GeneralUtils.hpp"

using namespace Tins;

class PacketLauncher {
public:
    PacketLauncher(const char* targetAddress, const int targetPort, const char* algo);

    void craftPacket();
        
    void launchPacket();

private:
    const int srcPort = 55555;
    const int targetPort;
    const char* packetType;
    PacketSender sender;
    EthernetII pkt;
    NetworkInterface iface;
    NetworkInterface::Info ifaceInfo;
    IPv4Address srcAddress;
    IPv4Address dstAddress;

    EthernetII craftTcpSynPacket();
    EthernetII craftTcpAckPacket();
    EthernetII craftTcpSynAckPacket();
    EthernetII craftTcpXmasPacket();
    EthernetII craftUdpPacket();
    EthernetII craftIcmpRequestPacket();
    EthernetII craftIcmpReplyPacket();
    EthernetII craftDnsQueryPacket();
    EthernetII craftArpQueryPacket();
};

#endif // PACKETLAUNCHER_HPP
