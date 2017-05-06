/*
 * TcpConnection.cpp
 *
 *  Created on: May 6, 2017
 *      Author: kvdr
 */

#include <amqpAnalyze/TcpConnection.hpp>
#include <iomanip>
#include <iostream>

namespace amqpAnalyze
{

    TcpConnection::TcpConnection(const TcpAddressInfo& tcpAddressInfo, uint32_t initSrcSequence, uint32_t connectionIndex, uint64_t packetNumber):
        TcpAddressInfo(tcpAddressInfo),
        _initSrcSequence(initSrcSequence),
        _initDestSequence(0),
        _srcFinFlag(false),
        _destFinFlag(false),
        _connectionIndex(connectionIndex),
        _firstPacketNumber(packetNumber),
        _lastPacketNumber(packetNumber)
    {}

    TcpConnection::~TcpConnection() {}

} /* namespace amqpAnalyze */

std::ostream& operator<<(std::ostream& o, const amqpAnalyze::TcpConnection& t) {
    o << t.srcAddress() << " -> " << t.destAddress() << " hash=0x" << std::setfill('0') << std::setw(16) << std::hex << t.hash() << std::dec;
    return o;
}

std::ostream& operator<<(std::ostream& o, const amqpAnalyze::TcpConnection* t) {
    o << t->srcAddress() << " -> " << t->destAddress() << " hash=0x" << std::setfill('0') << std::setw(16) << std::hex << t->hash() << std::dec;
    return o;
}