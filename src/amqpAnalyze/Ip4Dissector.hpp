/*
 * IpDissector.hpp
 *
 *  Created on: Apr 2, 2017
 *      Author: kvdr
 */

#ifndef SRC_AMQPANALYZE_IP4DISSECTOR_HPP_
#define SRC_AMQPANALYZE_IP4DISSECTOR_HPP_

#include <amqpAnalyze/IpDissector.hpp>

#include <netinet/ip.h>
#include <vector>

namespace amqpAnalyze {

class Ip4Dissector: public IpDissector {
protected:
	struct ip _ip4Header;
public:
	Ip4Dissector(const struct pcap_pkthdr* pcapPacketHeaderPtr,
	             const uint8_t* packetPtr,
                 const uint32_t packetOffs,
	             std::deque<WireDissector*>& protocolList);
	virtual ~Ip4Dissector();
	void appendString(std::ostringstream& oss, size_t margin) const;

	uint32_t getSourceAddr() const;
	uint32_t getDestinationAddr() const;
	std::string getSourceAddrStr() const;
    std::string getDestinationAddrStr() const;
    bool isIp6() const;
};

} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_IP4DISSECTOR_HPP_ */
