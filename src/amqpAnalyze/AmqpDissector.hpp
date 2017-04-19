/*
 * AmqpDissector.hpp
 *
 *  Created on: Apr 2, 2017
 *      Author: kvdr
 */

#ifndef SRC_AMQPANALYZE_AMQPDISSECTOR_HPP_
#define SRC_AMQPANALYZE_AMQPDISSECTOR_HPP_

#include <amqpAnalyze/amqp10/AmqpBlock.hpp>
#include <amqpAnalyze/WireDissector.hpp>

namespace amqpAnalyze {


    class AmqpDissector: public WireDissector {
    public:
        AmqpDissector(const WireDissector* parent,
                      uint64_t packetNum,
                      const struct pcap_pkthdr* pcapPacketHeaderPtr,
                      const uint8_t* packetPtr,
                      uint32_t packetOffs,
                      std::deque<WireDissector*>& protocolList,
                      std::size_t amqpDataSize);
        virtual ~AmqpDissector();

        void appendString(std::ostringstream& oss, size_t margin) const;
    protected:
        std::string _debugHexFrameData;
        amqp10::amqp_block_list_t _amqpBlockList;
    };

} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQPDISSECTOR_HPP_ */
