/*
 * WireProtocol.hpp
 *
 *  Created on: Apr 2, 2017
 *      Author: kvdr
 */

#ifndef SRC_AMQPANALYZE__WIREDISSECTOR_HPP_
#define SRC_AMQPANALYZE__WIREDISSECTOR_HPP_

#include <sstream>
#include <deque>

struct pcap_pkthdr;

namespace amqpAnalyze {

enum dissector_t:uint8_t { DISSECTOR_NONE = 0, DISSECTOR_IP4, DISSECTOR_IP6, DISSECTOR_TCP, DISSECTOR_AMQP };

class WireDissector {
protected:
    const WireDissector* _parent;
    const uint64_t _packetNum;
    const uint32_t _packetOffs;
    const dissector_t _dissectorType;
	std::deque<WireDissector*>& _protocolList;
public:
	WireDissector(const WireDissector* parent,
	              uint64_t packetNum,
	              const struct pcap_pkthdr* pcapPacketHeaderPtr,
	              const uint8_t* packetPtr,
	              uint32_t packetOffs,
                  dissector_t dissectorType,
	              std::deque<WireDissector*>& protocolList);
	virtual ~WireDissector();
	virtual void appendString(std::ostringstream& oss, size_t margin) const = 0;
	dissector_t dissectorType() const;
};

} /* namespace amqp_analyze */

#endif /* SRC_AMQPANALYZE__WIREDISSECTOR_HPP_ */
