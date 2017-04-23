/*
 * FrameHeader.cpp
 *
 *  Created on: Apr 3, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/amqp10/Performative.hpp>

#include <amqpAnalyze/amqp10/FieldType.hpp>
#include <amqpAnalyze/amqp10/FrameBuffer.hpp>
#include <amqpAnalyze/amqp10/Section.hpp>
#include <amqpAnalyze/amqp10/Type.hpp>
#include <amqpAnalyze/Error.hpp>
#include <iomanip>
#include <netinet/in.h>
#include <std/AnsiTermColors.hpp>
#include <string.h>

namespace amqpAnalyze
{
    namespace amqp10
    {

        Performative::Performative(uint64_t packetNum, std::size_t dataOffset, AmqpList* fieldListPtr):
                AmqpBlock(packetNum, dataOffset),
                _fieldListPtr(fieldListPtr)
        {}

        Performative::~Performative() {
            for (amqp_list_itr_t i=_fieldListPtr->value().begin(); i!=_fieldListPtr->value().end(); ++i) {
                delete *i;
            }
            _fieldListPtr->value().clear();
            delete _fieldListPtr;
        }

        std::ostringstream& Performative::appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin, bool colorFlag) const {
            if (!ignoreFirstMargin) oss << "\n" << std::string(margin, ' ') << "[" << std::hex << std::setfill('0') << std::setw(4) << _dataOffset << "] ";
            oss  << "p " << COLOR(FGND_BYLW, typeStr(), colorFlag);
            if (_fieldListPtr != nullptr) {
                _fieldListPtr->appendString(oss, margin + 9 + ::strlen(typeStr()), true, colorFlag);
            }
            return appendStringEpilog(oss, margin + 9, colorFlag);
        }

        void Performative::validate() {
            // Validate field list
            if (_fieldListPtr != nullptr) {
                _fieldListPtr->validate(fieldTypeList(), &AmqpBlock::addError, this);
            }
        }

        // static
        std::map<performativeType_t, const char*> Performative::s_performativeTypeName = {
            {performativeType_t::OPEN, "open"},
            {performativeType_t::BEGIN, "begin"},
            {performativeType_t::ATTACH, "attach"},
            {performativeType_t::FLOW, "flow"},
            {performativeType_t::TRANSFER, "transfer"},
            {performativeType_t::DISPOSITION, "disposition"},
            {performativeType_t::DETACH, "detach"},
            {performativeType_t::END, "end"},
            {performativeType_t::CLOSE, "close"}
        };


        //-- class AmqpOpen ---

        AmqpOpen::AmqpOpen(uint64_t packetNum, std::size_t dataOffset, AmqpList* fieldListPtr):
            Performative(packetNum, dataOffset, fieldListPtr)
        {}

        AmqpOpen::~AmqpOpen() {}

        // static
        const fieldTypeList_t AmqpOpen::s_fieldTypeList = {
            FieldType("container-id", amqpPrimitiveType_t::STRING_TYPE, true, false),
            FieldType("hostname", amqpPrimitiveType_t::STRING_TYPE, false, false),
            FieldType("max-frame-size", amqpPrimitiveType_t::UINT_TYPE, false, false),
            FieldType("channel-max", amqpPrimitiveType_t::USHORT_TYPE, false, false),
            FieldType("idle-time-out", amqpPrimitiveType_t::MILLISECONDS_TYPE, false, false),
            FieldType("outgoing-locales", amqpPrimitiveType_t::LANGUAGE_TAG_TYPE, false, true),
            FieldType("incoming-locales", amqpPrimitiveType_t::LANGUAGE_TAG_TYPE, false, true),
            FieldType("offered-capabilities", amqpPrimitiveType_t::SYMBOL_TYPE, false, true),
            FieldType("desired-capabilities", amqpPrimitiveType_t::SYMBOL_TYPE, false, true),
            FieldType("properties", amqpPrimitiveType_t::FIELDS_TYPE, false, false)
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
        const fieldTypeList_t AmqpBegin::s_fieldTypeList = {
            FieldType("remote-channel", amqpPrimitiveType_t::USHORT_TYPE, false, false),
            FieldType("next-outgoing-id", amqpPrimitiveType_t::TRANSFER_NUMBER_TYPE, true, false),
            FieldType("incoming-window", amqpPrimitiveType_t::UINT_TYPE, true, false),
            FieldType("outgoing-window", amqpPrimitiveType_t::UINT_TYPE, true, false),
            FieldType("handle-max", amqpPrimitiveType_t::HANDLE_TYPE, false, false),
            FieldType("offered-capabilities", amqpPrimitiveType_t::SYMBOL_TYPE, false, true),
            FieldType("desired-capabilities", amqpPrimitiveType_t::SYMBOL_TYPE, false, true),
            FieldType("properties", amqpPrimitiveType_t::FIELDS_TYPE, false, false)
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
        const fieldTypeList_t AmqpAttach::s_fieldTypeList = {
            FieldType("name", amqpPrimitiveType_t::STRING_TYPE, true, false),
            FieldType("handle", amqpPrimitiveType_t::HANDLE_TYPE, true, false),
            FieldType("role", amqpPrimitiveType_t::ROLE_TYPE, true, false),
            FieldType("snd-settle-mode", amqpPrimitiveType_t::SENDER_SETTLE_MODE_TYPE, false, false),
            FieldType("rcv-settle-mode", amqpPrimitiveType_t::RECEIVER_SETTLE_MODE_TYPE, false, false),
            FieldType("source", "*", false, false, {amqpRequiresProvides_t::SOURCE}),
            FieldType("target", "*", false, false, {amqpRequiresProvides_t::TARGET}),
            FieldType("unsettled", amqpPrimitiveType_t::MAP_TYPE, false, false),
            FieldType("incomplete-unsettled", amqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType("initial-delivery-count", amqpPrimitiveType_t::SEQUENCE_NUMBER_TYPE, false, false),
            FieldType("max-message-size", amqpPrimitiveType_t::ULONG_TYPE, false, false),
            FieldType("offered-capabilities", amqpPrimitiveType_t::SYMBOL_TYPE, false, true),
            FieldType("desired-capabilities", amqpPrimitiveType_t::SYMBOL_TYPE, false, true),
            FieldType("properties", amqpPrimitiveType_t::FIELDS_TYPE, false, false)
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
        const fieldTypeList_t AmqpFlow::s_fieldTypeList = {
            FieldType("next-incoming-id", amqpPrimitiveType_t::TRANSFER_NUMBER_TYPE, false, false),
            FieldType("incoming-window", amqpPrimitiveType_t::UINT_TYPE, true, false),
            FieldType("next-outgoing-id", amqpPrimitiveType_t::TRANSFER_NUMBER_TYPE, true, false),
            FieldType("outgoing-window", amqpPrimitiveType_t::UINT_TYPE, true, false),
            FieldType("handle", amqpPrimitiveType_t::HANDLE_TYPE, false, false),
            FieldType("delivery-count", amqpPrimitiveType_t::SEQUENCE_NUMBER_TYPE, false, false),
            FieldType("link-credit", amqpPrimitiveType_t::UINT_TYPE, false, false),
            FieldType("available", amqpPrimitiveType_t::UINT_TYPE, false, false),
            FieldType("drain", amqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType("echo", amqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType("properties", amqpPrimitiveType_t::FIELDS_TYPE, false, false)
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
        const fieldTypeList_t AmqpTransfer::s_fieldTypeList = {
            FieldType("handle", amqpPrimitiveType_t::HANDLE_TYPE, true, false),
            FieldType("delivery-id", amqpPrimitiveType_t::DELIVERY_NUMBER_TYPE, false, false),
            FieldType("delivery-tag", amqpPrimitiveType_t::DELIVERY_TAG_TYPE, false, false),
            FieldType("message-format", amqpPrimitiveType_t::MESSAGE_FORMAT_TYPE, false, false),
            FieldType("settled", amqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType("more", amqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType("rcv-settle-mode", amqpPrimitiveType_t::RECEIVER_SETTLE_MODE_TYPE, false, false),
            FieldType("state", "*", false, false, {amqpRequiresProvides_t::DELIVERY_STATE}),
            FieldType("resume", amqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType("aborted", amqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType("batchable", amqpPrimitiveType_t::BOOLEAN_TYPE, false, false)
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
        const fieldTypeList_t AmqpDisposition::s_fieldTypeList = {
            FieldType("role", amqpPrimitiveType_t::ROLE_TYPE, true, false),
            FieldType("first", amqpPrimitiveType_t::DELIVERY_NUMBER_TYPE, true, false),
            FieldType("last", amqpPrimitiveType_t::DELIVERY_NUMBER_TYPE, false, false),
            FieldType("settled", amqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType("state", "*", false, false, {amqpRequiresProvides_t::DELIVERY_STATE}),
            FieldType("batchable", amqpPrimitiveType_t::BOOLEAN_TYPE, false, false)
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
        const fieldTypeList_t AmqpDetach::s_fieldTypeList = {
            FieldType("handle", amqpPrimitiveType_t::HANDLE_TYPE, true, false),
            FieldType("closed", amqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
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
        const fieldTypeList_t AmqpEnd::s_fieldTypeList = {
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
        const fieldTypeList_t AmqpClose::s_fieldTypeList = {
            FieldType("error", "error", false, false)
        };

        void AmqpClose::validate() {
            Performative::validate();
        }



    } /* namespace amqp10 */
} /* namespace amqpAnalyze */
