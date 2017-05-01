/*
 * TcpAddressInfo.cpp
 *
 *  Created on: Apr 26, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/TcpAddressInfo.hpp>

#include <amqpAnalyze/TcpDissector.hpp>

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

    TcpAddressInfo::TcpAddressInfo(const TcpDissector* tcpDissectorPtr):
        _srcAddrStr(tcpDissectorPtr->getSourceAddrStr(false)),
        _destAddrStr(tcpDissectorPtr->getDestinationAddrStr(false)),
        _hash(tcpDissectorPtr->getConnectionHash())
    {}

    TcpAddressInfo::TcpAddressInfo(const std::string& srcAddrStr, const std::string& destAddrStr, std::size_t hash):
        _srcAddrStr(srcAddrStr),
        _destAddrStr(destAddrStr),
        _hash(hash)
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

} /* namespace amqpAnalyze */
