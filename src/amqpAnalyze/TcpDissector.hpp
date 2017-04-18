/*
 * TcpDissector.hpp
 *
 *  Created on: Apr 2, 2017
 *      Author: kvdr
 */

#ifndef SRC_AMQPANALYZE_TCPDISSECTOR_HPP_
#define SRC_AMQPANALYZE_TCPDISSECTOR_HPP_

#include <amqpAnalyze/WireDissector.hpp>

#include <netinet/tcp.h>
#include <vector>

namespace amqpAnalyze {

class IpDissector;

class TcpDissector: public WireDissector {
protected:
    struct tcphdr _tcpHeader;
    uint32_t _hdrSizeBytes;
    uint32_t _remainingDataLength;
public:
	TcpDissector(const WireDissector* parent,
	             uint64_t packetNum,
	             const struct pcap_pkthdr* pcapPacketHeaderPtr,
	             const uint8_t* packetPtr,
                 const uint32_t packetOffs,
	             std::deque<WireDissector*>& protocolList);
	virtual ~TcpDissector();
	void appendString(std::ostringstream& oss, size_t margin) const;

	std::string getSourceTcpAddrStr() const;
    std::string getDestinationAddrStr() const;
    std::string getConnectionIndex() const;
	u_int16_t getSourcePort() const;
    u_int16_t getDestinationPort() const;
    std::string getFlagsAsString() const;
    uint32_t getSequence() const;
    uint32_t getAckSequence() const;
    std::size_t getConnectionHash() const;
};

} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_TCPDISSECTOR_HPP_ */
