/*
 * TcpDissector.cpp
 *
 *  Created on: Apr 2, 2017
 *      Author: kpvdr
 */

#include "TcpDissector.hpp"

#include <amqpAnalyze/amqp10/ConnectionHandler.hpp>
#include <amqpAnalyze/AmqpDissector.hpp>
#include <amqpAnalyze/Color.hpp>
#include <amqpAnalyze/IpDissector.hpp>
#include <amqpAnalyze/Ip4Dissector.hpp>
#include <amqpAnalyze/Ip6Dissector.hpp>
#include <amqpAnalyze/Packet.hpp>
#include <amqpAnalyze/TcpConnection.hpp>
#include <amqpAnalyze/TcpConnectionMap.hpp>
#include <cstring>
#include <iostream>
#include <pcap.h>

namespace amqpAnalyze
{

    TcpDissector::TcpDissector(Packet* packetPtr, uint32_t dataOffs, Dissector* parent):
            Dissector(packetPtr, dataOffs, parent),
            _tcpConnection(nullptr),
            _replyFlag(false)
    {
        std::memcpy((char*)&_tcpHeader, (const char*)(_packetPtr->dataPtr()+_dataOffs), sizeof(struct tcphdr));
        _hdrSizeBytes = _tcpHeader.doff * sizeof(uint32_t);  // doff is tcp header size in 32-bit words
        _remainingDataLength = _packetPtr->pcapPacketHeaderPtr()->caplen - _dataOffs - _hdrSizeBytes;
        try {
            _tcpConnection = g_tcpConnectionMap.getTcpConnection(this, _packetPtr->packetNum());
            _replyFlag = _tcpConnection->srcAddress().compare(tcpDestinationAddrStr(false)) == 0;
            if (_tcpHeader.fin) {
                g_amqpConnectionHandler.tcpClose(this); // Notify connection state objects of TCP close
            }
            if (_remainingDataLength) {
                try {
                    packetPtr->addDissector(new AmqpDissector(_packetPtr, _dataOffs + _hdrSizeBytes, this, _remainingDataLength));
                } catch (Error& e) {
                    // ignore non-AMQP packets
                    // TODO, create specific error class for non-AMQP data
                }
            }
        } catch (const Error& e) {
            std::cout << Color::color(DisplayColorType_t::MSG_ERROR, MSG("Error: " << e.what())) << std::endl;
        }
    }

    TcpDissector::~TcpDissector() {}

    void TcpDissector::appendString(std::ostringstream& oss, size_t margin) const {
        oss << "\n" << std::string(margin, ' ') << Color::color(DisplayColorType_t::DISSECTOR_NAME, "TCP")
            << ": " << tcpSourceAddrStr() << " -> " << tcpDestinationAddrStr() << " [" << tcpFlagsAsStr()
            << "] ci=" << connectionIndex();
        appendErrors(oss, margin);
    }

    std::string TcpDissector::tcpSourceAddrStr(bool colorFlag) const {
        std::stringstream oss;
        oss << ((IpDissector*)_parent)->getSourceAddrStr() << ":";
        if (colorFlag) {
            oss << Color::color(DisplayColorType_t::TCP_PORT, std::to_string(tcpSourcePort()));
        } else {
            oss << std::to_string(tcpSourcePort());
        }
        return oss.str();
    }

    std::string TcpDissector::tcpDestinationAddrStr(bool colorFlag) const {
        std::stringstream oss;
        oss << ((IpDissector*)_parent)->getDestinationAddrStr() << ":";
        if (colorFlag) {
            oss << Color::color(DisplayColorType_t::TCP_PORT, std::to_string(tcpDestinationPort()));
        } else {
            oss << std::to_string(tcpDestinationPort());
        }
        return oss.str();
    }

    std::string TcpDissector::hashStr() const {
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
                if (tcpSourcePort() > tcpDestinationPort()) {
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
                if (tcpSourcePort() > tcpDestinationPort()) {
                    srcAddrGreater = true;
                }
            } else if (srcAddr > destAddr) {
                srcAddrGreater = true;
            }
        }
        std::stringstream oss;
        if (srcAddrGreater) {
            oss << tcpDestinationAddrStr() << ", " << tcpSourceAddrStr();
        } else {
            oss << tcpSourceAddrStr() << ", " << tcpDestinationAddrStr();
        }
        return oss.str();
    }

    uint32_t TcpDissector::connectionIndex() const {
        if (_tcpConnection == nullptr) return 0;
        return _tcpConnection->connectionIndex();
    }

    uint16_t TcpDissector::tcpSourcePort() const {
        return ntohs(_tcpHeader.source);
    }

    uint16_t TcpDissector::tcpDestinationPort() const {
        return ntohs(_tcpHeader.dest);
    }

    std::string TcpDissector::tcpFlagsAsStr() const {
        std::ostringstream oss;
        bool spacer = false;
        if (_tcpHeader.fin != 0) { oss << "FIN"; spacer = true; }
        if (_tcpHeader.syn != 0) { oss << (spacer?" ":"") << "SYN"; spacer = true; }
        if (_tcpHeader.rst != 0) { oss << (spacer?" ":"") << Color::color(DisplayColorType_t::TCP_RST_FLAG, "RST"); }
        if (_tcpHeader.psh != 0) { oss << (spacer?" ":"") << "PSH"; spacer = true; }
        if (_tcpHeader.ack != 0) { oss << (spacer?" ":"") << "ACK"; spacer = true; }
        if (_tcpHeader.urg != 0) { oss << (spacer?" ":"") << "URG"; }
        return oss.str();
    }

    bool TcpDissector::tcpAck() const {
        return _tcpHeader.ack != 0;
    }

    bool TcpDissector::tcpFin() const {
        return _tcpHeader.fin != 0;
    }

    bool TcpDissector::tcpSyn() const {
        return _tcpHeader.syn != 0;
    }

    uint32_t TcpDissector::tcpSequenceNum() const {
        return ::ntohl(_tcpHeader.seq);
    }

    uint32_t TcpDissector::tcpAckSequenceNum() const {
        return ::ntohl(_tcpHeader.ack_seq);
    }

    std::size_t TcpDissector::hash() const {
        std::hash<std::string> hashFn;
        return hashFn(hashStr());
    }

    bool TcpDissector::replyFlag() const {
        return _replyFlag;
    }

    const TcpConnection* TcpDissector::tcpConnection() const {
        return _tcpConnection;
    }

} /* namespace amqpAnalyze */
