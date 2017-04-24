/*
 * Dissector.cpp
 *
 *  Created on: Apr 2, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/Dissector.hpp>

namespace amqpAnalyze {

Dissector::Dissector(const Options* optionsPtr,
                             const Dissector* parent,
                             uint64_t packetNum,
                             uint32_t packetOffs,
                             DissectorList_t& protocolList):
       _optionsPtr(optionsPtr),
       _parent(parent),
       _packetNum(packetNum),
       _packetOffs(packetOffs),
       _protocolList(protocolList)
{}

Dissector::~Dissector() {}

} /* namespace amqp_analyze */
