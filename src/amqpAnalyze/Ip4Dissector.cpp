/*
 * Ip4Dissector.cpp
 *
 *  Created on: Apr 2, 2017
 *      Author: kpvdr
 */

#include "Ip4Dissector.hpp"

#include <amqpAnalyze/Error.hpp>
#include <amqpAnalyze/Packet.hpp>
#include <amqpAnalyze/TcpDissector.hpp>
#include <arpa/inet.h>
#include <cstring>

namespace amqpAnalyze {

    Ip4Dissector::Ip4Dissector(Packet* packetPtr, const uint32_t dataOffs):
        IpDissector(packetPtr, dataOffs)
    {
        std::memcpy((char*)&_ip4Header, (const char*)(_packetPtr->dataPtr()+dataOffs), sizeof(struct ip));
        switch (_ip4Header.ip_p) {
        case IPPROTO_TCP:
            packetPtr->addDissector(new TcpDissector(_packetPtr, _dataOffs + sizeof(struct ip), this));
            break;
        default:
            throw Error(MSG("[" << _packetPtr->packetNum() << "] IPv4 header: Unhandled IP protocol: 0x" << std::hex << (int)_ip4Header.ip_p));
        }
    }

    Ip4Dissector::~Ip4Dissector() {}

    void Ip4Dissector::appendString(std::ostringstream& oss, size_t margin) const {}

    uint32_t Ip4Dissector::destinationAddr() const {
        return _ip4Header.ip_dst.s_addr;
    }

    std::string Ip4Dissector::destinationAddrStr() const {
        char buf[INET_ADDRSTRLEN];
        ::inet_ntop(AF_INET, &(_ip4Header.ip_dst), buf, INET_ADDRSTRLEN);
        return buf;
    }

    uint32_t Ip4Dissector::sourceAddr() const {
        return _ip4Header.ip_src.s_addr;
    }

    std::string Ip4Dissector::sourceAddrStr() const {
        char buf[INET_ADDRSTRLEN];
        ::inet_ntop(AF_INET, &(_ip4Header.ip_src), buf, INET_ADDRSTRLEN);
        return buf;
    }

} /* namespace amqpAnalyze */
