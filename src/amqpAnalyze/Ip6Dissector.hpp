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
	Ip6Dissector(Packet* packetPtr, const uint32_t dataOffs);
	virtual ~Ip6Dissector();

	void appendString(std::ostringstream& oss, size_t margin) const override;
    void destinationAddr(std::array<uint32_t, 4>& destinationAddr) const;
    std::string destinationAddrStr() const override;
	inline DissectorType_t dissectorType() const override { return DissectorType_t::DISSECTOR_IP6; }
    inline bool isIp6() const { return true; }
    void sourceAddr(std::array<uint32_t, 4>& sourceAddr) const;
    std::string sourceAddrStr() const override;
};

} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_IP6DISSECTOR_HPP_ */
