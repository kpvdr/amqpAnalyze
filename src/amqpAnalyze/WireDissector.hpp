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

namespace amqpAnalyze
{
    class Options;
    class WireDissector;
    typedef std::deque<WireDissector*> protocol_list_t;

    enum class dissector_t:uint8_t {
        DISSECTOR_NONE = 0,
        DISSECTOR_IP4,
        DISSECTOR_IP6,
        DISSECTOR_TCP,
        DISSECTOR_AMQP
    };

    class WireDissector {
    public:
        WireDissector(const Options* optionsPtr,
                      const WireDissector* parent,
                      uint64_t packetNum,
                      uint32_t packetOffs,
                      protocol_list_t& protocolList);
        virtual ~WireDissector();
        virtual void appendString(std::ostringstream& oss, size_t margin) const = 0;
        virtual dissector_t dissectorType() const = 0;

    protected:
        const Options* _optionsPtr;
        const WireDissector* _parent;
        const uint64_t _packetNum;
        const uint32_t _packetOffs;
        protocol_list_t& _protocolList;
    };

} /* namespace amqp_analyze */

#endif /* SRC_AMQPANALYZE__WIREDISSECTOR_HPP_ */
