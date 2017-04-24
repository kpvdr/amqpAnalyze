/*
 * TcpDissector.hpp
 *
 *  Created on: Apr 2, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_TCPDISSECTOR_HPP_
#define SRC_AMQPANALYZE_TCPDISSECTOR_HPP_

#include <amqpAnalyze/Dissector.hpp>
#include <netinet/tcp.h>

struct pcap_pkthdr;

namespace amqpAnalyze
{

    class IpDissector;

    class TcpDissector: public Dissector {
    public:
        TcpDissector(const Options* optionsPtr,
                     const Dissector* parent,
                     uint64_t packetNum,
                     const struct pcap_pkthdr* pcapPacketHeaderPtr,
                     const uint8_t* packetPtr,
                     const uint32_t packetOffs,
                     DissectorList_t& protocolList);
        virtual ~TcpDissector();

        void appendString(std::ostringstream& oss, size_t margin) const override;
        inline dissector_t dissectorType() const override { return dissector_t::DISSECTOR_TCP; }
        std::string getSourceTcpAddrStr() const;
        std::string getDestinationAddrStr() const;
        std::string getConnectionIndex() const;
        uint16_t getSourcePort() const;
        uint16_t getDestinationPort() const;
        std::string getFlagsAsString() const;
        uint32_t getSequence() const;
        uint32_t getAckSequence() const;
        std::size_t getConnectionHash() const;

    protected:
        struct tcphdr _tcpHeader;
        uint32_t _hdrSizeBytes;
        uint32_t _remainingDataLength;
    };

} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_TCPDISSECTOR_HPP_ */
