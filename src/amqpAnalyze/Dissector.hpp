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
        Dissector(Packet* packetPtr, uint32_t dataOffs, Dissector* parent);
        virtual ~Dissector();

        void addError(const Error* errorPtr);
        void appendErrors(std::ostringstream& oss, size_t margin) const;
        virtual void appendString(std::ostringstream& oss, size_t margin) const = 0;
        const char* name() const;
        virtual DissectorType_t dissectorType() const = 0;

    protected:
        Packet* _packetPtr;
        const uint32_t _dataOffs;
        Dissector* _parent;
        static std::map<DissectorType_t, const char*> s_DissectorTypeNames;
        ErrorPtrList_t _errorList;
    };

} /* namespace amqp_analyze */

#endif /* SRC_AMQPANALYZE__WIREDISSECTOR_HPP_ */
