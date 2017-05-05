/*
 * TcpConnectionMap.cpp
 *
 *  Created on: May 3, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/TcpConnectionMap.hpp>

#include <amqpAnalyze/Error.hpp>
#include <amqpAnalyze/TcpAddressInfo.hpp>
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

    uint32_t TcpConnectionMap::handleTcpHeader(const TcpAddressInfo& tcpAddressInfo, const struct tcphdr& tcpHeader, uint64_t packetNum) {
        return getTcpConnection(tcpAddressInfo, tcpHeader, packetNum)->_connectionIndex;
    }

    bool TcpConnectionMap::hasConnection(std::size_t hash) const {
        return _connectionMap.find(hash) != _connectionMap.end();
    }

    void TcpConnectionMap::print(std::ostream& os, bool showHashFlag) const {
        os << "TCP connections found:\n";
        os << "index" << std::setw(16) << "initiator" << std::setw(19) << "responder";
        if (showHashFlag) os << std::setw(18) << "hash";
        os << "\n";
        for (uint32_t i=0; i<_connectionList.size(); ++i) {
            TcpConnection* tcPtr = _connectionMap.at(_connectionList.at(i));
            os << std::setfill(' ') << std::setw(4) << (i+1) << ". " << std::setw(15) << tcPtr->_srcAddrStr << " -> " << std::setw(15) << tcPtr->_destAddrStr;
            if (showHashFlag) os << "  " << std::hex << std::setfill('0') << std::setw(16) << tcPtr->_hash;
            os << "\n";
        }
    }

    TcpConnection* TcpConnectionMap::getTcpConnection(const TcpAddressInfo& tcpAddressInfo, const struct tcphdr& tcpHeader, uint64_t packetNum) {
        TcpConnection* tcpConnectionPtr = nullptr;
        if (hasConnection(tcpAddressInfo._hash)) {
            tcpConnectionPtr = _connectionMap.at(tcpAddressInfo._hash);
            if (tcpHeader.syn) {
                if (tcpConnectionPtr->_initDestSequence == 0) {
                    tcpConnectionPtr->_initDestSequence = tcpHeader.seq;
                } else {
                    throw amqpAnalyze::Error(MSG("[" << packetNum << "] TcpConnectionMap::getTcpConnection: Destination SYN already set: " << tcpAddressInfo));
                }
            }
            if (tcpHeader.ack) {
                // TODO: add ack handling here
            }
            if (tcpHeader.fin) {
                if (tcpAddressInfo._srcAddrStr.compare(tcpConnectionPtr->_srcAddrStr) == 0) {
                    tcpConnectionPtr->_srcFinFlag = true;
                } else if (tcpAddressInfo._srcAddrStr.compare(tcpConnectionPtr->_destAddrStr) == 0) {
                    tcpConnectionPtr->_destFinFlag = true;
                } else {
                    throw amqpAnalyze::Error(MSG("[" << packetNum << "] TcpConnectionMap::getTcpConnection: Address does not match connection source or destination: addr="
                                                 << tcpAddressInfo << "; connection=" << tcpConnectionPtr));
                }
            }
        } else {
            if (tcpHeader.syn) {
                tcpConnectionPtr = new TcpConnection(tcpAddressInfo, tcpHeader.seq, _connectionList.size()+1);
                _connectionMap.emplace(tcpAddressInfo._hash, tcpConnectionPtr);
                _connectionList.push_back(tcpAddressInfo._hash);
            } else {
                throw amqpAnalyze::Error(MSG("[" << packetNum << "] TcpConnectionMap::getTcpConnection: Address not found, no SYN flag: " << tcpAddressInfo));
            }
        }
        return tcpConnectionPtr;
    }

} /* namespace amqpAnalyze */
