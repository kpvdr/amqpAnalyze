/*
 * TcpDissector.hpp
 *
 *  Created on: Apr 2, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_TCPDISSECTOR_HPP_
#define SRC_AMQPANALYZE_TCPDISSECTOR_HPP_

#include <amqpAnalyze/Dissector.hpp>
#include <amqpAnalyze/Options.hpp>
#include <amqpAnalyze/TcpAddressInfo.hpp>
#include <netinet/tcp.h>

struct pcap_pkthdr;

namespace amqpAnalyze
{

    class TcpDissector: public Dissector {
    public:
        TcpDissector(Packet* packetPtr, uint32_t dataOffs, const Dissector* parent);
        virtual ~TcpDissector();

        void appendString(std::ostringstream& oss, size_t margin) const override;
        inline DissectorType_t dissectorType() const override { return DissectorType_t::DISSECTOR_TCP; }
        std::string getSourceAddrStr(bool colorFlag = g_optionsPtr->s_colorFlag) const;
        std::string getDestinationAddrStr(bool colorFlag = g_optionsPtr->s_colorFlag) const;
        std::string getConnectionHashStr() const;
        uint32_t getConnectionIndex() const;
        uint16_t getSourcePort() const;
        uint16_t getDestinationPort() const;
        std::string getFlagsAsString() const;
        bool ack() const;
        bool fin() const;
        bool syn() const;
        uint32_t getSequence() const;
        uint32_t getAckSequence() const;
        std::size_t getConnectionHash() const;
        const TcpAddressInfo& getTcpAddressInfo() const;

    protected:
        struct tcphdr _tcpHeader;
        uint32_t _hdrSizeBytes;
        uint32_t _remainingDataLength;
        uint32_t _connectionIndex;
        TcpAddressInfo _tcpAddressInfo;
    };

} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_TCPDISSECTOR_HPP_ */
