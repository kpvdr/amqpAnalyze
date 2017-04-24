/*
 * Packet.hpp
 *
 *  Created on: Mar 31, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_PACKET_HPP_
#define SRC_AMQPANALYZE_PACKET_HPP_

#include <amqpAnalyze/FwdDecls.hpp>

struct pcap_pkthdr;

namespace amqpAnalyze
{

    class Packet
    {
    public:
        Packet(const Options* optionsPtr,
               const struct pcap_pkthdr* pcapPacketHeaderPtr,
               const uint8_t* packetPtr,
               uint64_t packetNum,
               const struct timeval& relativeTimestamp);
        virtual ~Packet();
        std::string toString(std::size_t margin = 0) const;
        std::string connectionIndex() const;

    protected:
        const Options* _optionsPtr;
        const uint64_t _packetNum;
        const struct timeval _relativeTimestamp;
        const uint32_t _captureLength;
        const uint32_t _packetLength;
        DissectorList_t _protocolList;
    };

} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_PACKET_HPP_ */
