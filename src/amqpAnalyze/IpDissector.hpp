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
        IpDissector(Packet* packetPtr, uint32_t packetOffs);
        virtual ~IpDissector();

        virtual std::string sourceAddrStr() const = 0;
        virtual std::string destinationAddrStr() const = 0;
        virtual bool isIp6() const = 0;
    };

} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_IPDISSECTOR_HPP_ */
