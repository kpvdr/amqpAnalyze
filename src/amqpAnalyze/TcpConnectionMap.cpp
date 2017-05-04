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

    void TcpConnectionMap::handleTcpSyn(const TcpAddressInfo& tcpAddressInfo, uint32_t sequence) {
        if (hasConnection(tcpAddressInfo._hash)) {
            TcpConnection* tcpConnectionPtr = _connectionMap.at(tcpAddressInfo._hash);
            if (tcpConnectionPtr->_initDestSequence == 0) {
                tcpConnectionPtr->_initDestSequence = sequence;
            } else {
                throw amqpAnalyze::Error(MSG("TcpConnectionMap::handleTcpSyn: Destination SYN already set: " << tcpAddressInfo));
            }
        } else {
            TcpConnection* tcpConnectionPtr = new TcpConnection(tcpAddressInfo, sequence);
            _connectionMap.emplace(tcpAddressInfo._hash, tcpConnectionPtr);
            _connectionList.push_back(tcpAddressInfo._hash);
        }
    }

    void TcpConnectionMap::handleTcpFin(const TcpAddressInfo& tcpAddressInfo) {
        if (hasConnection(tcpAddressInfo._hash)) {
            TcpConnection* tcpConnectionPtr = _connectionMap.at(tcpAddressInfo._hash);
            if (tcpAddressInfo._srcAddrStr.compare(tcpConnectionPtr->_srcAddrStr) == 0) {
                tcpConnectionPtr->_srcFinFlag = true;
            } else if (tcpAddressInfo._srcAddrStr.compare(tcpConnectionPtr->_destAddrStr) == 0) {
                tcpConnectionPtr->_destFinFlag = true;
            } else {
                throw amqpAnalyze::Error(MSG("TcpConnectionMap::handleTcpFin: Address does not match connection source or destination: addr="
                                             << tcpAddressInfo << "; connection=" << tcpConnectionPtr));
            }
        } else {
            throw amqpAnalyze::Error(MSG("TcpConnectionMap::handleTcpFin: Address hash not found: " << tcpAddressInfo));
        }
    }

    bool TcpConnectionMap::hasConnection(std::size_t hash) const {
        return _connectionMap.find(hash) != _connectionMap.end();
    }

    void TcpConnectionMap::print(std::ostream& os) const {
        os << "TCP connections found:\n";
        for (uint32_t i=0; i<_connectionList.size(); ++i) {
            TcpConnection* tcPtr = _connectionMap.at(_connectionList.at(i));
            os << std::setfill(' ') << std::setw(4) << (i+1) << ". " << std::setw(15) << tcPtr->_srcAddrStr << " -> " << std::setw(15) << tcPtr->_destAddrStr << "\n";
        }
    }

} /* namespace amqpAnalyze */
