/*
 * IpDissector.cpp
 *
 *  Created on: Apr 3, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/IpDissector.hpp>

namespace amqpAnalyze
{

    IpDissector::IpDissector(uint64_t packetNum,
                             const struct pcap_pkthdr* pcapPacketHeaderPtr,
                             const uint8_t* packetPtr,
                             const uint32_t packetOffs,
                             dissector_t dissectorType,
                             std::deque<WireDissector*>& protocolList):
            WireDissector(nullptr, packetNum, pcapPacketHeaderPtr, packetPtr, packetOffs, dissectorType, protocolList)
    {}

    IpDissector::~IpDissector() {}

} /* namespace amqpAnalyze */
