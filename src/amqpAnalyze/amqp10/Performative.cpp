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
            FieldType(AMQPOPEN_FN_CONTAINER_ID, AmqpPrimitiveType_t::STRING_TYPE, true, false),
            FieldType(AMQPOPEN_FN_HOSTNAME, AmqpPrimitiveType_t::STRING_TYPE, false, false),
            FieldType(AMQPOPEN_FN_MAX_FRAME_SIZE, AmqpPrimitiveType_t::UINT_TYPE, false, false),
            FieldType(AMQPOPEN_FN_CHANNEL_MAX, AmqpPrimitiveType_t::USHORT_TYPE, false, false),
            FieldType(AMQPOPEN_FN_IDLE_TIME_OUT, AmqpPrimitiveType_t::MILLISECONDS_TYPE, false, false),
            FieldType(AMQPOPEN_FN_OUTGOING_LOCALES, AmqpPrimitiveType_t::LANGUAGE_TAG_TYPE, false, true),
            FieldType(AMQPOPEN_FN_INCOMING_LOCALES, AmqpPrimitiveType_t::LANGUAGE_TAG_TYPE, false, true),
            FieldType(AMQPOPEN_FN_OFFERED_CAPABILITIES, AmqpPrimitiveType_t::SYMBOL_TYPE, false, true),
            FieldType(AMQPOPEN_FN_DESIRED_CAPABILITIES, AmqpPrimitiveType_t::SYMBOL_TYPE, false, true),
            FieldType(AMQPOPEN_FN_PROPERTIES, AmqpPrimitiveType_t::FIELDS_TYPE, false, false)
        };

        void AmqpOpen::validate() {
            Performative::validate();
        }

        const std::string& AmqpOpen::containerId(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPOPEN_FN_CONTAINER_ID)) {
                return _fieldListPtr->getNamedType<AmqpString>(AMQPOPEN_FN_CONTAINER_ID)->value();
            }
            return AmqpString::s_emptyString;
        }

        const std::string& AmqpOpen::hostname(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPOPEN_FN_HOSTNAME)) {
                return _fieldListPtr->getNamedType<AmqpString>(AMQPOPEN_FN_HOSTNAME)->value();
            }
            return AmqpString::s_emptyString;
        }

        uint32_t AmqpOpen::maxFrameSize(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPOPEN_FN_MAX_FRAME_SIZE)) {
                return _fieldListPtr->getNamedType<AmqpUint>(AMQPOPEN_FN_MAX_FRAME_SIZE)->value();
            }
            return 0;
        }

        uint16_t AmqpOpen::channelMax(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPOPEN_FN_CHANNEL_MAX)) {
                return _fieldListPtr->getNamedType<AmqpUshort>(AMQPOPEN_FN_CHANNEL_MAX)->value();
            }
            return 0;
        }

        uint32_t AmqpOpen::idleTimeOut(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPOPEN_FN_IDLE_TIME_OUT)) {
                return _fieldListPtr->getNamedType<AmqpMilliseconds>(AMQPOPEN_FN_IDLE_TIME_OUT)->value();
            }
            return 0;
        }

        const AmqpArray_t& AmqpOpen::outgoingLocales(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPOPEN_FN_OUTGOING_LOCALES)) {
                return _fieldListPtr->getNamedType<AmqpArray>(AMQPOPEN_FN_OUTGOING_LOCALES)->value();
            }
            return AmqpArray::s_emptyArray;
        }

        const AmqpArray_t& AmqpOpen::incomingLocales(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPOPEN_FN_INCOMING_LOCALES)) {
                return _fieldListPtr->getNamedType<AmqpArray>(AMQPOPEN_FN_INCOMING_LOCALES)->value();
            }
            return AmqpArray::s_emptyArray;
        }

        const AmqpArray_t& AmqpOpen::offeredCapabilities(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPOPEN_FN_OFFERED_CAPABILITIES)) {
                return _fieldListPtr->getNamedType<AmqpArray>(AMQPOPEN_FN_OFFERED_CAPABILITIES)->value();
            }
            return AmqpArray::s_emptyArray;
        }

        const AmqpArray_t& AmqpOpen::desiredCapabilities(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPOPEN_FN_DESIRED_CAPABILITIES)) {
                return _fieldListPtr->getNamedType<AmqpArray>(AMQPOPEN_FN_DESIRED_CAPABILITIES)->value();
            }
            return AmqpArray::s_emptyArray;
        }

        const AmqpMap_t& AmqpOpen::properties(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPOPEN_FN_PROPERTIES)) {
                return _fieldListPtr->getNamedType<AmqpFields>(AMQPOPEN_FN_PROPERTIES)->value();
            }
            return AmqpMap::s_emptyMap;
        }


        //-- class AmqpBegin ---

        AmqpBegin::AmqpBegin(uint64_t packetNum, std::size_t dataOffset, AmqpList* fieldListPtr):
            Performative(packetNum, dataOffset, fieldListPtr)
        {}

        AmqpBegin::~AmqpBegin() {}

        // static
        const FieldTypeList_t AmqpBegin::s_fieldTypeList = {
            FieldType(AMQPBEGIN_FN_REMOTE_CHANNEL, AmqpPrimitiveType_t::USHORT_TYPE, false, false),
            FieldType(AMQPBEGIN_FN_NEXT_OUTGOING_ID, AmqpPrimitiveType_t::TRANSFER_NUMBER_TYPE, true, false),
            FieldType(AMQPBEGIN_FN_INCOMING_WINDOW, AmqpPrimitiveType_t::UINT_TYPE, true, false),
            FieldType(AMQPBEGIN_FN_OUTGOING_WINDOW, AmqpPrimitiveType_t::UINT_TYPE, true, false),
            FieldType(AMQPBEGIN_FN_HANDLE_MAX, AmqpPrimitiveType_t::HANDLE_TYPE, false, false),
            FieldType(AMQPBEGIN_FN_OFFERED_CAPABILITIES, AmqpPrimitiveType_t::SYMBOL_TYPE, false, true),
            FieldType(AMQPBEGIN_FN_DESIRED_CAPABILITIES, AmqpPrimitiveType_t::SYMBOL_TYPE, false, true),
            FieldType(AMQPBEGIN_FN_PROPERTIES, AmqpPrimitiveType_t::FIELDS_TYPE, false, false)
        };

        void AmqpBegin::validate() {
            Performative::validate();
        }

        uint16_t AmqpBegin::remoteChannel(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPBEGIN_FN_REMOTE_CHANNEL)) {
                return _fieldListPtr->getNamedType<AmqpUshort>(AMQPBEGIN_FN_REMOTE_CHANNEL)->value();
            }
            return 0;
        }

        uint32_t AmqpBegin::nextOutgoingId(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPBEGIN_FN_NEXT_OUTGOING_ID)) {
                return _fieldListPtr->getNamedType<AmqpTransferNum>(AMQPBEGIN_FN_NEXT_OUTGOING_ID)->value();
            }
            return 0;
        }

        uint32_t AmqpBegin::incomingWindow(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPBEGIN_FN_INCOMING_WINDOW)) {
                return _fieldListPtr->getNamedType<AmqpUint>(AMQPBEGIN_FN_INCOMING_WINDOW)->value();
            }
            return 0;
        }

        uint32_t AmqpBegin::outgoingWindow(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPBEGIN_FN_OUTGOING_WINDOW)) {
                return _fieldListPtr->getNamedType<AmqpUint>(AMQPBEGIN_FN_OUTGOING_WINDOW)->value();
            }
            return 0;
        }

        uint32_t AmqpBegin::handleMax(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPBEGIN_FN_HANDLE_MAX)) {
                return _fieldListPtr->getNamedType<AmqpHandle>(AMQPBEGIN_FN_HANDLE_MAX)->value();
            }
            return 0;
        }

        const AmqpArray_t& AmqpBegin::offeredCapabilities(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPBEGIN_FN_OFFERED_CAPABILITIES)) {
                return _fieldListPtr->getNamedType<AmqpArray>(AMQPBEGIN_FN_OFFERED_CAPABILITIES)->value();
            }
            return AmqpArray::s_emptyArray;
        }

        const AmqpArray_t& AmqpBegin::desiredCapabilities(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPBEGIN_FN_DESIRED_CAPABILITIES)) {
                return _fieldListPtr->getNamedType<AmqpArray>(AMQPBEGIN_FN_DESIRED_CAPABILITIES)->value();
            }
            return AmqpArray::s_emptyArray;
        }

        const AmqpMap_t& AmqpBegin::properties(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPBEGIN_FN_PROPERTIES)) {
                return _fieldListPtr->getNamedType<AmqpFields>(AMQPBEGIN_FN_PROPERTIES)->value();
            }
            return AmqpMap::s_emptyMap;
        }



        //-- class AmqpAttach ---

        AmqpAttach::AmqpAttach(uint64_t packetNum, std::size_t dataOffset, AmqpList* fieldListPtr):
            Performative(packetNum, dataOffset, fieldListPtr)
        {}

        AmqpAttach::~AmqpAttach() {}

        // static
        const FieldTypeList_t AmqpAttach::s_fieldTypeList = {
            FieldType(AMQPATTACH_FN_NAME, AmqpPrimitiveType_t::STRING_TYPE, true, false),
            FieldType(AMQPATTACH_FN_HANDLE, AmqpPrimitiveType_t::HANDLE_TYPE, true, false),
            FieldType(AMQPATTACH_FN_ROLE, AmqpPrimitiveType_t::ROLE_TYPE, true, false),
            FieldType(AMQPATTACH_FN_SND_SETTLE_MODE, AmqpPrimitiveType_t::SENDER_SETTLE_MODE_TYPE, false, false),
            FieldType(AMQPATTACH_FN_RCV_SETTLE_MODE, AmqpPrimitiveType_t::RECEIVER_SETTLE_MODE_TYPE, false, false),
            FieldType(AMQPATTACH_FN_SOURCE, "*", false, false, {AmqpRequiresProvides_t::SOURCE}),
            FieldType(AMQPATTACH_FN_TARGET, "*", false, false, {AmqpRequiresProvides_t::TARGET}),
            FieldType(AMQPATTACH_FN_UNSETTLED, AmqpPrimitiveType_t::MAP_TYPE, false, false),
            FieldType(AMQPATTACH_FN_INOMPLETE_UNSETTTLED, AmqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType(AMQPATTACH_FN_INITIAL_DELIVERY_COUNT, AmqpPrimitiveType_t::SEQUENCE_NUMBER_TYPE, false, false),
            FieldType(AMQPATTACH_FN_MAX_MESSAGE_SIZE, AmqpPrimitiveType_t::ULONG_TYPE, false, false),
            FieldType(AMQPATTACH_FN_OFFERED_CAPABILITES, AmqpPrimitiveType_t::SYMBOL_TYPE, false, true),
            FieldType(AMQPATTACH_FN_DESIRED_CAPABILITIES, AmqpPrimitiveType_t::SYMBOL_TYPE, false, true),
            FieldType(AMQPATTACH_FN_PROPERTIES, AmqpPrimitiveType_t::FIELDS_TYPE, false, false)
        };

        void AmqpAttach::validate() {
            Performative::validate();
        }
        const std::string& AmqpAttach::name(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPATTACH_FN_NAME)) {
                return _fieldListPtr->getNamedType<AmqpString>(AMQPATTACH_FN_NAME)->value();
            }
            return AmqpString::s_emptyString;
        }
        uint32_t AmqpAttach::handle(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPATTACH_FN_HANDLE)) {
                return _fieldListPtr->getNamedType<AmqpHandle>(AMQPATTACH_FN_HANDLE)->value();
            }
            return 0;
        }
        AmqpRole_t AmqpAttach::role(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPATTACH_FN_ROLE)) {
                return _fieldListPtr->getNamedType<AmqpRole>(AMQPATTACH_FN_ROLE)->value();
            }
            return AmqpRole_t::SENDER;
        }
        AmqpSenderSettleMode_t AmqpAttach::sndSettleMode(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPATTACH_FN_SND_SETTLE_MODE)) {
                return _fieldListPtr->getNamedType<AmqpSenderSettleMode>(AMQPATTACH_FN_SND_SETTLE_MODE)->value();
            }
            return AmqpSenderSettleMode_t::UNSETTLED;
        }
        AmqpReceiverSettleMode_t AmqpAttach::rcvSettleMode(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPATTACH_FN_RCV_SETTLE_MODE)) {
                return _fieldListPtr->getNamedType<AmqpReceiverSettleMode>(AMQPATTACH_FN_RCV_SETTLE_MODE)->value();
            }
            return AmqpReceiverSettleMode_t::FIRST;
        }
        const AmqpSource* AmqpAttach::source(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPATTACH_FN_SOURCE)) {
                return _fieldListPtr->getNamedType<AmqpSource>(AMQPATTACH_FN_SOURCE);
            }
            return nullptr;
        }
        const AmqpTarget* AmqpAttach::target(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPATTACH_FN_TARGET)) {
                return _fieldListPtr->getNamedType<AmqpTarget>(AMQPATTACH_FN_TARGET);
            }
            return nullptr;
        }
        const AmqpMap_t& AmqpAttach::unsettled(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPATTACH_FN_UNSETTLED)) {
                return _fieldListPtr->getNamedType<AmqpFields>(AMQPATTACH_FN_UNSETTLED)->value();
            }
            return AmqpMap::s_emptyMap;
        }
        bool AmqpAttach::incompleteUnsettled(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPATTACH_FN_INOMPLETE_UNSETTTLED)) {
                return _fieldListPtr->getNamedType<AmqpBoolean>(AMQPATTACH_FN_INOMPLETE_UNSETTTLED)->value();
            }
            return false;
        }
        uint32_t AmqpAttach::initialDeliveryCount(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPATTACH_FN_INITIAL_DELIVERY_COUNT)) {
                return _fieldListPtr->getNamedType<AmqpSequenceNum>(AMQPATTACH_FN_INITIAL_DELIVERY_COUNT)->value();
            }
            return 0;
        }
        uint64_t AmqpAttach::maxMessageSize(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPATTACH_FN_MAX_MESSAGE_SIZE)) {
                return _fieldListPtr->getNamedType<AmqpUlong>(AMQPATTACH_FN_MAX_MESSAGE_SIZE)->value();
            }
            return 0;
        }
        const AmqpArray_t& AmqpAttach::offeredCapabilities(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPATTACH_FN_OFFERED_CAPABILITES)) {
                return _fieldListPtr->getNamedType<AmqpArray>(AMQPATTACH_FN_OFFERED_CAPABILITES)->value();
            }
            return AmqpArray::s_emptyArray;
        }
        const AmqpArray_t& AmqpAttach::desiredCapabilities(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPATTACH_FN_DESIRED_CAPABILITIES)) {
                return _fieldListPtr->getNamedType<AmqpArray>(AMQPATTACH_FN_DESIRED_CAPABILITIES)->value();
            }
            return AmqpArray::s_emptyArray;
        }
        const AmqpMap_t& AmqpAttach::properties(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPATTACH_FN_PROPERTIES)) {
                return _fieldListPtr->getNamedType<AmqpFields>(AMQPATTACH_FN_PROPERTIES)->value();
            }
            return AmqpMap::s_emptyMap;
        }


        //-- class AmqpFlow ---

        AmqpFlow::AmqpFlow(uint64_t packetNum, std::size_t dataOffset, AmqpList* fieldListPtr):
            Performative(packetNum, dataOffset, fieldListPtr)
        {}

        AmqpFlow::~AmqpFlow() {}

        // static
        const FieldTypeList_t AmqpFlow::s_fieldTypeList = {
            FieldType(AMQPFLOW_FN_NEXT_INCOMING_ID, AmqpPrimitiveType_t::TRANSFER_NUMBER_TYPE, false, false),
            FieldType(AMQPFLOW_FN_INCOMING_WINDOW, AmqpPrimitiveType_t::UINT_TYPE, true, false),
            FieldType(AMQPFLOW_FN_NEXT_OUTGOING_WINDOW, AmqpPrimitiveType_t::TRANSFER_NUMBER_TYPE, true, false),
            FieldType(AMQPFLOW_FN_OUTGOING_WINDOW, AmqpPrimitiveType_t::UINT_TYPE, true, false),
            FieldType(AMQPFLOW_FN_HANDLE, AmqpPrimitiveType_t::HANDLE_TYPE, false, false),
            FieldType(AMQPFLOW_FN_DELIVERY_COUNT, AmqpPrimitiveType_t::SEQUENCE_NUMBER_TYPE, false, false),
            FieldType(AMQPFLOW_FN_LINK_CREDIT, AmqpPrimitiveType_t::UINT_TYPE, false, false),
            FieldType(AMQPFLOW_FN_AVAILABLE, AmqpPrimitiveType_t::UINT_TYPE, false, false),
            FieldType(AMQPFLOW_FN_DRAIN, AmqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType(AMQPFLOW_FN_ECHO, AmqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType(AMQPFLOW_FN_PROPERTIES, AmqpPrimitiveType_t::FIELDS_TYPE, false, false)
        };

        void AmqpFlow::validate() {
            Performative::validate();
        }

        uint32_t AmqpFlow::nextIncomingId(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPFLOW_FN_NEXT_INCOMING_ID)) {
                return _fieldListPtr->getNamedType<AmqpTransferNum>(AMQPFLOW_FN_NEXT_INCOMING_ID)->value();
            }
            return 0;
        }

        uint32_t AmqpFlow::incomingWindow(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPFLOW_FN_INCOMING_WINDOW)) {
                return _fieldListPtr->getNamedType<AmqpUint>(AMQPFLOW_FN_INCOMING_WINDOW)->value();
            }
            return 0;
        }

        uint32_t AmqpFlow::nextOutgoingId(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPFLOW_FN_NEXT_OUTGOING_WINDOW)) {
                return _fieldListPtr->getNamedType<AmqpTransferNum>(AMQPFLOW_FN_NEXT_OUTGOING_WINDOW)->value();
            }
            return 0;
        }

        uint32_t AmqpFlow::outgoingWindow(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPFLOW_FN_OUTGOING_WINDOW)) {
                return _fieldListPtr->getNamedType<AmqpUint>(AMQPFLOW_FN_OUTGOING_WINDOW)->value();
            }
            return 0;
        }

        uint32_t AmqpFlow::handle(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPFLOW_FN_HANDLE)) {
                return _fieldListPtr->getNamedType<AmqpHandle>(AMQPFLOW_FN_HANDLE)->value();
            }
            return 0;
        }

        uint32_t AmqpFlow::deliveryCount(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPFLOW_FN_DELIVERY_COUNT)) {
                return _fieldListPtr->getNamedType<AmqpSequenceNum>(AMQPFLOW_FN_DELIVERY_COUNT)->value();
            }
            return 0;
        }

        uint32_t AmqpFlow::linkCredit(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPFLOW_FN_LINK_CREDIT)) {
                return _fieldListPtr->getNamedType<AmqpUint>(AMQPFLOW_FN_LINK_CREDIT)->value();
            }
            return 0;
        }

        uint32_t AmqpFlow::available(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPFLOW_FN_AVAILABLE)) {
                return _fieldListPtr->getNamedType<AmqpUint>(AMQPFLOW_FN_AVAILABLE)->value();
            }
            return 0;
        }

        bool AmqpFlow::drain(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPFLOW_FN_DRAIN)) {
                return _fieldListPtr->getNamedType<AmqpBoolean>(AMQPFLOW_FN_DRAIN)->value();
            }
            return false;
        }

        bool AmqpFlow::echo(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPFLOW_FN_ECHO)) {
                return _fieldListPtr->getNamedType<AmqpBoolean>(AMQPFLOW_FN_ECHO)->value();
            }
            return false;
        }

        const AmqpMap_t& AmqpFlow::properties(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPFLOW_FN_PROPERTIES)) {
                return _fieldListPtr->getNamedType<AmqpFields>(AMQPFLOW_FN_PROPERTIES)->value();
            }
            return AmqpMap::s_emptyMap;
        }


        //-- class AmqpTransfer ---

        AmqpTransfer::AmqpTransfer(uint64_t packetNum, std::size_t dataOffset, AmqpList* fieldListPtr):
            Performative(packetNum, dataOffset, fieldListPtr)
        {}

        AmqpTransfer::~AmqpTransfer() {}

        // static
        const FieldTypeList_t AmqpTransfer::s_fieldTypeList = {
            FieldType(AMQPTRANSFER_FN_HANDLE, AmqpPrimitiveType_t::HANDLE_TYPE, true, false),
            FieldType(AMQPTRANSFER_FN_DELIVERY_ID, AmqpPrimitiveType_t::DELIVERY_NUMBER_TYPE, false, false),
            FieldType(AMQPTRANSFER_FN_DELIVERY_TAG, AmqpPrimitiveType_t::DELIVERY_TAG_TYPE, false, false),
            FieldType(AMQPTRANSFER_FN_MESSAGE_FORMAT, AmqpPrimitiveType_t::MESSAGE_FORMAT_TYPE, false, false),
            FieldType(AMQPTRANSFER_FN_SETTLED, AmqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType(AMQPTRANSFER_FN_MORE, AmqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType(AMQPTRANSFER_FN_RCV_SETTLE_MODE, AmqpPrimitiveType_t::RECEIVER_SETTLE_MODE_TYPE, false, false),
            FieldType(AMQPTRANSFER_FN_STATE, "*", false, false, {AmqpRequiresProvides_t::DELIVERY_STATE}),
            FieldType(AMQPTRANSFER_FN_RESUME, AmqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType(AMQPTRANSFER_FN_ABORTED, AmqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType(AMQPTRANSFER_FN_BATCHABLE, AmqpPrimitiveType_t::BOOLEAN_TYPE, false, false)
        };

        void AmqpTransfer::validate() {
            Performative::validate();
        }

        uint32_t AmqpTransfer::handle(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPTRANSFER_FN_HANDLE)) {
                return _fieldListPtr->getNamedType<AmqpHandle>(AMQPTRANSFER_FN_HANDLE)->value();
            }
            return 0;
        }

        uint32_t AmqpTransfer::deliveryId(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPTRANSFER_FN_DELIVERY_ID)) {
                return _fieldListPtr->getNamedType<AmqpDeliveryNum>(AMQPTRANSFER_FN_DELIVERY_ID)->value();
            }
            return 0;
        }

        const AmqpBinary_t& AmqpTransfer::deliveryTag(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPTRANSFER_FN_DELIVERY_TAG)) {
                return _fieldListPtr->getNamedType<AmqpDeliveryTag>(AMQPTRANSFER_FN_DELIVERY_TAG)->value();
            }
            return AmqpBinary::s_emptyBinary;
        }

        uint32_t AmqpTransfer::messageFormat(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPTRANSFER_FN_MESSAGE_FORMAT)) {
                return _fieldListPtr->getNamedType<AmqpMessageFormat>(AMQPTRANSFER_FN_MESSAGE_FORMAT)->value();
            }
            return 0;
        }

        const AmqpMessageFormat* AmqpTransfer::messageFormatObj(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPTRANSFER_FN_MESSAGE_FORMAT)) {
                return _fieldListPtr->getNamedType<AmqpMessageFormat>(AMQPTRANSFER_FN_MESSAGE_FORMAT);
            }
            return nullptr;
        }

        bool AmqpTransfer::settled(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPTRANSFER_FN_SETTLED)) {
                return _fieldListPtr->getNamedType<AmqpBoolean>(AMQPTRANSFER_FN_SETTLED)->value();
            }
            return false;
        }

        bool AmqpTransfer::more(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPTRANSFER_FN_MORE)) {
                return _fieldListPtr->getNamedType<AmqpBoolean>(AMQPTRANSFER_FN_MORE)->value();
            }
            return false;
        }

        AmqpReceiverSettleMode_t AmqpTransfer::rcvSettleMode(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPTRANSFER_FN_RCV_SETTLE_MODE)) {
                return _fieldListPtr->getNamedType<AmqpReceiverSettleMode>(AMQPTRANSFER_FN_RCV_SETTLE_MODE)->value();
            }
            return AmqpReceiverSettleMode_t::FIRST;
        }

        const CompositeType* AmqpTransfer::state(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPTRANSFER_FN_STATE)) {
                return _fieldListPtr->getNamedType<CompositeType>(AMQPTRANSFER_FN_STATE);
            }
            return nullptr;
        }

        bool AmqpTransfer::resume(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPTRANSFER_FN_RESUME)) {
                return _fieldListPtr->getNamedType<AmqpBoolean>(AMQPTRANSFER_FN_RESUME)->value();
            }
            return false;
        }

        bool AmqpTransfer::aborted(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPTRANSFER_FN_ABORTED)) {
                return _fieldListPtr->getNamedType<AmqpBoolean>(AMQPTRANSFER_FN_ABORTED)->value();
            }
            return false;
        }

        bool AmqpTransfer::batchable(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPTRANSFER_FN_BATCHABLE)) {
                return _fieldListPtr->getNamedType<AmqpBoolean>(AMQPTRANSFER_FN_BATCHABLE)->value();
            }
            return false;
        }


        //-- class AmqpDisposition ---

        AmqpDisposition::AmqpDisposition(uint64_t packetNum, std::size_t dataOffset, AmqpList* fieldListPtr):
            Performative(packetNum, dataOffset, fieldListPtr)
        {}

        AmqpDisposition::~AmqpDisposition() {}

        // static
        const FieldTypeList_t AmqpDisposition::s_fieldTypeList = {
            FieldType(AMQPDISPOSITION_FN_ROLE, AmqpPrimitiveType_t::ROLE_TYPE, true, false),
            FieldType(AMQPDISPOSITION_FN_FIRST, AmqpPrimitiveType_t::DELIVERY_NUMBER_TYPE, true, false),
            FieldType(AMQPDISPOSITION_FN_LAST, AmqpPrimitiveType_t::DELIVERY_NUMBER_TYPE, false, false),
            FieldType(AMQPDISPOSITION_FN_SETTLED, AmqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType(AMQPDISPOSITION_FN_STATE, "*", false, false, {AmqpRequiresProvides_t::DELIVERY_STATE}),
            FieldType(AMQPDISPOSITION_FN_BATCHABLE, AmqpPrimitiveType_t::BOOLEAN_TYPE, false, false)
        };

        void AmqpDisposition::validate() {
            Performative::validate();
        }

        AmqpRole_t AmqpDisposition::role(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPDISPOSITION_FN_ROLE)) {
                return _fieldListPtr->getNamedType<AmqpRole>(AMQPDISPOSITION_FN_ROLE)->value();
            }
            return AmqpRole_t::SENDER;
        }

        uint32_t AmqpDisposition::first(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPDISPOSITION_FN_FIRST)) {
                return _fieldListPtr->getNamedType<AmqpDeliveryNum>(AMQPDISPOSITION_FN_FIRST)->value();
            }
            return 0;
        }

        uint32_t AmqpDisposition::last(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPDISPOSITION_FN_LAST)) {
                return _fieldListPtr->getNamedType<AmqpDeliveryNum>(AMQPDISPOSITION_FN_LAST)->value();
            }
            return 0;
        }

        bool AmqpDisposition::settled(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPDISPOSITION_FN_SETTLED)) {
                return _fieldListPtr->getNamedType<AmqpBoolean>(AMQPDISPOSITION_FN_SETTLED)->value();
            }
            return false;
        }

        const CompositeType* AmqpDisposition::state(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPDISPOSITION_FN_STATE)) {
                return _fieldListPtr->getNamedType<CompositeType>(AMQPDISPOSITION_FN_STATE);
            }
            return nullptr;
        }

        bool AmqpDisposition::batchable(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPDISPOSITION_FN_BATCHABLE)) {
                return _fieldListPtr->getNamedType<AmqpBoolean>(AMQPDISPOSITION_FN_BATCHABLE)->value();
            }
            return false;
        }


        //-- class AmqpDetach ---

        AmqpDetach::AmqpDetach(uint64_t packetNum, std::size_t dataOffset, AmqpList* fieldListPtr):
            Performative(packetNum, dataOffset, fieldListPtr)
        {}

        AmqpDetach::~AmqpDetach() {}

        // static
        const FieldTypeList_t AmqpDetach::s_fieldTypeList = {
            FieldType(AMQPDETACH_FN_HANDLE, AmqpPrimitiveType_t::HANDLE_TYPE, true, false),
            FieldType(AMQPDETACH_FN_CLOSED, AmqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType(AMQPDETACH_FN_ERROR, "error", false, false)
        };

        void AmqpDetach::validate() {
            Performative::validate();
        }

        uint32_t AmqpDetach::handle(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPDETACH_FN_HANDLE)) {
                return _fieldListPtr->getNamedType<AmqpHandle>(AMQPDETACH_FN_HANDLE)->value();
            }
            return 0;
        }

        bool AmqpDetach::cloased(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPDETACH_FN_CLOSED)) {
                return _fieldListPtr->getNamedType<AmqpBoolean>(AMQPDETACH_FN_CLOSED)->value();
            }
            return false;
        }

        const AmqpErrorRecord* AmqpDetach::error(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPDETACH_FN_ERROR)) {
                return _fieldListPtr->getNamedType<AmqpErrorRecord>(AMQPDETACH_FN_ERROR);
            }
            return nullptr;
        }


        //-- class AmqpEnd ---

        AmqpEnd::AmqpEnd(uint64_t packetNum, std::size_t dataOffset, AmqpList* fieldListPtr):
            Performative(packetNum, dataOffset, fieldListPtr)
        {}

        AmqpEnd::~AmqpEnd() {}

        // static
        const FieldTypeList_t AmqpEnd::s_fieldTypeList = {
            FieldType(AMQPEND_FN_ERROR, "error", false, false)
        };

        void AmqpEnd::validate() {
            Performative::validate();
        }

        const AmqpErrorRecord* AmqpEnd::error(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPEND_FN_ERROR)) {
                return _fieldListPtr->getNamedType<AmqpErrorRecord>(AMQPEND_FN_ERROR);
            }
            return nullptr;
        }


        //-- class AmqpClose ---

        AmqpClose::AmqpClose(uint64_t packetNum, std::size_t dataOffset, AmqpList* fieldListPtr):
            Performative(packetNum, dataOffset, fieldListPtr)
        {}

        AmqpClose::~AmqpClose() {}

        // static
        const FieldTypeList_t AmqpClose::s_fieldTypeList = {
            FieldType(AMQPCLOSE_FN_ERROR, "error", false, false)
        };

        void AmqpClose::validate() {
            Performative::validate();
        }

        const AmqpErrorRecord* AmqpClose::error(bool throwFlag) const {
            if (throwFlag || _fieldListPtr->hasNamedEntryNotNull(AMQPCLOSE_FN_ERROR)) {
                return _fieldListPtr->getNamedType<AmqpErrorRecord>(AMQPCLOSE_FN_ERROR);
            }
            return nullptr;
        }



    } /* namespace amqp10 */
} /* namespace amqpAnalyze */
