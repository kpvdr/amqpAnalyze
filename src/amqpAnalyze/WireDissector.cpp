/*
 * WireProtocol.cpp
 *
 *  Created on: Apr 2, 2017
 *      Author: kvdr
 */

#include <amqpAnalyze/WireDissector.hpp>

namespace amqpAnalyze {

WireDissector::WireDissector(const WireDissector* parent,
                             uint64_t packetNum,
                             const struct pcap_pkthdr* pcapPacketHeaderPtr,
                             const uint8_t* packetPtr,
                             uint32_t packetOffs,
                             dissector_t dissectorType,
                             std::deque<WireDissector*>& protocolList):
       _parent(parent),
       _packetNum(packetNum),
       _packetOffs(packetOffs),
       _dissectorType(dissectorType),
       _protocolList(protocolList)
{}

WireDissector::~WireDissector() {}

dissector_t WireDissector::dissectorType() const {
    return _dissectorType;
}


} /* namespace amqp_analyze */
