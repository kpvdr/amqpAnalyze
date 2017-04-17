/*
 * Ip6Dissector.hpp
 *
 *  Created on: Apr 2, 2017
 *      Author: kvdr
 */

#ifndef SRC_AMQPANALYZE_IP6DISSECTOR_HPP_
#define SRC_AMQPANALYZE_IP6DISSECTOR_HPP_

#include <amqpAnalyze/IpDissector.hpp>

#include <array>
#include <deque>
#include <netinet/ip6.h>

namespace amqpAnalyze {

class Ip6Dissector: public IpDissector {
protected:
	struct ip6_hdr _ip6Header;
public:
	Ip6Dissector(uint64_t packetNum,
	             const struct pcap_pkthdr* pcapPacketHeaderPtr,
	             const uint8_t* packetPtr,
                 const uint32_t packetOffs,
	             std::deque<WireDissector*>& protocolList);
	virtual ~Ip6Dissector();
	void appendString(std::ostringstream& oss, size_t margin) const;

	void getSourceAddr(std::array<uint32_t, 4>& sourceAddr) const;
    void getDestinationAddr(std::array<uint32_t, 4>& destinationAddr) const;
    std::string getSourceAddrStr() const;
    std::string getDestinationAddrStr() const;
    bool isIp6() const;
};

} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_IP6DISSECTOR_HPP_ */
