/*
 * IpDissector.cpp
 *
 *  Created on: Apr 3, 2017
 *      Author: kpvdr
 */

#include "IpDissector.hpp"

namespace amqpAnalyze
{

    IpDissector::IpDissector(Packet* packetPtr, uint32_t packetOffs):
            Dissector(packetPtr, packetOffs, nullptr)
    {}

    IpDissector::~IpDissector() {}

} /* namespace amqpAnalyze */
