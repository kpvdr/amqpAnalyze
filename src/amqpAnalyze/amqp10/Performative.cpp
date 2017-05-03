/*
 * FrameHeader.cpp
 *
 *  Created on: Apr 3, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/amqp10/Performative.hpp>

#include <amqpAnalyze/amqp10/FieldType.hpp>
#include <amqpAnalyze/amqp10/ProvidesRequires.hpp>
#include <amqpAnalyze/amqp10/Type.hpp>
#include <amqpAnalyze/Color.hpp>
#include <amqpAnalyze/Options.hpp>
#include <cstring>
#include <iomanip>

namespace amqpAnalyze
{
    namespace amqp10
    {

        Performative::Performative(uint64_t packetNum, std::size_t dataOffset, AmqpList* fieldListPtr):
                AmqpBlock(packetNum, dataOffset),
                _fieldListPtr(fieldListPtr)
        {}

        Performative::~Performative() {
            for (AmqpListItr_t i=_fieldListPtr->value().begin(); i!=_fieldListPtr->value().end(); ++i) {
                delete *i;
            }
            _fieldListPtr->value().clear();
            delete _fieldListPtr;
        }

        std::ostringstream& Performative::appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const {
            if (!ignoreFirstMargin) oss << "\n" << std::string(margin, ' ') << "[" << std::hex << std::setfill('0') << std::setw(4) << _dataOffset << "] ";
            oss  << "p " << Color::color(DisplayColorType_t::AMQP_PERFORMATIVE, typeStr());
            if (_fieldListPtr != nullptr) {
                _fieldListPtr->appendString(oss, margin + 9 + std::strlen(typeStr()), false, g_optionsPtr->s_colorFlag);
            }
            if (g_optionsPtr->s_showStateFlag && !_stateStr.empty()) oss << " | " << Color::color(DisplayColorType_t::AMQP_STATE_SESSION, _stateStr);
            return appendStringEpilog(oss, margin + 9);
        }

        void Performative::validate() {
            // Validate field list
            if (_fieldListPtr != nullptr) {
                _fieldListPtr->validate(fieldTypeList(), &AmqpBlock::addError, this);
            }
        }

        // static
        std::map<PerformativeType_t, const char*> Performative::s_performativeTypeName = {
            {PerformativeType_t::OPEN, "open"},
            {PerformativeType_t::BEGIN, "begin"},
            {PerformativeType_t::ATTACH, "attach"},
            {PerformativeType_t::FLOW, "flow"},
            {PerformativeType_t::TRANSFER, "transfer"},
            {PerformativeType_t::DISPOSITION, "disposition"},
            {PerformativeType_t::DETACH, "detach"},
            {PerformativeType_t::END, "end"},
            {PerformativeType_t::CLOSE, "close"}
        };


        //-- class AmqpOpen ---

        AmqpOpen::AmqpOpen(uint64_t packetNum, std::size_t dataOffset, AmqpList* fieldListPtr):
            Performative(packetNum, dataOffset, fieldListPtr)
        {}

        AmqpOpen::~AmqpOpen() {}

        // static
        const FieldTypeList_t AmqpOpen::s_fieldTypeList = {
            FieldType("container-id", AmqpPrimitiveType_t::STRING_TYPE, true, false),
            FieldType("hostname", AmqpPrimitiveType_t::STRING_TYPE, false, false),
            FieldType("max-frame-size", AmqpPrimitiveType_t::UINT_TYPE, false, false),
            FieldType("channel-max", AmqpPrimitiveType_t::USHORT_TYPE, false, false),
            FieldType("idle-time-out", AmqpPrimitiveType_t::MILLISECONDS_TYPE, false, false),
            FieldType("outgoing-locales", AmqpPrimitiveType_t::LANGUAGE_TAG_TYPE, false, true),
            FieldType("incoming-locales", AmqpPrimitiveType_t::LANGUAGE_TAG_TYPE, false, true),
            FieldType("offered-capabilities", AmqpPrimitiveType_t::SYMBOL_TYPE, false, true),
            FieldType("desired-capabilities", AmqpPrimitiveType_t::SYMBOL_TYPE, false, true),
            FieldType("properties", AmqpPrimitiveType_t::FIELDS_TYPE, false, false)
        };

        void AmqpOpen::validate() {
            Performative::validate();
        }


        //-- class AmqpBegin ---

        AmqpBegin::AmqpBegin(uint64_t packetNum, std::size_t dataOffset, AmqpList* fieldListPtr):
            Performative(packetNum, dataOffset, fieldListPtr)
        {}

        AmqpBegin::~AmqpBegin() {}

        // static
        const FieldTypeList_t AmqpBegin::s_fieldTypeList = {
            FieldType("remote-channel", AmqpPrimitiveType_t::USHORT_TYPE, false, false),
            FieldType("next-outgoing-id", AmqpPrimitiveType_t::TRANSFER_NUMBER_TYPE, true, false),
            FieldType("incoming-window", AmqpPrimitiveType_t::UINT_TYPE, true, false),
            FieldType("outgoing-window", AmqpPrimitiveType_t::UINT_TYPE, true, false),
            FieldType("handle-max", AmqpPrimitiveType_t::HANDLE_TYPE, false, false),
            FieldType("offered-capabilities", AmqpPrimitiveType_t::SYMBOL_TYPE, false, true),
            FieldType("desired-capabilities", AmqpPrimitiveType_t::SYMBOL_TYPE, false, true),
            FieldType("properties", AmqpPrimitiveType_t::FIELDS_TYPE, false, false)
        };

        void AmqpBegin::validate() {
            Performative::validate();
        }


        //-- class AmqpAttach ---

        AmqpAttach::AmqpAttach(uint64_t packetNum, std::size_t dataOffset, AmqpList* fieldListPtr):
            Performative(packetNum, dataOffset, fieldListPtr)
        {}

        AmqpAttach::~AmqpAttach() {}

        // static
        const FieldTypeList_t AmqpAttach::s_fieldTypeList = {
            FieldType("name", AmqpPrimitiveType_t::STRING_TYPE, true, false),
            FieldType("handle", AmqpPrimitiveType_t::HANDLE_TYPE, true, false),
            FieldType("role", AmqpPrimitiveType_t::ROLE_TYPE, true, false),
            FieldType("snd-settle-mode", AmqpPrimitiveType_t::SENDER_SETTLE_MODE_TYPE, false, false),
            FieldType("rcv-settle-mode", AmqpPrimitiveType_t::RECEIVER_SETTLE_MODE_TYPE, false, false),
            FieldType("source", "*", false, false, {AmqpRequiresProvides_t::SOURCE}),
            FieldType("target", "*", false, false, {AmqpRequiresProvides_t::TARGET}),
            FieldType("unsettled", AmqpPrimitiveType_t::MAP_TYPE, false, false),
            FieldType("incomplete-unsettled", AmqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType("initial-delivery-count", AmqpPrimitiveType_t::SEQUENCE_NUMBER_TYPE, false, false),
            FieldType("max-message-size", AmqpPrimitiveType_t::ULONG_TYPE, false, false),
            FieldType("offered-capabilities", AmqpPrimitiveType_t::SYMBOL_TYPE, false, true),
            FieldType("desired-capabilities", AmqpPrimitiveType_t::SYMBOL_TYPE, false, true),
            FieldType("properties", AmqpPrimitiveType_t::FIELDS_TYPE, false, false)
        };

        void AmqpAttach::validate() {
            Performative::validate();
        }


        //-- class AmqpFlow ---

        AmqpFlow::AmqpFlow(uint64_t packetNum, std::size_t dataOffset, AmqpList* fieldListPtr):
            Performative(packetNum, dataOffset, fieldListPtr)
        {}

        AmqpFlow::~AmqpFlow() {}

        // static
        const FieldTypeList_t AmqpFlow::s_fieldTypeList = {
            FieldType("next-incoming-id", AmqpPrimitiveType_t::TRANSFER_NUMBER_TYPE, false, false),
            FieldType("incoming-window", AmqpPrimitiveType_t::UINT_TYPE, true, false),
            FieldType("next-outgoing-id", AmqpPrimitiveType_t::TRANSFER_NUMBER_TYPE, true, false),
            FieldType("outgoing-window", AmqpPrimitiveType_t::UINT_TYPE, true, false),
            FieldType("handle", AmqpPrimitiveType_t::HANDLE_TYPE, false, false),
            FieldType("delivery-count", AmqpPrimitiveType_t::SEQUENCE_NUMBER_TYPE, false, false),
            FieldType("link-credit", AmqpPrimitiveType_t::UINT_TYPE, false, false),
            FieldType("available", AmqpPrimitiveType_t::UINT_TYPE, false, false),
            FieldType("drain", AmqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType("echo", AmqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType("properties", AmqpPrimitiveType_t::FIELDS_TYPE, false, false)
        };

        void AmqpFlow::validate() {
            Performative::validate();
        }


        //-- class AmqpTransfer ---

        AmqpTransfer::AmqpTransfer(uint64_t packetNum, std::size_t dataOffset, AmqpList* fieldListPtr):
            Performative(packetNum, dataOffset, fieldListPtr)
        {}

        AmqpTransfer::~AmqpTransfer() {}

        // static
        const FieldTypeList_t AmqpTransfer::s_fieldTypeList = {
            FieldType("handle", AmqpPrimitiveType_t::HANDLE_TYPE, true, false),
            FieldType("delivery-id", AmqpPrimitiveType_t::DELIVERY_NUMBER_TYPE, false, false),
            FieldType("delivery-tag", AmqpPrimitiveType_t::DELIVERY_TAG_TYPE, false, false),
            FieldType("message-format", AmqpPrimitiveType_t::MESSAGE_FORMAT_TYPE, false, false),
            FieldType("settled", AmqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType("more", AmqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType("rcv-settle-mode", AmqpPrimitiveType_t::RECEIVER_SETTLE_MODE_TYPE, false, false),
            FieldType("state", "*", false, false, {AmqpRequiresProvides_t::DELIVERY_STATE}),
            FieldType("resume", AmqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType("aborted", AmqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType("batchable", AmqpPrimitiveType_t::BOOLEAN_TYPE, false, false)
        };

        void AmqpTransfer::validate() {
            Performative::validate();
        }


        //-- class AmqpDisposition ---

        AmqpDisposition::AmqpDisposition(uint64_t packetNum, std::size_t dataOffset, AmqpList* fieldListPtr):
            Performative(packetNum, dataOffset, fieldListPtr)
        {}

        AmqpDisposition::~AmqpDisposition() {}

        // static
        const FieldTypeList_t AmqpDisposition::s_fieldTypeList = {
            FieldType("role", AmqpPrimitiveType_t::ROLE_TYPE, true, false),
            FieldType("first", AmqpPrimitiveType_t::DELIVERY_NUMBER_TYPE, true, false),
            FieldType("last", AmqpPrimitiveType_t::DELIVERY_NUMBER_TYPE, false, false),
            FieldType("settled", AmqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType("state", "*", false, false, {AmqpRequiresProvides_t::DELIVERY_STATE}),
            FieldType("batchable", AmqpPrimitiveType_t::BOOLEAN_TYPE, false, false)
        };

        void AmqpDisposition::validate() {
            Performative::validate();
        }


        //-- class AmqpDetach ---

        AmqpDetach::AmqpDetach(uint64_t packetNum, std::size_t dataOffset, AmqpList* fieldListPtr):
            Performative(packetNum, dataOffset, fieldListPtr)
        {}

        AmqpDetach::~AmqpDetach() {}

        // static
        const FieldTypeList_t AmqpDetach::s_fieldTypeList = {
            FieldType("handle", AmqpPrimitiveType_t::HANDLE_TYPE, true, false),
            FieldType("closed", AmqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType("error", "error", false, false)
        };

        void AmqpDetach::validate() {
            Performative::validate();
        }


        //-- class AmqpEnd ---

        AmqpEnd::AmqpEnd(uint64_t packetNum, std::size_t dataOffset, AmqpList* fieldListPtr):
            Performative(packetNum, dataOffset, fieldListPtr)
        {}

        AmqpEnd::~AmqpEnd() {}

        // static
        const FieldTypeList_t AmqpEnd::s_fieldTypeList = {
            FieldType("error", "error", false, false)
        };

        void AmqpEnd::validate() {
            Performative::validate();
        }


        //-- class AmqpClose ---

        AmqpClose::AmqpClose(uint64_t packetNum, std::size_t dataOffset, AmqpList* fieldListPtr):
            Performative(packetNum, dataOffset, fieldListPtr)
        {}

        AmqpClose::~AmqpClose() {}

        // static
        const FieldTypeList_t AmqpClose::s_fieldTypeList = {
            FieldType("error", "error", false, false)
        };

        void AmqpClose::validate() {
            Performative::validate();
        }



    } /* namespace amqp10 */
} /* namespace amqpAnalyze */
