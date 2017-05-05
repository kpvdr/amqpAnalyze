/*
 * AmqpDissector.hpp
 *
 *  Created on: Apr 2, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_AMQPDISSECTOR_HPP_
#define SRC_AMQPANALYZE_AMQPDISSECTOR_HPP_

#include <amqpAnalyze/amqp10/AmqpBlock.hpp>
#include <amqpAnalyze/Dissector.hpp>

struct pcap_pkthdr;

namespace amqpAnalyze
{


    class AmqpDissector: public Dissector {
    public:
        AmqpDissector(Packet* packetPtr, uint32_t dataOffs, Dissector* parent, std::size_t amqpDataSize);
        virtual ~AmqpDissector();

        void appendString(std::ostringstream& oss, size_t margin) const override;
        inline DissectorType_t dissectorType() const override { return DissectorType_t::DISSECTOR_AMQP; }

    protected:
        std::string _debugHexFrameData;
        amqp10::AmqpBlockList_t _amqpBlockList;
    };

} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQPDISSECTOR_HPP_ */
