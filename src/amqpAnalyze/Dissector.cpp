/*
 * Dissector.cpp
 *
 *  Created on: Apr 2, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/Dissector.hpp>

namespace amqpAnalyze
{

    Dissector::Dissector(Packet* packetPtr, uint32_t dataOffs, const Dissector* parent):
           _packetPtr(packetPtr),
           _dataOffs(dataOffs),
           _parent(parent)
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
