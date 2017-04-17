/*
 * TcpDissector.cpp
 *
 *  Created on: Apr 2, 2017
 *      Author: kvdr
 */

#include <amqpAnalyze/TcpDissector.hpp>

#include <amqpAnalyze/AmqpDissector.hpp>
#include <amqpAnalyze/Error.hpp>
#include <amqpAnalyze/IpDissector.hpp>
#include <amqpAnalyze/Ip4Dissector.hpp>
#include <amqpAnalyze/Ip6Dissector.hpp>

#include <arpa/inet.h>
#include <array>
#include <pcap.h>
#include <cstring>

// debug
//#include <iostream>

namespace amqpAnalyze {

TcpDissector::TcpDissector(uint64_t packetNum,
                           const struct pcap_pkthdr* pcapPacketHeaderPtr,
                           const uint8_t* packetPtr,
                           const uint32_t packetOffs,
                           std::deque<WireDissector*>& protocolList,
                           const IpDissector* parentIpDissctor):
		WireDissector(packetNum, pcapPacketHeaderPtr, packetPtr, packetOffs, DISSECTOR_TCP, protocolList),
		_parentIpDissctor(parentIpDissctor)
{
    //std::cout << "*** TCP: offs=0x" << std::hex << packetOffs << std::dec << std::endl;
    std::memcpy((char*)&_tcpHeader, (const char*)(packetPtr+packetOffs), sizeof(struct tcphdr));
    _hdrSizeBytes = _tcpHeader.doff * sizeof(uint32_t);  // doff is tcp header size in 32-bit words
    _remainingDataLength = pcapPacketHeaderPtr->caplen - packetOffs - _hdrSizeBytes;
    if (_remainingDataLength) {
        try {
            _protocolList.push_front(new AmqpDissector(_packetNum,
                                                       pcapPacketHeaderPtr,
                                                       packetPtr,
                                                       packetOffs + _hdrSizeBytes,
                                                       protocolList,
                                                       _remainingDataLength,
                                                       this));
        } catch (Error& e) {
            // ignore, non-AMQP
            // TODO, create specific error class for this!
            //std::cout << e.what() << std::endl;
        }
    }
}

TcpDissector::~TcpDissector() {}

void TcpDissector::appendString(std::ostringstream& oss, size_t margin) const {
    oss << std::endl << std::string(margin, ' ') << "TCP " << getSourceTcpAddrStr() << " -> "
        << getDestinationAddrStr() << " [" << getFlagsAsString() << "]";
}

std::string TcpDissector::getSourceTcpAddrStr() const {
    std::stringstream oss;
    oss << _parentIpDissctor->getSourceAddrStr() << ":" << getSourcePort();
    return oss.str();
}

std::string TcpDissector::getDestinationAddrStr() const {
    std::stringstream oss;
    oss << _parentIpDissctor->getDestinationAddrStr() << ":" << getDestinationPort();
    return oss.str();
}

std::string TcpDissector::getConnectionIndex() const {
    bool srcAddrGreater = false;
    if (_parentIpDissctor->isIp6()) {
        // IPv6 hash
        Ip6Dissector* ip6ParentDissector = (Ip6Dissector*)_parentIpDissctor;
        std::array<uint32_t, 4> srcAddr;
        ip6ParentDissector->getSourceAddr(srcAddr);
        std::array<uint32_t, 4> destAddr;
        ip6ParentDissector->getDestinationAddr(destAddr);
        // Order so that addr1<addr2 when taking hash
        if (srcAddr == destAddr) {
            if (getSourcePort() > getDestinationPort()) {
                srcAddrGreater = true;
            }
        } else if (srcAddr > destAddr) {
            srcAddrGreater = true;
        }
    } else {
        // IPv4 hash
        Ip4Dissector* ip4Dissector = (Ip4Dissector*)_parentIpDissctor;
        uint32_t srcAddr = ip4Dissector->getSourceAddr();
        uint32_t destAddr = ip4Dissector->getDestinationAddr();
        // Order so that addr1<addr2 when taking hash
        if (srcAddr == destAddr) {
            if (getSourcePort() > getDestinationPort()) {
                srcAddrGreater = true;
            }
        } else if (srcAddr > destAddr) {
            srcAddrGreater = true;
        }
    }
    std::stringstream oss;
    if (srcAddrGreater) {
        oss << getDestinationAddrStr() << ", " << getSourceTcpAddrStr();
    } else {
        oss << getSourceTcpAddrStr() << ", " << getDestinationAddrStr();
    }
    return oss.str();
}

u_int16_t TcpDissector::getSourcePort() const {
    return ::ntohs(_tcpHeader.source);
}

u_int16_t TcpDissector::getDestinationPort() const {
    return ::ntohs(_tcpHeader.dest);
}

std::string TcpDissector::getFlagsAsString() const {
    std::ostringstream oss;
    bool spacer = false;
    if (_tcpHeader.fin) { oss << "FIN"; spacer = true; }
    if (_tcpHeader.syn) { oss << (spacer?" ":"")  << "SYN"; spacer = true; }
    if (_tcpHeader.rst) { oss << (spacer?" ":"")  << "RST"; spacer = true; }
    if (_tcpHeader.psh) { oss << (spacer?" ":"")  << "PSH"; spacer = true; }
    if (_tcpHeader.ack) { oss << (spacer?" ":"")  << "ACK"; spacer = true; }
    if (_tcpHeader.urg) { oss << (spacer?" ":"")  << "URG"; }
    return oss.str();
}

uint32_t TcpDissector::getSequence() const {
    return ::ntohl(_tcpHeader.seq);
}

uint32_t TcpDissector::getAckSequence() const {
    return ::ntohl(_tcpHeader.ack_seq);
}

std::size_t TcpDissector::getConnectionHash() const {
    std::hash<std::string> hashFn;
    return hashFn(getConnectionIndex());
}

} /* namespace amqpAnalyze */
