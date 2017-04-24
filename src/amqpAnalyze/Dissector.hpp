/*
 * Dissector.hpp
 *
 *  Created on: Apr 2, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE__WIREDISSECTOR_HPP_
#define SRC_AMQPANALYZE__WIREDISSECTOR_HPP_

#include <amqpAnalyze/FwdDecls.hpp>
#include <sstream>

namespace amqpAnalyze
{

    enum class dissector_t:uint8_t {
        DISSECTOR_NONE = 0,
        DISSECTOR_IP4,
        DISSECTOR_IP6,
        DISSECTOR_TCP,
        DISSECTOR_AMQP
    };

    class Dissector {
    public:
        Dissector(const Options* optionsPtr,
                      const Dissector* parent,
                      uint64_t packetNum,
                      uint32_t packetOffs,
                      DissectorList_t& protocolList);
        virtual ~Dissector();
        virtual void appendString(std::ostringstream& oss, size_t margin) const = 0;
        virtual dissector_t dissectorType() const = 0;

    protected:
        const Options* _optionsPtr;
        const Dissector* _parent;
        const uint64_t _packetNum;
        const uint32_t _packetOffs;
        DissectorList_t& _protocolList;
    };

} /* namespace amqp_analyze */

#endif /* SRC_AMQPANALYZE__WIREDISSECTOR_HPP_ */
