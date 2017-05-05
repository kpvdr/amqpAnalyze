/*
 * TcpConnectionMap.cpp
 *
 *  Created on: May 3, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/TcpConnectionMap.hpp>

#include <amqpAnalyze/Error.hpp>
#include <amqpAnalyze/TcpAddressInfo.hpp>
#include <amqpAnalyze/TcpDissector.hpp>
#include <iomanip>
#include <netinet/tcp.h>
#include <sstream>

// Global TCP connection map
amqpAnalyze::TcpConnectionMap g_tcpConnectionMap;

namespace amqpAnalyze
{

    TcpConnectionMap::TcpConnectionMap():
        _connectionMap(),
        _connectionList()
    {}

    TcpConnectionMap::~TcpConnectionMap() {
        for (ConnectionMap_itr_t i=_connectionMap.begin(); i!=_connectionMap.end(); ++i) {
            delete i->second;
        }
        _connectionMap.clear();
        _connectionList.clear();
    }

    uint32_t TcpConnectionMap::handleTcpHeader(TcpDissector* tcpDissector, uint64_t packetNum) {
        return getTcpConnection(tcpDissector, packetNum)->_connectionIndex;
    }

    bool TcpConnectionMap::hasConnection(std::size_t hash) const {
        return _connectionMap.find(hash) != _connectionMap.end();
    }

    void TcpConnectionMap::print(std::ostream& os, bool showHashFlag) const {
        os << "TCP connections found:\nindex       initiator          responder";
        if (showHashFlag) os << "  hash            ";
        os << "  packet range" << "\n";
        for (uint32_t i=0; i<_connectionList.size(); ++i) {
            TcpConnection* tcPtr = _connectionMap.at(_connectionList.at(i));
            os << std::setfill(' ') << std::setw(4) << (i+1) << ". " << std::setw(15) << tcPtr->_srcAddrStr << " -> " << std::setw(15) << tcPtr->_destAddrStr;
            if (showHashFlag) os << "  " << std::hex << std::setfill('0') << std::setw(16) << tcPtr->_hash << std::dec;
            os << "  [" << tcPtr->_firstPacketNumber << " - " << tcPtr->_lastPacketNumber << "]" << "\n";
        }
    }

    TcpConnection* TcpConnectionMap::getTcpConnection(TcpDissector* tcpDissector, uint64_t packetNum) {
        TcpConnection* tcpConnectionPtr = nullptr;
        const TcpAddressInfo& tcpAddressInfo = tcpDissector->getTcpAddressInfo();
        if (hasConnection(tcpAddressInfo._hash)) {
            tcpConnectionPtr = _connectionMap.at(tcpAddressInfo._hash);
            tcpConnectionPtr->setLastPacketNumber(packetNum);
            if (tcpDissector->syn()) {
                if (tcpConnectionPtr->_initDestSequence == 0) {
                    tcpConnectionPtr->_initDestSequence = tcpDissector->getSequence();
                } else {
                    tcpDissector->addError(new amqpAnalyze::Error(MSG("TcpConnectionMap::getTcpConnection: Destination SYN already set: " << tcpAddressInfo)));
                }
            }
            if (tcpDissector->ack()) {
                // TODO: add ack handling here
            }
            if (tcpDissector->fin()) {
                if (tcpAddressInfo._srcAddrStr.compare(tcpConnectionPtr->_srcAddrStr) == 0) {
                    tcpConnectionPtr->_srcFinFlag = true;
                } else if (tcpAddressInfo._srcAddrStr.compare(tcpConnectionPtr->_destAddrStr) == 0) {
                    tcpConnectionPtr->_destFinFlag = true;
                } else {
                    tcpDissector->addError(new amqpAnalyze::Error(MSG("TcpConnectionMap::getTcpConnection: Address does not match connection source or destination: addr="
                                                 << tcpAddressInfo << "; connection=" << tcpConnectionPtr)));
                }
            }
        } else {
            if (!tcpDissector->syn()) {
                tcpDissector->addError(new amqpAnalyze::Error(MSG("ERROR: No previous TCP SYN flag seen for addresss "
                                << tcpAddressInfo._srcAddrStr << " or " << tcpAddressInfo._destAddrStr << ": Possible previous packet(s) missing")));
            }
            tcpConnectionPtr = new TcpConnection(tcpAddressInfo, tcpDissector->getSequence(), _connectionList.size()+1, packetNum);
            _connectionMap.emplace(tcpAddressInfo._hash, tcpConnectionPtr);
            _connectionList.push_back(tcpAddressInfo._hash);
        }
        return tcpConnectionPtr;
    }

} /* namespace amqpAnalyze */
