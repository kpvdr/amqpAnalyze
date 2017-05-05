/*
 * Ip4Dissector.hpp
 *
 *  Created on: Apr 2, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_IP4DISSECTOR_HPP_
#define SRC_AMQPANALYZE_IP4DISSECTOR_HPP_

#include <amqpAnalyze/IpDissector.hpp>
#include <netinet/ip.h>

struct pcap_pkthdr;

namespace amqpAnalyze
{

    class Ip4Dissector: public IpDissector
    {
    public:
        Ip4Dissector(Packet* packetPtr, uint32_t dataOffs);
        virtual ~Ip4Dissector();

        void appendString(std::ostringstream& oss, size_t margin) const override;
        inline DissectorType_t dissectorType() const override { return DissectorType_t::DISSECTOR_IP4; }
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
