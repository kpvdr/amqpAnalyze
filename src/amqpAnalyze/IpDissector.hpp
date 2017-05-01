/*
 * IpDissector.hpp
 *
 *  Created on: Apr 3, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_IPDISSECTOR_HPP_
#define SRC_AMQPANALYZE_IPDISSECTOR_HPP_

#include <amqpAnalyze/Dissector.hpp>

namespace amqpAnalyze
{

    class IpDissector: public Dissector
    {
    public:
        IpDissector(uint64_t packetNum,
                    const uint32_t packetOffs,
                    DissectorList_t& protocolList);
        virtual ~IpDissector();

        virtual std::string getSourceAddrStr() const = 0;
        virtual std::string getDestinationAddrStr() const = 0;
        virtual bool isIp6() const = 0;
    };

} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_IPDISSECTOR_HPP_ */
