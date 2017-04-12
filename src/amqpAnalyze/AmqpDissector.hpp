/*
 * AmqpDissector.hpp
 *
 *  Created on: Apr 2, 2017
 *      Author: kvdr
 */

#ifndef SRC_AMQPANALYZE_AMQPDISSECTOR_HPP_
#define SRC_AMQPANALYZE_AMQPDISSECTOR_HPP_

#include <amqpAnalyze/TcpDissector.hpp>
#include <amqpAnalyze/WireDissector.hpp>

#include <vector>

namespace amqpAnalyze {
    namespace amqp10 {
        class FrameBase;
    }

class AmqpDissector: public WireDissector {
protected:
    const TcpDissector* _tcpDissectorPtr;
    std::string _debugHexFrameData;
    std::deque<amqp10::FrameBase*> _amqpFrameList;
public:
	AmqpDissector(const struct pcap_pkthdr* pcapPacketHeaderPtr,
	              const uint8_t* packetPtr,
	              uint32_t packetOffs,
	              std::deque<WireDissector*>& protocolList,
	              std::size_t amqpDataSize,
	              const TcpDissector* tcpDissectorPtr);
	virtual ~AmqpDissector();
	void appendString(std::ostringstream& oss, size_t margin) const;
};

} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQPDISSECTOR_HPP_ */
