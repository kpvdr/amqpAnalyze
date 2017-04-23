/*
 * IpDissector.hpp
 *
 *  Created on: Apr 3, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_IPDISSECTOR_HPP_
#define SRC_AMQPANALYZE_IPDISSECTOR_HPP_

#include <amqpAnalyze/WireDissector.hpp>

namespace amqpAnalyze
{

    class IpDissector: public WireDissector
    {
    public:
        IpDissector(const Options* optionsPtr,
                    uint64_t packetNum,
                    const uint32_t packetOffs,
                    protocol_list_t& protocolList);
        virtual ~IpDissector();

        virtual std::string getSourceAddrStr() const = 0;
        virtual std::string getDestinationAddrStr() const = 0;
        virtual bool isIp6() const = 0;
    };

} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_IPDISSECTOR_HPP_ */
