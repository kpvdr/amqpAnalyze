/*
 * IpDissector.cpp
 *
 *  Created on: Apr 3, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/IpDissector.hpp>

namespace amqpAnalyze
{

    IpDissector::IpDissector(const Options* optionsPtr,
                             uint64_t packetNum,
                             const uint32_t packetOffs,
                             DissectorList_t& protocolList):
            Dissector(optionsPtr, nullptr, packetNum, packetOffs, protocolList)
    {}

    IpDissector::~IpDissector() {}

} /* namespace amqpAnalyze */
