/*
 * TcpAddressInfo.cpp
 *
 *  Created on: Apr 26, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/TcpAddressInfo.hpp>

#include <amqpAnalyze/TcpDissector.hpp>
#include <iomanip>

namespace amqpAnalyze
{

    TcpAddressInfo::TcpAddressInfo():
        _srcAddrStr(),
        _destAddrStr(),
        _hash(0)
    {}

    TcpAddressInfo::TcpAddressInfo(const TcpAddressInfo& other):
        _srcAddrStr(other._srcAddrStr),
        _destAddrStr(other._destAddrStr),
        _hash(other._hash)
    {}

    void TcpAddressInfo::setAddress(const TcpDissector* tcpDissectorPtr) {
        _srcAddrStr.assign(tcpDissectorPtr->getSourceAddrStr(false));
        _destAddrStr.assign(tcpDissectorPtr->getDestinationAddrStr(false));
        _hash = tcpDissectorPtr->getConnectionHash();
    }

    std::ostream& operator<<(std::ostream& o, const TcpAddressInfo& t) {
        o << t._srcAddrStr << " -> " << t._destAddrStr << " hash=0x" << std::hex << t._hash << std::dec;
        return o;
    }

    std::ostream& operator<<(std::ostream& o, const TcpAddressInfo* t) {
        o << t->_srcAddrStr << " -> " << t->_destAddrStr << " hash=0x" << std::hex << t->_hash << std::dec;
        return o;
    }



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

    void TcpConnection::setInitDestSequence(uint32_t initDestSequence) {
        _initDestSequence = initDestSequence;
    }

    std::ostream& operator<<(std::ostream& o, const TcpConnection& t) {
        o<< t._srcAddrStr << " -> " << t._destAddrStr << " hash=0x" << std::setfill('0') << std::setw(16) << std::hex << t._hash << std::dec;
        return o;
    }

    std::ostream& operator<<(std::ostream& o, const TcpConnection* t) {
        o<< t->_srcAddrStr << " -> " << t->_destAddrStr << " hash=0x" << std::setfill('0') << std::setw(16) << std::hex << t->_hash << std::dec;
        return o;
    }

} /* namespace amqpAnalyze */
