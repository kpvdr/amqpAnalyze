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
        Packet(uint64_t packetNum,
               const struct pcap_pkthdr* pcapPacketHeaderPtr,
               const uint8_t* dataPtr,
               const struct timeval& relativeTimestamp);
        virtual ~Packet();
        void addDissector(const Dissector* dissectorPtr);
        inline const uint8_t* dataPtr() const { return _dataPtr; }
        inline uint64_t packetNum() const { return _packetNum; }
        inline const struct pcap_pkthdr* pcapPacketHeaderPtr() const { return _pcapPacketHeaderPtr; }

        std::string toString(std::size_t margin = 0) const;
        uint32_t connectionIndex() const;

    protected:
        const uint64_t _packetNum;
        const struct pcap_pkthdr* _pcapPacketHeaderPtr;
        const uint8_t* _dataPtr;
        const struct timeval _relativeTimestamp;
        const uint32_t _captureLength;
        const uint32_t _packetLength;
        DissectorList_t _dissectorList;
    };

} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_PACKET_HPP_ */
