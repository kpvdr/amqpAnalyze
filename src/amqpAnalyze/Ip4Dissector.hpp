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

struct pcap_pkthdr;

namespace amqpAnalyze
{

    class Ip4Dissector: public IpDissector
    {
    public:
        Ip4Dissector(const Options* optionsPtr,
                     uint64_t packetNum,
                     const struct pcap_pkthdr* pcapPacketHeaderPtr,
                     const uint8_t* packetPtr,
                     const uint32_t packetOffs,
                     protocol_list_t& protocolList);
        virtual ~Ip4Dissector();

        void appendString(std::ostringstream& oss, size_t margin) const override;
        inline dissector_t dissectorType() const override { return dissector_t::DISSECTOR_IP4; }
        uint32_t getSourceAddr() const;
        uint32_t getDestinationAddr() const;
        std::string getSourceAddrStr() const override;
        std::string getDestinationAddrStr() const override;
        inline bool isIp6() const override { return false; }

    protected:
        struct ip _ip4Header;
    };

} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_IP4DISSECTOR_HPP_ */
