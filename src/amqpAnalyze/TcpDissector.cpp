/*
 * TcpDissector.cpp
 *
 *  Created on: Apr 2, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/TcpDissector.hpp>

#include <amqpAnalyze/amqp10/ConnectionHandler.hpp>
#include <amqpAnalyze/AmqpDissector.hpp>
#include <amqpAnalyze/Color.hpp>
#include <amqpAnalyze/IpDissector.hpp>
#include <amqpAnalyze/Ip4Dissector.hpp>
#include <amqpAnalyze/Ip6Dissector.hpp>
#include <cstring>
#include <iostream>
#include <pcap.h>

namespace amqpAnalyze
{

    TcpDissector::TcpDissector(const Dissector* parent,
                               uint64_t packetNum,
                               const struct pcap_pkthdr* pcapPacketHeaderPtr,
                               const uint8_t* packetPtr,
                               const uint32_t packetOffs,
                               DissectorList_t& protocolList):
            Dissector(parent, packetNum, packetOffs, protocolList)
    {
        std::memcpy((char*)&_tcpHeader, (const char*)(packetPtr+packetOffs), sizeof(struct tcphdr));
        _hdrSizeBytes = _tcpHeader.doff * sizeof(uint32_t);  // doff is tcp header size in 32-bit words
        _remainingDataLength = pcapPacketHeaderPtr->caplen - packetOffs - _hdrSizeBytes;
        _tcpAddressInfo.setAddress(this);
        try {
            if (_tcpHeader.fin) {
                // Notify connection state objects of TCP close
                g_amqpConnectionHandlerPtr->tcpClose(_tcpAddressInfo);
            }
            if (_remainingDataLength) {
                try {
                    _protocolList.push_front(new AmqpDissector(this,
                                                               _packetNum,
                                                               pcapPacketHeaderPtr,
                                                               packetPtr,
                                                               packetOffs + _hdrSizeBytes,
                                                               protocolList,
                                                               _remainingDataLength));
                } catch (Error& e) {
                    // ignore, non-AMQP
                    // TODO, create specific error class for this!
                }
            }
        } catch (const Error& e) {
            std::cout << Color::color(DisplayColorType_t::MSG_ERROR, MSG("Error: " << e.what())) << std::endl;
        }
    }

    TcpDissector::~TcpDissector() {}

    void TcpDissector::appendString(std::ostringstream& oss, size_t margin) const {
        oss << "\n" << std::string(margin, ' ') << Color::color(DisplayColorType_t::DISSECTOR_NAME, "TCP")
            << ": " << getSourceAddrStr() << " -> " << getDestinationAddrStr() << " [" << getFlagsAsString() << "]";
    }

    std::string TcpDissector::getSourceAddrStr(bool colorFlag) const {
        std::stringstream oss;
        oss << ((IpDissector*)_parent)->getSourceAddrStr() << ":";
        if (colorFlag) {
            oss << Color::color(DisplayColorType_t::TCP_PORT, std::to_string(getSourcePort()));
        } else {
            oss << std::to_string(getSourcePort());
        }
        return oss.str();
    }

    std::string TcpDissector::getDestinationAddrStr(bool colorFlag) const {
        std::stringstream oss;
        oss << ((IpDissector*)_parent)->getDestinationAddrStr() << ":";
        if (colorFlag) {
            oss << Color::color(DisplayColorType_t::TCP_PORT, std::to_string(getDestinationPort()));
        } else {
            oss << std::to_string(getDestinationPort());
        }
        return oss.str();
    }

    std::string TcpDissector::getConnectionIndex() const {
        bool srcAddrGreater = false;
        if (((IpDissector*)_parent)->isIp6()) {
            // IPv6 hash
            Ip6Dissector* ip6ParentDissector = (Ip6Dissector*)_parent;
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
            Ip4Dissector* ip4Dissector = (Ip4Dissector*)_parent;
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
            oss << getDestinationAddrStr() << ", " << getSourceAddrStr();
        } else {
            oss << getSourceAddrStr() << ", " << getDestinationAddrStr();
        }
        return oss.str();
    }

    uint16_t TcpDissector::getSourcePort() const {
        return ::ntohs(_tcpHeader.source);
    }

    uint16_t TcpDissector::getDestinationPort() const {
        return ::ntohs(_tcpHeader.dest);
    }

    std::string TcpDissector::getFlagsAsString() const {
        std::ostringstream oss;
        bool spacer = false;
        if (_tcpHeader.fin) { oss << "FIN"; spacer = true; }
        if (_tcpHeader.syn) { oss << (spacer?" ":"") << "SYN"; spacer = true; }
        if (_tcpHeader.rst) { oss << (spacer?" ":"") << Color::color(DisplayColorType_t::TCP_RST_FLAG, "RST"); }
        if (_tcpHeader.psh) { oss << (spacer?" ":"") << "PSH"; spacer = true; }
        if (_tcpHeader.ack) { oss << (spacer?" ":"") << "ACK"; spacer = true; }
        if (_tcpHeader.urg) { oss << (spacer?" ":"") << "URG"; }
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

    const TcpAddressInfo& TcpDissector::getTcpAddressInfo() const {
        return _tcpAddressInfo;
    }

} /* namespace amqpAnalyze */
