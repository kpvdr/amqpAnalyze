/*
 * TcpConnectionMap.cpp
 *
 *  Created on: May 3, 2017
 *      Author: kpvdr
 */

#include "TcpConnectionMap.hpp"

#include <amqpAnalyze/Error.hpp>
#include <amqpAnalyze/TcpConnection.hpp>
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

    TcpConnection* TcpConnectionMap::getTcpConnection(TcpDissector* tcpDissectorPtr, uint64_t packetNum) {
        TcpConnection* tcpConnectionPtr = nullptr;
        const std::size_t hash = tcpDissectorPtr->hash();
        if (hasConnection(hash)) {
            // Existing TcpConnection
            tcpConnectionPtr = _connectionMap.at(hash);
            tcpConnectionPtr->setLastPacketNumber(packetNum);
            if (tcpDissectorPtr->tcpSyn()) {
                if (tcpConnectionPtr->initDestSequence() == 0) {
                    tcpConnectionPtr->setInitDestSequence(tcpDissectorPtr->tcpSequenceNum());
                } else {
                    tcpDissectorPtr->addError(new amqpAnalyze::Error(MSG("ERROR: TcpConnectionMap::getTcpConnection: "
                                    << "Destination sequence number already set: " << tcpConnectionPtr)));
                }
            }
            if (tcpDissectorPtr->tcpAck()) {
                // TODO: add ack handling here
            }
            if (tcpDissectorPtr->tcpFin()) {
                if (tcpConnectionPtr->srcAddress().compare(tcpConnectionPtr->srcAddress()) == 0) {
                    tcpConnectionPtr->setSrcFinFlag();
                } else if (tcpConnectionPtr->srcAddress().compare(tcpConnectionPtr->destAddress()) == 0) {
                    tcpConnectionPtr->setDestFinFlag();
                } else {
                    tcpDissectorPtr->addError(new amqpAnalyze::Error(MSG("ERROR: TcpConnectionMap::getTcpConnection: "
                                    << "Address does not match connection source or destination: addr="
                                    << tcpConnectionPtr << "; connection=" << tcpConnectionPtr)));
                }
            }
        } else {
            // Create new TcpConnection
            if (!tcpDissectorPtr->tcpSyn()) {
                tcpDissectorPtr->addError(new amqpAnalyze::Error(MSG("ERROR: No previous TCP SYN flag seen for addresss "
                                << tcpDissectorPtr->tcpSourceAddrStr(false) << " or " << tcpDissectorPtr->tcpDestinationAddrStr(false)
                                << ": Possible previous packet(s) missing")));
            }
            tcpConnectionPtr = new TcpConnection(tcpDissectorPtr, tcpDissectorPtr->tcpSequenceNum(), _connectionList.size()+1, packetNum);
            _connectionMap.emplace(hash, tcpConnectionPtr);
            _connectionList.push_back(hash);
        }
        return tcpConnectionPtr;
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
            os << std::setfill(' ') << std::setw(4) << (i+1) << ". " << std::setw(15) << tcPtr->srcAddress() << " -> "
                            << std::setw(15) << tcPtr->destAddress();
            if (showHashFlag) os << "  " << std::hex << std::setfill('0') << std::setw(16) << tcPtr->hash() << std::dec;
            os << "  [" << tcPtr->firstPacketNumber() << " - " << tcPtr->lastPacketNumber() << "]" << "\n";
        }
    }

} /* namespace amqpAnalyze */
