/*
 * Dissector.hpp
 *
 *  Created on: Apr 2, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE__WIREDISSECTOR_HPP_
#define SRC_AMQPANALYZE__WIREDISSECTOR_HPP_

#include <amqpAnalyze/FwdDecls.hpp>
#include <map>
#include <sstream>

namespace amqpAnalyze
{

    enum class DissectorType_t:uint8_t {
        DISSECTOR_IP4,
        DISSECTOR_IP6,
        DISSECTOR_TCP,
        DISSECTOR_AMQP
    };

    class Dissector {
    public:
        Dissector(const Dissector* parent,
                  uint64_t packetNum,
                  uint32_t packetOffs,
                  DissectorList_t& protocolList);
        virtual ~Dissector();

        virtual void appendString(std::ostringstream& oss, size_t margin) const = 0;
        const char* name() const;
        virtual DissectorType_t dissectorType() const = 0;

    protected:
        const Dissector* _parent;
        const uint64_t _packetNum;
        const uint32_t _packetOffs;
        DissectorList_t& _protocolList;
        static std::map<DissectorType_t, const char*> s_DissectorTypeNames;
    };

} /* namespace amqp_analyze */

#endif /* SRC_AMQPANALYZE__WIREDISSECTOR_HPP_ */
