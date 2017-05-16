/*
 * Dissector.cpp
 *
 *  Created on: Apr 2, 2017
 *      Author: kpvdr
 */

#include "Dissector.hpp"

#include <amqpAnalyze/Color.hpp>
#include <amqpAnalyze/Error.hpp>
#include <amqpAnalyze/Options.hpp>

namespace amqpAnalyze
{

    Dissector::Dissector(Packet* packetPtr, uint32_t dataOffs, Dissector* parent):
           _packetPtr(packetPtr),
           _dataOffs(dataOffs),
           _parent(parent),
           _errorList()
    {}

    Dissector::~Dissector() {
        for (ErrorPtrListItr_t i=_errorList.begin(); i!=_errorList.end(); ++i) {
            delete (*i);
        }
        _errorList.clear();
    }

    void Dissector::addError(const Error* errorPtr) {
        _errorList.push_back(errorPtr);
    }

    void Dissector::appendErrors(std::ostringstream& oss, size_t margin) const {
        for (ErrorPtrListCitr_t i=_errorList.cbegin(); i!=_errorList.cend(); ++i) {
            oss << "\n" << std::string(margin, ' ') << Color::color(DisplayColorType_t::MSG_ERROR, (*i)->what());
        }
    }

    const char* Dissector::name() const {
        return s_DissectorTypeNames[dissectorType()];
    }

    // static
    std::map<DissectorType_t, const char*> Dissector::s_DissectorTypeNames = {
        {DissectorType_t::DISSECTOR_IP4, "Ip4Dissector"},
        {DissectorType_t::DISSECTOR_IP6, "Ip6Dissector"},
        {DissectorType_t::DISSECTOR_TCP, "TcpDissector"},
        {DissectorType_t::DISSECTOR_AMQP, "AmqpDissector"}
    };

} /* namespace amqp_analyze */
