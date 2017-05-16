/*
 * Ip6Dissector.cpp
 *
 *  Created on: Apr 2, 2017
 *      Author: kpvdr
 */

#include "Ip6Dissector.hpp"

#include <amqpAnalyze/Error.hpp>
#include <amqpAnalyze/Packet.hpp>
#include <amqpAnalyze/TcpDissector.hpp>
#include <arpa/inet.h>
#include <cstring>

namespace amqpAnalyze {

    Ip6Dissector::Ip6Dissector(Packet* packetPtr, const uint32_t dataOffs):
        IpDissector(packetPtr, dataOffs)
    {
        std::memcpy((char*)&_ip6Header, (const char*)(_packetPtr->dataPtr()+dataOffs), sizeof(struct ip6_hdr));
        switch (_ip6Header.ip6_nxt) {
        case IPPROTO_TCP:
            packetPtr->addDissector(new TcpDissector(_packetPtr, _dataOffs + sizeof(struct ip6_hdr), this));
            break;
        default:
            throw Error(MSG("[" << _packetPtr->packetNum() << "] IPv6 header: Unhandled IP protocol: 0x" << std::hex << (int)_ip6Header.ip6_nxt));
        }
    }

    Ip6Dissector::~Ip6Dissector() {}

    void Ip6Dissector::appendString(std::ostringstream& oss, size_t margin) const {}

    void Ip6Dissector::getSourceAddr(std::array<uint32_t, 4>& sourceAddr) const {
        sourceAddr[0] = _ip6Header.ip6_src.s6_addr32[0];
        sourceAddr[1] = _ip6Header.ip6_src.s6_addr32[1];
        sourceAddr[2] = _ip6Header.ip6_src.s6_addr32[2];
        sourceAddr[3] = _ip6Header.ip6_src.s6_addr32[3];
    }

    void Ip6Dissector::getDestinationAddr(std::array<uint32_t, 4>& destinationAddr) const {
        destinationAddr[0] = _ip6Header.ip6_dst.s6_addr32[0];
        destinationAddr[1] = _ip6Header.ip6_dst.s6_addr32[1];
        destinationAddr[2] = _ip6Header.ip6_dst.s6_addr32[2];
        destinationAddr[3] = _ip6Header.ip6_dst.s6_addr32[3];
    }

    std::string Ip6Dissector::getSourceAddrStr() const {
        char buf[INET6_ADDRSTRLEN];
        ::inet_ntop(AF_INET6, &(_ip6Header.ip6_src), buf, INET6_ADDRSTRLEN);
        return buf;
    }

    std::string Ip6Dissector::getDestinationAddrStr() const {
        char buf[INET6_ADDRSTRLEN];
        ::inet_ntop(AF_INET6, &(_ip6Header.ip6_dst), buf, INET6_ADDRSTRLEN);
        return buf;
    }

} /* namespace amqpAnalyze */
