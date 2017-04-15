/*
 * FrameHeader.cpp
 *
 *  Created on: Apr 3, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/amqp10/Performative.hpp>

#include <amqpAnalyze/amqp10/FieldType.hpp>
#include <amqpAnalyze/amqp10/Type.hpp>
#include <amqpAnalyze/Error.hpp>
#include <amqpAnalyze/amqp10/FrameBuffer.hpp>
#include <amqpAnalyze/amqp10/Section.hpp>

#include <iomanip>
#include <netinet/in.h>

namespace amqpAnalyze
{
    namespace amqp10
    {

        //-- class FrameHeader ---

        FrameHeader::FrameHeader(std::size_t frameOffset, uint32_t frameSize, uint8_t dataOffset, frameType_t type, uint16_t typeSpecific):
            FrameBase(frameOffset),
            _frameSize(frameSize),
            _dataOffset(dataOffset),
            _type(type),
            _typeSpecific(typeSpecific),
            _dataOffsetBytes(_dataOffset*4),
            _extendedHeaderSize(_dataOffsetBytes - 8)
        {
            if (_frameSize < 8) {
                throw amqpAnalyze::Error(MSG("[" << std::hex << std::setfill('0') << std::setw(4) << frameOffset
                                             << "] Invalid AMQP frame size: 0x" << _frameSize));
            }
            if (_dataOffset < 2) {
                throw amqpAnalyze::Error(MSG("[" << std::hex << std::setfill('0') << std::setw(4) << frameOffset
                                                 << "] Invalid AMQP frame data offset (doff): 0x" << _dataOffset));
            }
        }

        FrameHeader::~FrameHeader() {}

        void FrameHeader::appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const {
            if (margin > 0 && !ignoreFirstMargin) oss << std::endl << std::string(margin, ' ');
            oss << "[" << std::setw(4) << std::setfill('0') << std::hex << _frameOffset
                << "] AMQP frame: size=0x" << _frameSize << " doff=0x" << (int)_dataOffset
                << " type=0x" << (int)_type << " (" << s_frameTypeName[_type] << ")";
            if (_type == 0) { // AMQP frame
                oss << " chnl=0x" << _typeSpecific;
            }
            if (_extendedHeaderSize > 0) {
                oss << " extHdrSize=0x" << _extendedHeaderSize;
            }
            oss << ": " << typeStr();
        }
        std::size_t FrameHeader::frameSize() const {
            return 8;
        }
        // static
        std::map<frameType_t, const char*> FrameHeader::s_frameTypeName = {
            {AMQP_FRAME, "AMQP"},
            {SASL_FRAME, "SASL"}
        };


        //-- class Performative ---

        Performative::Performative(std::size_t frameOffset, uint32_t frameSize, uint8_t dataOffset, frameType_t type, uint16_t typeSpecific, AmqpList* fieldListPtr):
                FrameHeader(frameOffset, frameSize, dataOffset, type, typeSpecific),
                _fieldListPtr(fieldListPtr),
                _sectionPtrList()
        {}

        Performative::~Performative() {
            for (amqp_list_itr_t i=_fieldListPtr->value().begin(); i!=_fieldListPtr->value().end(); ++i) {
                delete *i;
            }
            _fieldListPtr->value().clear();
            delete _fieldListPtr;
            for (sectionPtrList_itr_t j=_sectionPtrList.begin(); j!=_sectionPtrList.end(); ++j) {
                delete *j;
            }
            _sectionPtrList.clear();
        }

        void Performative::appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const {
            FrameHeader::appendString(oss, margin, ignoreFirstMargin);
            if (_fieldListPtr != nullptr) {
                oss << std::endl << std::string(margin + 9, ' ');
                _fieldListPtr->appendString(oss, margin + 9);
            }
            if (!_sectionPtrList.empty()) {
                for (sectionPtrList_citr_t i=_sectionPtrList.cbegin(); i!=_sectionPtrList.cend(); ++i) {
                    oss << std::endl << std::string(margin + 9, ' ');
                    (*i)->appendString(oss, margin + 9);
                }
            }
        }

        std::size_t Performative::frameSize() const {
            return _frameSize;;
        }

        // static
        FrameHeader* Performative::decode(std::size_t frameOffset, FrameBuffer& b) {
            const uint32_t frameSize = b.getUint32();
            const uint8_t dataOffset = b.getUint8();
            const frameType_t type = (frameType_t)b.getUint8(); // TODO: ugly cast, fix by performing validity check
            const uint16_t typeSpecific = b.getUint16();
            if (b.getRemaining() == 0) { // empty frame (heartbeat)
                return new FrameHeader(frameOffset, frameSize, dataOffset, type, typeSpecific);
            }
            switch (type) {
                case frameType_t::AMQP_FRAME: return decodeAmqpFrame(frameOffset, frameSize, dataOffset, type, typeSpecific, b);
                case frameType_t::SASL_FRAME: return decodeSaslFrame(frameOffset, frameSize, dataOffset, type, typeSpecific, b);
            };
        }

        // static
        Performative* Performative::decodeAmqpFrame(std::size_t frameOffset,
                                                    uint32_t frameSize,
                                                    uint8_t dataOffset,
                                                    frameType_t type,
                                                    uint16_t typeSpecific,
                                                    FrameBuffer& frameBuffer) {

            const uint8_t lfb = frameBuffer.getUint8();
            if (lfb != 0) {
                throw amqpAnalyze::Error(MSG("[" << std::hex << std::setfill('0') << std::setw(4) << frameOffset
                                             << "] Unexpected leading frame byte: expected 0x0, found 0x" << (int)lfb));
            }
            Performative* performativePtr = nullptr;
            std::unique_ptr<PrimitiveType> descriptorPtr((PrimitiveType*)Type::decode(frameBuffer));
            switch (descriptorPtr->type()) {
                case amqpPrimitiveType_t::ULONG_TYPE: {
                    AmqpUlong* longDescriptorPtr = (AmqpUlong*)descriptorPtr.get(); // TODO: This feels ugly, need more elegant solution here
                    switch((performativeType_t)longDescriptorPtr->value()) {
                        case performativeType_t::OPEN:
                            performativePtr = new AmqpOpen(frameOffset, frameSize, dataOffset, type, typeSpecific, (AmqpList*)Type::decode(frameBuffer));
                            break;
                        case performativeType_t::BEGIN:
                            performativePtr = new AmqpBegin(frameOffset, frameSize, dataOffset, type, typeSpecific, (AmqpList*)Type::decode(frameBuffer));
                            break;
                        case performativeType_t::ATTACH:
                            performativePtr = new AmqpAttach(frameOffset, frameSize, dataOffset, type, typeSpecific, (AmqpList*)Type::decode(frameBuffer));
                            break;
                        case performativeType_t::FLOW:
                            performativePtr = new AmqpFlow(frameOffset, frameSize, dataOffset, type, typeSpecific, (AmqpList*)Type::decode(frameBuffer));
                            break;
                        case performativeType_t::TRANSFER:
                            performativePtr = new AmqpTransfer(frameOffset, frameSize, dataOffset, type, typeSpecific, (AmqpList*)Type::decode(frameBuffer));
                            break;
                        case performativeType_t::DISPOSITION:
                            performativePtr = new AmqpDisposition(frameOffset, frameSize, dataOffset, type, typeSpecific, (AmqpList*)Type::decode(frameBuffer));
                            break;
                        case performativeType_t::DETACH:
                            performativePtr = new AmqpDetach(frameOffset, frameSize, dataOffset, type, typeSpecific, (AmqpList*)Type::decode(frameBuffer));
                            break;
                        case performativeType_t::END:
                            performativePtr = new AmqpEnd(frameOffset, frameSize, dataOffset, type, typeSpecific, (AmqpList*)Type::decode(frameBuffer));
                            break;
                        case performativeType_t::CLOSE:
                            performativePtr = new AmqpClose(frameOffset, frameSize, dataOffset, type, typeSpecific, (AmqpList*)Type::decode(frameBuffer));
                            break;
                        default:
                            throw amqpAnalyze::Error(MSG("[" << std::hex << std::setfill('0') << std::setw(4) << frameOffset
                                                         << "] Invalid AMQP performative descriptor: " << longDescriptorPtr->valueStr()));
                    }
                    break;
                }
                case amqpPrimitiveType_t::SYMBOL_TYPE:
                    throw amqpAnalyze::Error(MSG("[" << std::hex << std::setfill('0') << std::setw(4) << frameOffset
                                                 << "] AMQP symbol descriptors not handled (" << descriptorPtr->typeStr() << ")"));
                default:
                    throw amqpAnalyze::Error(MSG("[" << std::hex << std::setfill('0') << std::setw(4) << frameOffset
                                                 << "] Invalid AMQP descriptor type:" << descriptorPtr->typeStr()));
            }
            while (!frameBuffer.isEmpty()) {
                performativePtr->_sectionPtrList.push_back(Section::decode(frameBuffer));
            }
            return performativePtr;
        }

        // static
        Performative* Performative::decodeSaslFrame(std::size_t frameOffset,
                                                    uint32_t frameSize,
                                                    uint8_t dataOffset,
                                                    frameType_t type,
                                                    uint16_t typeSpecific,
                                                    FrameBuffer& b) {
            throw amqpAnalyze::Error(MSG("[" << std::hex << std::setfill('0') << std::setw(4) << frameOffset
                                         << "] SASL frames not supported (yet)"));
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

        AmqpOpen::AmqpOpen(std::size_t frameOffset,
                           uint32_t frameSize,
                           uint8_t dataOffset,
                           frameType_t type,
                           uint16_t typeSpecific,
                           AmqpList* fieldListPtr):
            Performative(frameOffset, frameSize, dataOffset, type, typeSpecific, fieldListPtr)
        {}

        AmqpOpen::~AmqpOpen() {}

        // static
        fieldTypeList_t AmqpOpen::s_fieldTypeList = {
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


        //-- class AmqpBegin ---

        AmqpBegin::AmqpBegin(std::size_t frameOffset,
                             uint32_t frameSize,
                             uint8_t dataOffset,
                             frameType_t type,
                             uint16_t typeSpecific,
                             AmqpList* fieldListPtr):
            Performative(frameOffset, frameSize, dataOffset, type, typeSpecific, fieldListPtr)
        {}

        AmqpBegin::~AmqpBegin() {}

        // static
        fieldTypeList_t AmqpBegin::s_fieldTypeList = {
            FieldType("remote-channel", amqpPrimitiveType_t::USHORT_TYPE, false, false),
            FieldType("next-outgoing-id", amqpPrimitiveType_t::TRANSFER_NUMBER_TYPE, true, false),
            FieldType("incoming-window", amqpPrimitiveType_t::UINT_TYPE, true, false),
            FieldType("outgoing-window", amqpPrimitiveType_t::UINT_TYPE, true, false),
            FieldType("handle-max", amqpPrimitiveType_t::HANDLE_TYPE, false, false),
            FieldType("offered-capabilities", amqpPrimitiveType_t::SYMBOL_TYPE, false, true),
            FieldType("desired-capabilities", amqpPrimitiveType_t::SYMBOL_TYPE, false, true),
            FieldType("properties", amqpPrimitiveType_t::FIELDS_TYPE, false, false)
        };


        //-- class AmqpAttach ---

        AmqpAttach::AmqpAttach(std::size_t frameOffset,
                               uint32_t frameSize,
                               uint8_t dataOffset,
                               frameType_t type,
                               uint16_t typeSpecific,
                               AmqpList* fieldListPtr):
            Performative(frameOffset, frameSize, dataOffset, type, typeSpecific, fieldListPtr)
        {}

        AmqpAttach::~AmqpAttach() {}

        // static
        fieldTypeList_t AmqpAttach::s_fieldTypeList = {
            FieldType("name", amqpPrimitiveType_t::STRING_TYPE, true, false),
            FieldType("handle", amqpPrimitiveType_t::HANDLE_TYPE, true, false),
            FieldType("role", amqpPrimitiveType_t::ROLE_TYPE, true, false),
            FieldType("snd-settle-mode", amqpPrimitiveType_t::SENDER_SETTLE_MODE_TYPE, false, false),
            FieldType("rcv-settle-mode", amqpPrimitiveType_t::RECEIVER_SETTLE_MODE_TYPE, false, false),
            FieldType("source", '*', false, false, {amqpRequiresProvides_t::SOURCE}),
            FieldType("target", '*', false, false, {amqpRequiresProvides_t::TARGET}),
            FieldType("unsettled", amqpPrimitiveType_t::MAP_TYPE, false, false),
            FieldType("incomplete-unsettled", amqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType("initial-delivery-count", amqpPrimitiveType_t::SEQUENCE_NUMBER_TYPE, false, false),
            FieldType("max-message-size", amqpPrimitiveType_t::ULONG_TYPE, false, false),
            FieldType("offered-capabilities", amqpPrimitiveType_t::SYMBOL_TYPE, false, true),
            FieldType("desired-capabilities", amqpPrimitiveType_t::SYMBOL_TYPE, false, true),
            FieldType("properties", amqpPrimitiveType_t::FIELDS_TYPE, false, false)
        };


        //-- class AmqpFlow ---

        AmqpFlow::AmqpFlow(std::size_t frameOffset,
                           uint32_t frameSize,
                           uint8_t dataOffset,
                           frameType_t type,
                           uint16_t typeSpecific,
                           AmqpList* fieldListPtr):
            Performative(frameOffset, frameSize, dataOffset, type, typeSpecific, fieldListPtr)
        {}

        AmqpFlow::~AmqpFlow() {}

        // static
        fieldTypeList_t AmqpFlow::s_fieldTypeList = {
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


        //-- class AmqpTransfer ---

        AmqpTransfer::AmqpTransfer(std::size_t frameOffset,
                                   uint32_t frameSize,
                                   uint8_t dataOffset,
                                   frameType_t type,
                                   uint16_t typeSpecific,
                                   AmqpList* fieldListPtr):
            Performative(frameOffset, frameSize, dataOffset, type, typeSpecific, fieldListPtr)
        {}

        AmqpTransfer::~AmqpTransfer() {}

        // static
        fieldTypeList_t AmqpTransfer::s_fieldTypeList = {
            FieldType("handle", amqpPrimitiveType_t::HANDLE_TYPE, true, false),
            FieldType("delivery-id", amqpPrimitiveType_t::DELIVERY_NUMBER_TYPE, false, false),
            FieldType("delivery-tag", amqpPrimitiveType_t::DELIVERY_TAG_TYPE, false, false),
            FieldType("message-format", amqpPrimitiveType_t::MESSAGE_FORMAT_TYPE, false, false),
            FieldType("settled", amqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType("more", amqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType("rcv-settle-mode", amqpPrimitiveType_t::RECEIVER_SETTLE_MODE_TYPE, false, false),
            FieldType("state", '*', false, false, {amqpRequiresProvides_t::DELIVERY_STATE}),
            FieldType("resume", amqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType("aborted", amqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType("batchable", amqpPrimitiveType_t::BOOLEAN_TYPE, false, false)
        };


        //-- class AmqpDisposition ---

        AmqpDisposition::AmqpDisposition(std::size_t frameOffset,
                                         uint32_t frameSize,
                                         uint8_t dataOffset,
                                         frameType_t type,
                                         uint16_t typeSpecific,
                                         AmqpList* fieldListPtr):
            Performative(frameOffset, frameSize, dataOffset, type, typeSpecific, fieldListPtr)
        {}

        AmqpDisposition::~AmqpDisposition() {}

        // static
        fieldTypeList_t AmqpDisposition::s_fieldTypeList = {
            FieldType("role", amqpPrimitiveType_t::ROLE_TYPE, true, false),
            FieldType("first", amqpPrimitiveType_t::DELIVERY_NUMBER_TYPE, true, false),
            FieldType("last", amqpPrimitiveType_t::DELIVERY_NUMBER_TYPE, false, false),
            FieldType("settled", amqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType("state", '*', false, false, {amqpRequiresProvides_t::DELIVERY_STATE}),
            FieldType("batchable", amqpPrimitiveType_t::BOOLEAN_TYPE, false, false)
        };


        //-- class AmqpDetach ---

        AmqpDetach::AmqpDetach(std::size_t frameOffset,
                               uint32_t frameSize,
                               uint8_t dataOffset,
                               frameType_t type,
                               uint16_t typeSpecific,
                               AmqpList* fieldListPtr):
            Performative(frameOffset, frameSize, dataOffset, type, typeSpecific, fieldListPtr)
        {}

        AmqpDetach::~AmqpDetach() {}

        // static
        fieldTypeList_t AmqpDetach::s_fieldTypeList = {
            FieldType("handle", amqpPrimitiveType_t::HANDLE_TYPE, true, false),
            FieldType("closed", amqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType("error", amqpPrimitiveType_t::AMQP_ERROR_TYPE, false, false)
        };


        //-- class AmqpEnd ---

        AmqpEnd::AmqpEnd(std::size_t frameOffset,
                         uint32_t frameSize,
                         uint8_t dataOffset,
                         frameType_t type,
                         uint16_t typeSpecific,
                         AmqpList* fieldListPtr):
            Performative(frameOffset, frameSize, dataOffset, type, typeSpecific, fieldListPtr)
        {}

        AmqpEnd::~AmqpEnd() {}

        // static
        fieldTypeList_t AmqpEnd::s_fieldTypeList = {
            FieldType("error", amqpPrimitiveType_t::AMQP_ERROR_TYPE, false, false)
        };


        //-- class AmqpClose ---

        AmqpClose::AmqpClose(std::size_t frameOffset,
                             uint32_t frameSize,
                             uint8_t dataOffset,
                             frameType_t type,
                             uint16_t typeSpecific,
                             AmqpList* fieldListPtr):
            Performative(frameOffset, frameSize, dataOffset, type, typeSpecific, fieldListPtr)
        {}

        AmqpClose::~AmqpClose() {}

        // static
        fieldTypeList_t AmqpClose::s_fieldTypeList = {
            FieldType("error", amqpPrimitiveType_t::AMQP_ERROR_TYPE, false, false)
        };



    } /* namespace amqp10 */
} /* namespace amqpAnalyze */
