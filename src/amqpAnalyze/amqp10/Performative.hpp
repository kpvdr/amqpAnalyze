/*
 * Performative.hpp
 *
 *  Created on: Apr 3, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_AMQP10_PERFORMATIVE_HPP_
#define SRC_AMQPANALYZE_AMQP10_PERFORMATIVE_HPP_

#include <amqpAnalyze/amqp10/AmqpBlock.hpp>

namespace amqpAnalyze
{
    namespace amqp10
    {
        enum class AmqpRole_t:bool;
        enum class AmqpSenderSettleMode_t:uint8_t;
        enum class AmqpReceiverSettleMode_t:uint8_t;
        class AmqpErrorRecord;
        class AmqpList;
        class AmqpMap;
        class AmqpMessageFormat;
        class AmqpSource;
        class AmqpTarget;
        class CompositeType;

        enum class PerformativeType_t:uint64_t {
            OPEN=0x10,
            BEGIN,
            ATTACH,
            FLOW,
            TRANSFER,
            DISPOSITION,
            DETACH,
            END,
            CLOSE
        };

        class Performative: public AmqpBlock {
        public:
            Performative(uint64_t packetNum, std::size_t dataOffset, AmqpList* fieldListPtr);
            virtual ~Performative();

            std::ostringstream& appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const override;
            inline AmqpBlockType_t blockType() const override { return AmqpBlockType_t::PERFORMATIVE; }
            inline const AmqpList* fieldList() const { return _fieldListPtr; }
            virtual const FieldTypeList_t& fieldTypeList() const = 0;
            virtual PerformativeType_t type() const = 0;
            inline const char* typeStr() const { return s_performativeTypeName.at(type()); }
            void validate() override;

        protected:
            AmqpList* _fieldListPtr;

            static std::map<PerformativeType_t, const char*> s_performativeTypeName;
        };



#define AMQPOPEN_FN_CONTAINER_ID "container-id"
#define AMQPOPEN_FN_HOSTNAME "hostname"
#define AMQPOPEN_FN_MAX_FRAME_SIZE "max-frame-size"
#define AMQPOPEN_FN_CHANNEL_MAX "channel-max"
#define AMQPOPEN_FN_IDLE_TIME_OUT "idle-time-out"
#define AMQPOPEN_FN_OUTGOING_LOCALES "outgoing-locales"
#define AMQPOPEN_FN_INCOMING_LOCALES "incoming-locales"
#define AMQPOPEN_FN_OFFERED_CAPABILITIES "offered-capabilities"
#define AMQPOPEN_FN_DESIRED_CAPABILITIES "desired-capabilities"
#define AMQPOPEN_FN_PROPERTIES "properties"

        class AmqpOpen: public Performative {
        public:
            AmqpOpen(uint64_t packetNum, std::size_t dataOffset, AmqpList* fieldListPtr);
            virtual ~AmqpOpen();

            inline const FieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; };
            inline PerformativeType_t type() const override { return PerformativeType_t::OPEN; }
            void validate() override;

            // convenience getter methods
            const std::string& containerId(bool throwFlag) const; // required
            const std::string& hostname(bool throwFlag) const;
            uint32_t maxFrameSize(bool throwFlag) const;
            uint16_t channelMax(bool throwFlag) const;
            uint32_t idleTimeOut(bool throwFlag) const;
            const AmqpArray_t& outgoingLocales(bool throwFlag) const;
            const AmqpArray_t& incomingLocales(bool throwFlag) const;
            const AmqpArray_t& offeredCapabilities(bool throwFlag) const;
            const AmqpArray_t& desiredCapabilities(bool throwFlag) const;
            const AmqpMap_t& properties(bool throwFlag) const;

            static const FieldTypeList_t s_fieldTypeList;
        };



#define AMQPBEGIN_FN_REMOTE_CHANNEL "remote-channel"
#define AMQPBEGIN_FN_NEXT_OUTGOING_ID "next-outgoing-id"
#define AMQPBEGIN_FN_INCOMING_WINDOW "incoming-window"
#define AMQPBEGIN_FN_OUTGOING_WINDOW "outgoing-window"
#define AMQPBEGIN_FN_HANDLE_MAX "handle-max"
#define AMQPBEGIN_FN_OFFERED_CAPABILITIES "offered-capabilities"
#define AMQPBEGIN_FN_DESIRED_CAPABILITIES "desired-capabilities"
#define AMQPBEGIN_FN_PROPERTIES "properties"

        class AmqpBegin: public Performative {
        public:
            AmqpBegin(uint64_t packetNum, std::size_t dataOffset, AmqpList* fieldListPtr);
            virtual ~AmqpBegin();

            inline const FieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; };
            inline PerformativeType_t type() const override { return PerformativeType_t::BEGIN; }
            void validate() override;

            // convenience getter methods
            uint16_t remoteChannel(bool throwFlag) const;
            uint32_t nextOutgoingId(bool throwFlag) const; // req'd
            uint32_t incomingWindow(bool throwFlag) const; // req'd
            uint32_t outgoingWindow(bool throwFlag) const; // req'd
            uint32_t handleMax(bool throwFlag) const;
            const AmqpArray_t& offeredCapabilities(bool throwFlag) const;
            const AmqpArray_t& desiredCapabilities(bool throwFlag) const;
            const AmqpMap_t& properties(bool throwFlag) const;

            static const FieldTypeList_t s_fieldTypeList;
        };



#define AMQPATTACH_FN_NAME "name"
#define AMQPATTACH_FN_HANDLE "handle"
#define AMQPATTACH_FN_ROLE "role"
#define AMQPATTACH_FN_SND_SETTLE_MODE "snd-settle-mode"
#define AMQPATTACH_FN_RCV_SETTLE_MODE "rcv-settle-mode"
#define AMQPATTACH_FN_SOURCE "source"
#define AMQPATTACH_FN_TARGET "target"
#define AMQPATTACH_FN_UNSETTLED "unsettled"
#define AMQPATTACH_FN_INOMPLETE_UNSETTTLED "incomplete-unsettled"
#define AMQPATTACH_FN_INITIAL_DELIVERY_COUNT "initial-delivery-count"
#define AMQPATTACH_FN_MAX_MESSAGE_SIZE "max-message-size"
#define AMQPATTACH_FN_OFFERED_CAPABILITES "offered-capabilities"
#define AMQPATTACH_FN_DESIRED_CAPABILITIES "desired-capabilities"
#define AMQPATTACH_FN_PROPERTIES "properties"
        class AmqpAttach: public Performative {
        public:
            AmqpAttach(uint64_t packetNum, std::size_t dataOffset, AmqpList* fieldListPtr);
            virtual ~AmqpAttach();

            inline const FieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; };
            inline PerformativeType_t type() const override { return PerformativeType_t::ATTACH; }
            void validate() override;

            // convenience getter methods
            const std::string& name(bool throwFlag) const; // TODO: shadows AmqpBlock::name()
            uint32_t handle(bool throwFlag) const;
            AmqpRole_t role(bool throwFlag) const;
            AmqpSenderSettleMode_t sndSettleMode(bool throwFlag) const;
            AmqpReceiverSettleMode_t rcvSettleMode(bool throwFlag) const;
            const AmqpSource* source(bool throwFlag) const;
            const AmqpTarget* target(bool throwFlag) const;
            const AmqpMap_t& unsettled(bool throwFlag) const;
            bool incompleteUnsettled(bool throwFlag) const;
            uint32_t initialDeliveryCount(bool throwFlag) const;
            uint64_t maxMessageSize(bool throwFlag) const;
            const AmqpArray_t& offeredCapabilities(bool throwFlag) const;
            const AmqpArray_t& desiredCapabilities(bool throwFlag) const;
            const AmqpMap_t& properties(bool throwFlag) const;

            static const FieldTypeList_t s_fieldTypeList;
        };



#define AMQPFLOW_FN_NEXT_INCOMING_ID "next-incoming-id"
#define AMQPFLOW_FN_INCOMING_WINDOW "incoming-window"
#define AMQPFLOW_FN_NEXT_OUTGOING_WINDOW "next-outgoing-id"
#define AMQPFLOW_FN_OUTGOING_WINDOW "outgoing-window"
#define AMQPFLOW_FN_HANDLE "handle"
#define AMQPFLOW_FN_DELIVERY_COUNT "delivery-count"
#define AMQPFLOW_FN_LINK_CREDIT "link-credit"
#define AMQPFLOW_FN_AVAILABLE "available"
#define AMQPFLOW_FN_DRAIN "drain"
#define AMQPFLOW_FN_ECHO "echo"
#define AMQPFLOW_FN_PROPERTIES "properties"
        class AmqpFlow: public Performative {
        public:
            AmqpFlow(uint64_t packetNum, std::size_t dataOffset, AmqpList* fieldListPtr);
            virtual ~AmqpFlow();

            inline const FieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; };
            inline PerformativeType_t type() const override { return PerformativeType_t::FLOW; }
            void validate() override;

            // convenience getter methods
            uint32_t nextIncomingId(bool throwFlag) const;
            uint32_t incomingWindow(bool throwFlag) const; // req'd
            uint32_t nextOutgoingId(bool throwFlag) const; // req'd
            uint32_t outgoingWindow(bool throwFlag) const; // req'd
            uint32_t handle(bool throwFlag) const;
            uint32_t deliveryCount(bool throwFlag) const;
            uint32_t linkCredit(bool throwFlag) const;
            uint32_t available(bool throwFlag) const;
            bool drain(bool throwFlag) const;
            bool echo(bool throwFlag) const;
            const AmqpMap_t& properties(bool throwFlag) const;

            static const FieldTypeList_t s_fieldTypeList;
        };



#define AMQPTRANSFER_FN_HANDLE "handle"
#define AMQPTRANSFER_FN_DELIVERY_ID "delivery-id"
#define AMQPTRANSFER_FN_DELIVERY_TAG "delivery-tag"
#define AMQPTRANSFER_FN_MESSAGE_FORMAT "message-format"
#define AMQPTRANSFER_FN_SETTLED "settled"
#define AMQPTRANSFER_FN_MORE "more"
#define AMQPTRANSFER_FN_RCV_SETTLE_MODE "rcv-settle-mode"
#define AMQPTRANSFER_FN_STATE "state"
#define AMQPTRANSFER_FN_RESUME "resume"
#define AMQPTRANSFER_FN_ABORTED "aborted"
#define AMQPTRANSFER_FN_BATCHABLE "batchable"
        class AmqpTransfer: public Performative {
        public:
            AmqpTransfer(uint64_t packetNum, std::size_t dataOffset, AmqpList* fieldListPtr);
            virtual ~AmqpTransfer();

            inline const FieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; };
            inline PerformativeType_t type() const override { return PerformativeType_t::TRANSFER; }
            void validate() override;

            // convenience getter methods
            uint32_t handle(bool throwFlag) const; // req'd
            uint32_t deliveryId(bool throwFlag) const;
            const AmqpBinary_t& deliveryTag(bool throwFlag) const;
            uint32_t messageFormat(bool throwFlag) const;
            const AmqpMessageFormat* messageFormatObj(bool throwFlag) const;
            bool settled(bool throwFlag) const;
            bool more(bool throwFlag) const;
            AmqpReceiverSettleMode_t rcvSettleMode(bool throwFlag) const;
            const CompositeType* state(bool throwFlag) const;
            bool resume(bool throwFlag) const;
            bool aborted(bool throwFlag) const;
            bool batchable(bool throwFlag) const;

            static const FieldTypeList_t s_fieldTypeList;
        };



#define AMQPDISPOSITION_FN_ROLE "role"
#define AMQPDISPOSITION_FN_FIRST "first"
#define AMQPDISPOSITION_FN_LAST "last"
#define AMQPDISPOSITION_FN_SETTLED "settled"
#define AMQPDISPOSITION_FN_STATE "state"
#define AMQPDISPOSITION_FN_BATCHABLE "batchable"
        class AmqpDisposition: public Performative {
        public:
            AmqpDisposition(uint64_t packetNum, std::size_t dataOffset, AmqpList* fieldListPtr);
            virtual ~AmqpDisposition();

            inline const FieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; };
            inline PerformativeType_t type() const override { return PerformativeType_t::DISPOSITION; }
            void validate() override;

            // convenience getter methods
            AmqpRole_t role(bool throwFlag) const;
            uint32_t first(bool throwFlag) const;
            uint32_t last(bool throwFlag) const;
            bool settled(bool throwFlag) const;
            const CompositeType* state(bool throwFlag) const;
            bool batchable(bool throwFlag) const;

            static const FieldTypeList_t s_fieldTypeList;
        };



#define AMQPDETACH_FN_HANDLE "handle"
#define AMQPDETACH_FN_CLOSED "closed"
#define AMQPDETACH_FN_ERROR "error"
        class AmqpDetach: public Performative {
        public:
            AmqpDetach(uint64_t packetNum, std::size_t dataOffset, AmqpList* fieldListPtr);
            virtual ~AmqpDetach();

            inline const FieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; };
            inline PerformativeType_t type() const override { return PerformativeType_t::DETACH; }
            void validate() override;

            // convenience getter methods
            uint32_t handle(bool throwFlag) const;
            bool cloased(bool throwFlag) const;
            const AmqpErrorRecord* error(bool throwFlag) const;

            static const FieldTypeList_t s_fieldTypeList;
        };



#define AMQPEND_FN_ERROR "error"

        class AmqpEnd: public Performative {
        public:
            AmqpEnd(uint64_t packetNum, std::size_t dataOffset, AmqpList* fieldListPtr);
            virtual ~AmqpEnd();

            inline const FieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; };
            inline PerformativeType_t type() const override { return PerformativeType_t::END; }
            void validate() override;

            // convenience getter methods
            const AmqpErrorRecord* error(bool throwFlag) const;

            static const FieldTypeList_t s_fieldTypeList;
        };



#define AMQPCLOSE_FN_ERROR "error"

        class AmqpClose: public Performative {
        public:
            AmqpClose(uint64_t packetNum, std::size_t dataOffset, AmqpList* fieldListPtr);
            virtual ~AmqpClose();

            inline const FieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; };
            inline PerformativeType_t type() const override { return PerformativeType_t::CLOSE; }
            void validate() override;

            // convenience getter methods
            const AmqpErrorRecord* error(bool throwFlag) const;

            static const FieldTypeList_t s_fieldTypeList;
        };


    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQP10_PERFORMATIVE_HPP_ */
