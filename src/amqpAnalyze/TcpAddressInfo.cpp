/*
 * TcpAddressInfo.cpp
 *
 *  Created on: Apr 26, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/TcpAddressInfo.hpp>

#include <amqpAnalyze/TcpDissector.hpp>
#include <iomanip>
#include <iostream>

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

    TcpAddressInfo::~TcpAddressInfo() {}

    void TcpAddressInfo::setAddress(const TcpDissector* tcpDissectorPtr) {
        _srcAddrStr.assign(tcpDissectorPtr->getSourceAddrStr(false));
        _destAddrStr.assign(tcpDissectorPtr->getDestinationAddrStr(false));
        _hash = tcpDissectorPtr->getConnectionHash();
    }

} /* namespace amqpAnalyze */

std::ostream& operator<<(std::ostream& o, const amqpAnalyze::TcpAddressInfo& t) {
    o << t.srcAddress() << " -> " << t.destAddress() << " hash=0x" << std::hex << t.hash() << std::dec;
    return o;
}

std::ostream& operator<<(std::ostream& o, const amqpAnalyze::TcpAddressInfo* t) {
    o << t->srcAddress() << " -> " << t->destAddress() << " hash=0x" << std::hex << t->hash() << std::dec;
    return o;
}
