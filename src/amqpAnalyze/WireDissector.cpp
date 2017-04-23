/*
 * WireProtocol.cpp
 *
 *  Created on: Apr 2, 2017
 *      Author: kvdr
 */

#include <amqpAnalyze/WireDissector.hpp>

namespace amqpAnalyze {

WireDissector::WireDissector(const Options* optionsPtr,
                             const WireDissector* parent,
                             uint64_t packetNum,
                             uint32_t packetOffs,
                             protocol_list_t& protocolList):
       _optionsPtr(optionsPtr),
       _parent(parent),
       _packetNum(packetNum),
       _packetOffs(packetOffs),
       _protocolList(protocolList)
{}

WireDissector::~WireDissector() {}

} /* namespace amqp_analyze */
