/*
 * Ip6Dissector.hpp
 *
 *  Created on: Apr 2, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_IP6DISSECTOR_HPP_
#define SRC_AMQPANALYZE_IP6DISSECTOR_HPP_

#include <amqpAnalyze/IpDissector.hpp>
#include <array>
#include <netinet/ip6.h>

struct pcap_pkthdr;

namespace amqpAnalyze {

class Ip6Dissector: public IpDissector {
protected:
	struct ip6_hdr _ip6Header;
public:
	Ip6Dissector(const Options* optionsPtr,
                 uint64_t packetNum,
	             const struct pcap_pkthdr* pcapPacketHeaderPtr,
	             const uint8_t* packetPtr,
                 const uint32_t packetOffs,
                 DissectorList_t& protocolList);
	virtual ~Ip6Dissector();

	void appendString(std::ostringstream& oss, size_t margin) const override;
	inline dissector_t dissectorType() const override { return dissector_t::DISSECTOR_IP6; }
	void getSourceAddr(std::array<uint32_t, 4>& sourceAddr) const;
    void getDestinationAddr(std::array<uint32_t, 4>& destinationAddr) const;
    std::string getSourceAddrStr() const override;
    std::string getDestinationAddrStr() const override;
    inline bool isIp6() const { return true; }
};

} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_IP6DISSECTOR_HPP_ */
