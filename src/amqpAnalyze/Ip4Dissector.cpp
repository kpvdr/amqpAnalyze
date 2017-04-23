/*
 * IpDissector.cpp
 *
 *  Created on: Apr 2, 2017
 *      Author: kvdr
 */

#include <amqpAnalyze/Ip4Dissector.hpp>

#include <amqpAnalyze/Error.hpp>
#include <amqpAnalyze/TcpDissector.hpp>

#include <arpa/inet.h>
#include <cstring>

namespace amqpAnalyze {

Ip4Dissector::Ip4Dissector(const Options* optionsPtr,
                           uint64_t packetNum,
                           const struct pcap_pkthdr* pcapPacketHeaderPtr,
                           const uint8_t* packetPtr,
                           const uint32_t packetOffs,
                           protocol_list_t& protocolList):
		IpDissector(optionsPtr, packetNum, packetOffs, protocolList)
{
    std::memcpy((char*)&_ip4Header, (const char*)(packetPtr+packetOffs), sizeof(struct ip));
    switch (_ip4Header.ip_p) {
    case IPPROTO_TCP:
        _protocolList.push_front(new TcpDissector(optionsPtr,
                                                  this,
                                                  _packetNum,
                                                  pcapPacketHeaderPtr,
                                                  packetPtr,
                                                  _packetOffs + sizeof(struct ip),
                                                  _protocolList));
        break;
    default:
        throw Error(MSG("[" << _packetNum << "] IPv4 header: Unhandled IP protocol: 0x" << std::hex << (int)_ip4Header.ip_p));
    }
}

Ip4Dissector::~Ip4Dissector() {}

void Ip4Dissector::appendString(std::ostringstream& oss, size_t margin) const {}

uint32_t Ip4Dissector::getSourceAddr() const {
    return _ip4Header.ip_src.s_addr;
}

uint32_t Ip4Dissector::getDestinationAddr() const {
    return _ip4Header.ip_dst.s_addr;
}

std::string Ip4Dissector::getSourceAddrStr() const {
    char buf[INET_ADDRSTRLEN];
    ::inet_ntop(AF_INET, &(_ip4Header.ip_src), buf, INET_ADDRSTRLEN);
    return buf;
}

std::string Ip4Dissector::getDestinationAddrStr() const {
    char buf[INET_ADDRSTRLEN];
    ::inet_ntop(AF_INET, &(_ip4Header.ip_dst), buf, INET_ADDRSTRLEN);
    return buf;
}

} /* namespace amqpAnalyze */
