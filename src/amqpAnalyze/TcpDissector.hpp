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
#include <netinet/tcp.h>

struct pcap_pkthdr;

namespace amqpAnalyze
{
    class TcpConnection;

    class TcpDissector: public Dissector {
    public:
        TcpDissector(Packet* packetPtr, uint32_t dataOffs, Dissector* parent);
        virtual ~TcpDissector();

        void appendString(std::ostringstream& oss, size_t margin) const override;
        inline DissectorType_t dissectorType() const override { return DissectorType_t::DISSECTOR_TCP; }

        // TCP destination & source address
        std::string tcpDestinationAddrStr(bool colorFlag = g_optionsPtr->s_colorFlag) const;
        uint16_t tcpDestinationPort() const;
        std::string tcpSourceAddrStr(bool colorFlag = g_optionsPtr->s_colorFlag) const;
        uint16_t tcpSourcePort() const;

        // TCP flags
        std::string tcpFlagsAsStr() const;
        bool tcpAck() const;
        bool tcpFin() const;
        bool tcpSyn() const;

        // TCP sequence and ack sequence numbers
        uint32_t tcpSequenceNum() const;
        uint32_t tcpAckSequenceNum() const;

        // Functions for mapping TCP connections
        uint32_t connectionIndex() const; // index in global list
        std::size_t hash() const; // calculate hash for this instance
        bool replyFlag() const;
        const TcpConnection* tcpConnection() const;
        TcpConnection* tcpConnection();

    protected:
        struct tcphdr _tcpHeader;
        uint32_t _hdrSizeBytes;
        uint32_t _remainingDataLength;
        TcpConnection* _tcpConnection;
        bool _replyFlag;

        std::string hashStr() const;
    };

} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_TCPDISSECTOR_HPP_ */
