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

struct pcap_pkthdr;

namespace amqpAnalyze
{


    class AmqpDissector: public WireDissector {
    public:
        AmqpDissector(const Options* optionsPtr,
                      const WireDissector* parent,
                      uint64_t packetNum,
                      const struct pcap_pkthdr* pcapPacketHeaderPtr,
                      const uint8_t* packetPtr,
                      uint32_t packetOffs,
                      protocol_list_t& protocolList,
                      std::size_t amqpDataSize);
        virtual ~AmqpDissector();

        void appendString(std::ostringstream& oss, size_t margin) const override;
        inline dissector_t dissectorType() const override { return dissector_t::DISSECTOR_AMQP; }

    protected:
        std::string _debugHexFrameData;
        amqp10::amqp_block_list_t _amqpBlockList;
    };

} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQPDISSECTOR_HPP_ */
