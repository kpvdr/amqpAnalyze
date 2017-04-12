/*
 * Packet.hpp
 *
 *  Created on: Mar 31, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_PACKET_HPP_
#define SRC_AMQPANALYZE_PACKET_HPP_

#include <sstream>
#include <deque>

struct pcap_pkthdr;

namespace amqpAnalyze
{

    class WireDissector;

    class Packet
    {
    protected:
        const uint64_t _packetNum;
        const struct timeval _relativeTimestamp;
        const uint32_t _captureLength;
        const uint32_t _packetLength;
        std::deque<WireDissector*> _protocolList;
    public:
        Packet(uint8_t* userDataPtr,
               const struct pcap_pkthdr* pcapPacketHeaderPtr,
               const uint8_t* packetPtr,
               uint64_t packetNum,
               const struct timeval& relativeTimestamp);
        virtual ~Packet();
        std::string toString(std::size_t margin = 0) const;
        std::string connectionIndex() const;
    };

} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_PACKET_HPP_ */
