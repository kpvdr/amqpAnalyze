/*
 * Dissector.cpp
 *
 *  Created on: Apr 2, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/Dissector.hpp>

namespace amqpAnalyze
{

    Dissector::Dissector(const Dissector* parent,
                         uint64_t packetNum,
                         uint32_t packetOffs,
                         DissectorList_t& protocolList):
           _parent(parent),
           _packetNum(packetNum),
           _packetOffs(packetOffs),
           _protocolList(protocolList)
    {}

    Dissector::~Dissector() {}

    const char* Dissector::name() const {
        return s_DissectorTypeNames[dissectorType()];
    }

    // static
    std::map<DissectorType_t, const char*> Dissector::s_DissectorTypeNames = {
        {DissectorType_t::DISSECTOR_IP4, "Ip4Dissector"},
        {DissectorType_t::DISSECTOR_IP6, "Ip6Dissector"},
        {DissectorType_t::DISSECTOR_TCP, "TcpDissector"},
        {DissectorType_t::DISSECTOR_AMQP, "AmqpDissector"}
    };

} /* namespace amqp_analyze */
