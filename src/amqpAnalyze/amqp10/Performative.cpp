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
            if (margin > 0 && !ignoreFirstMargin) oss << "\n" << std::string(margin, ' ');
            oss << "[" << std::setw(4) << std::setfill('0') << std::hex << _frameOffset
                << "] " << std::b_cyan << "AMQP frame" << std::res << ": size=0x" << _frameSize << " doff=0x" << (int)_dataOffset
                << " type=0x" << (int)_type << " (" << s_frameTypeName[_type] << ")";
            if (_type == 0) { // AMQP frame
                oss << " chnl=0x" << _typeSpecific;
            }
            if (_extendedHeaderSize > 0) {
                oss << " extHdrSize=0x" << _extendedHeaderSize;
            }
            oss << ":";
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

        Performative::Performative(std::size_t frameOffset,
                                   uint32_t frameSize,
                                   uint8_t dataOffset,
                                   frameType_t type,
                                   uint16_t typeSpecific,
                                   AmqpList* fieldListPtr):
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
                oss << "\n" << std::string(margin, ' ') << "[" << std::setfill('0') << std::setw(4) << _frameOffset << "] + ";
                //_fieldListPtr->appendString(oss, margin + 9);
                appendFieldList(oss, margin + 9);
            }
            if (!_sectionPtrList.empty()) {
                for (sectionPtrList_citr_t i=_sectionPtrList.cbegin(); i!=_sectionPtrList.cend(); ++i) {
                    oss << "\n" << std::string(margin, ' ');
                    (*i)->appendString(oss, margin + 9);
                }
            }
        }

        void Performative::appendFieldList(std::ostringstream& oss, std::size_t margin) const {
            std::string t(typeStr());
            std::size_t l(margin + t.length() + 3);
            std::string m(l, ' ');
            oss << std::b_yellow << t << std::res << ": [";
            int fieldNumberIndex = 0;
            for (amqp_list_citr_t i=_fieldListPtr->value().cbegin(); i<_fieldListPtr->value().cend(); ++i) {
                if (i!=_fieldListPtr->value().cbegin()) oss << "\n" << m;
                CompoundType* cPtr(dynamic_cast<CompoundType*>(*i));
                if (cPtr) {
                    cPtr->appendString(oss, margin);
                } else {
                    CompositeType* compositePtr(dynamic_cast<CompositeType*>(*i));
                    if (compositePtr) {
                        oss << compositePtr->toString(margin + 2);
                    } else {
                        FieldType& ft = fieldTypeList()[fieldNumberIndex];
                        oss << ft._fieldName << "(" << (*i)->valueStr() << ")";
                    }
                }
                fieldNumberIndex++;
            }
            oss << "]";
        }

        std::size_t Performative::frameSize() const {
            return _frameSize;;
        }

        // static
        FrameHeader* Performative::decode(FrameBuffer& frameBuffer) {
            frameBuffer.setFrameOffsetSnapshot();
            const uint32_t frameSize = frameBuffer.getUint32();
            const uint8_t dataOffset = frameBuffer.getUint8();
            const frameType_t type = (frameType_t)frameBuffer.getUint8(); // TODO: ugly cast, fix by performing validity check
            const uint16_t typeSpecific = frameBuffer.getUint16();
            if (frameBuffer.getRemaining() == 0) { // empty frame (heartbeat)
                return new FrameHeader(frameBuffer.getFrameOffsetSnapshot(), frameSize, dataOffset, type, typeSpecific);
            }
            switch (type) {
                case frameType_t::AMQP_FRAME: return decodeAmqpFrame(frameSize, dataOffset, type, typeSpecific, frameBuffer);
                case frameType_t::SASL_FRAME: return decodeSaslFrame(frameSize, dataOffset, type, typeSpecific, frameBuffer);
                default: throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "Unknown frame type: 0x" << (int)type));
            };
        }

        // static
        Performative* Performative::decodeAmqpFrame(uint32_t frameSize,
                                                    uint8_t dataOffset,
                                                    frameType_t type,
                                                    uint16_t typeSpecific,
                                                    FrameBuffer& frameBuffer) {
            frameBuffer.setFrameOffsetSnapshot();
            const uint8_t lfb = frameBuffer.getUint8();
            if (lfb != 0) {
                throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "Unexpected leading frame byte: expected 0x0, found 0x" << (int)lfb));
            }
            Performative* performativePtr = nullptr;
            std::unique_ptr<PrimitiveType> descriptorPtr((PrimitiveType*)Type::decode(frameBuffer));
            switch (descriptorPtr->type()) {
                case amqpPrimitiveType_t::ULONG_TYPE: {
                    AmqpUlong* longDescriptorPtr = (AmqpUlong*)descriptorPtr.get();
                    switch((performativeType_t)longDescriptorPtr->value()) {
                        case performativeType_t::OPEN:
                            performativePtr = new AmqpOpen(frameBuffer.getFrameOffsetSnapshot(),
                                                           frameSize,
                                                           dataOffset,
                                                           type,
                                                           typeSpecific,
                                                           decodeFieldList(frameBuffer, AmqpOpen::s_fieldTypeList));
                            break;
                        case performativeType_t::BEGIN:
                            performativePtr = new AmqpBegin(frameBuffer.getFrameOffsetSnapshot(),
                                                            frameSize,
                                                            dataOffset,
                                                            type,
                                                            typeSpecific,
                                                            decodeFieldList(frameBuffer, AmqpBegin::s_fieldTypeList));
                            break;
                        case performativeType_t::ATTACH:
                            performativePtr = new AmqpAttach(frameBuffer.getFrameOffsetSnapshot(),
                                                             frameSize,
                                                             dataOffset,
                                                             type,
                                                             typeSpecific,
                                                             decodeFieldList(frameBuffer, AmqpAttach::s_fieldTypeList));
                            break;
                        case performativeType_t::FLOW:
                            performativePtr = new AmqpFlow(frameBuffer.getFrameOffsetSnapshot(),
                                                           frameSize,
                                                           dataOffset,
                                                           type,
                                                           typeSpecific,
                                                           decodeFieldList(frameBuffer, AmqpFlow::s_fieldTypeList));
                            break;
                        case performativeType_t::TRANSFER:
                            performativePtr = new AmqpTransfer(frameBuffer.getFrameOffsetSnapshot(),
                                                               frameSize,
                                                               dataOffset,
                                                               type,
                                                               typeSpecific,
                                                               decodeFieldList(frameBuffer, AmqpTransfer::s_fieldTypeList));
                            break;
                        case performativeType_t::DISPOSITION:
                            performativePtr = new AmqpDisposition(frameBuffer.getFrameOffsetSnapshot(),
                                                                  frameSize,
                                                                  dataOffset,
                                                                  type,
                                                                  typeSpecific,
                                                                  decodeFieldList(frameBuffer, AmqpDisposition::s_fieldTypeList));
                            break;
                        case performativeType_t::DETACH:
                            performativePtr = new AmqpDetach(frameBuffer.getFrameOffsetSnapshot(),
                                                             frameSize,
                                                             dataOffset,
                                                             type,
                                                             typeSpecific,
                                                             decodeFieldList(frameBuffer, AmqpDetach::s_fieldTypeList));
                            break;
                        case performativeType_t::END:
                            performativePtr = new AmqpEnd(frameBuffer.getFrameOffsetSnapshot(),
                                                          frameSize,
                                                          dataOffset,
                                                          type,
                                                          typeSpecific,
                                                          decodeFieldList(frameBuffer, AmqpEnd::s_fieldTypeList));
                            break;
                        case performativeType_t::CLOSE:
                            performativePtr = new AmqpClose(frameBuffer.getFrameOffsetSnapshot(),
                                                            frameSize,
                                                            dataOffset,
                                                            type,
                                                            typeSpecific,
                                                            decodeFieldList(frameBuffer, AmqpClose::s_fieldTypeList));
                            break;
                        default:
                            throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "Invalid AMQP performative descriptor: " << longDescriptorPtr->valueStr()));
                    }
                    break;
                }
                case amqpPrimitiveType_t::SYMBOL_TYPE:
                    throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "AMQP symbol descriptors not handled (" << descriptorPtr->typeStr() << ")"));
                default:
                    throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "Invalid AMQP descriptor type:" << descriptorPtr->typeStr()));
            }
            while (!frameBuffer.isEmpty()) {
                performativePtr->_sectionPtrList.push_back(Section::decode(frameBuffer));
            }
            return performativePtr;
        }

        // static
        Performative* Performative::decodeSaslFrame(uint32_t frameSize,
                                                    uint8_t dataOffset,
                                                    frameType_t type,
                                                    uint16_t typeSpecific,
                                                    FrameBuffer& frameBuffer) {
            throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() <<  "SASL frames not supported (yet)"));
        }

        // static
        AmqpList* Performative::decodeFieldList(FrameBuffer& frameBuffer,
                                                fieldTypeList_t& fieldTypeList) {
            uint8_t code = frameBuffer.getUint8();
            switch (code) {
                case 0x45:
                    return new AmqpList();
                case 0xc0: {
                    const uint8_t size(frameBuffer.getUint8());
                    const uint8_t count(frameBuffer.getUint8());
                    return decodeFieldList(size - sizeof(uint8_t), count, frameBuffer, fieldTypeList);
                }
                case 0xd0: {
                    const uint32_t size(frameBuffer.getUint32());
                    const uint32_t count(frameBuffer.getUint32());
                    return decodeFieldList(size - sizeof(uint32_t), count, frameBuffer, fieldTypeList);
                }
                default:
                    throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "Invalid AMQP type code for list: 0x" << std::hex << (int)code));
            }
        }

        // static
        AmqpList* Performative::decodeFieldList(std::size_t size,
                                                std::size_t count,
                                                FrameBuffer& frameBuffer,
                                                fieldTypeList_t& fieldTypeList) {
            if (count > fieldTypeList.size()) {
                throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "FieldList too large for fieldTypeList: FieldList size="
                                         << count << "; fieldTypeList size=" << fieldTypeList.size()));
            }
            const size_t startOffs = frameBuffer.getOffset();
            AmqpList* parameterListPtr = new AmqpList();
            for (std::size_t i=0; i<count; ++i) {
                parameterListPtr->value().push_back(decodeField(fieldTypeList[i], frameBuffer));
            }
            if (frameBuffer.getOffset() - startOffs != size) {
                throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "List size mismatch: expected 0x" << std::hex
                                         << size << ", found 0x" << (frameBuffer.getOffset() - startOffs)));
            }
            return parameterListPtr;
        }

        // static
        Type* Performative::decodeField(const FieldType& fieldType, FrameBuffer& frameBuffer) {
            uint8_t code = frameBuffer.getUint8();
            if (code == 0x40) {
                if (fieldType._mandatoryFlag) throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "Null found in mandatory field"));
                return new AmqpNull;
            }
            switch (fieldType._unionType) {
                case FieldType::type::PRIMITIVE:
                    switch (fieldType._types._primitiveType) {
                        case amqpPrimitiveType_t::MILLISECONDS_TYPE:
                            switch (code) {
                                case 0x43: return new AmqpMilliseconds(0);
                                case 0x52: return new AmqpMilliseconds(frameBuffer.getUint8());
                                case 0x70: return new AmqpMilliseconds(frameBuffer.getUint32());
                                default: throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "Invalid type code for \"milliseconds\" field: 0x"
                                                                  << std::hex << (int)code));
                            }
                        case amqpPrimitiveType_t::LANGUAGE_TAG_TYPE:
                            switch (code) {
                                case 0xa3: {
                                    AmqpLanguageTag* lt = new AmqpLanguageTag();
                                    frameBuffer.getSymbol(lt->value(), frameBuffer.getUint8());
                                    return lt;
                                }
                                case 0xb3: {
                                    AmqpLanguageTag* lt = new AmqpLanguageTag();
                                    frameBuffer.getSymbol(lt->value(), frameBuffer.getUint32());
                                    return lt;
                                }
                                default: throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "Invalid type code for \"ietf-language-tag\" field: 0x"
                                                                  << std::hex << (int)code));
                            }
                        case amqpPrimitiveType_t::FIELDS_TYPE:
                                switch (code) {
                                case 0xc1: {
                                    AmqpFields* f = new AmqpFields();
                                    const uint8_t size(frameBuffer.getUint8());
                                    const uint8_t count(frameBuffer.getUint8());
                                    frameBuffer.getMap(f->value(), size - sizeof(count), count);
                                    return f;
                                }
                                case 0xd1: {
                                    AmqpFields* f = new AmqpFields();
                                    const uint32_t size(frameBuffer.getUint32());
                                    const uint32_t count(frameBuffer.getUint32());
                                    frameBuffer.getMap(f->value(), size - sizeof(count), count);
                                    return f;
                                }
                                default: throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "Invalid type code for \"fields\" field: 0x" << std::hex << (int)code));
                            }
                        case amqpPrimitiveType_t::TRANSFER_NUMBER_TYPE:
                            switch (code) {
                                case 0x70: return new AmqpTransferNum(frameBuffer.getUint32());
                                case 0x52: return new AmqpTransferNum(frameBuffer.getUint8());
                                case 0x43: return new AmqpTransferNum(0);
                                default: throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "Invalid type code for \"transfer-number\" field: 0x" << std::hex << (int)code));
                            }
                        case amqpPrimitiveType_t::HANDLE_TYPE:
                            switch (code) {
                                case 0x70: return new AmqpHandle(frameBuffer.getUint32());
                                case 0x52: return new AmqpHandle(frameBuffer.getUint8());
                                case 0x43: return new AmqpHandle(0);
                                default: throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "Invalid type code for \"handle\" field: 0x" << std::hex << (int)code));
                            }
                        case amqpPrimitiveType_t::ROLE_TYPE:
                            switch (code) {
                                case 0x41: return new AmqpRole((amqp_role_t)true);
                                case 0x42: return new AmqpRole((amqp_role_t)false);
                                case 0x56: return new AmqpRole((amqp_role_t)frameBuffer.getBool());
                                default: throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "Invalid type code for \"role\" field: 0x" << std::hex << (int)code));
                            }
                        case amqpPrimitiveType_t::SENDER_SETTLE_MODE_TYPE:
                            switch (code) {
                                case 0x50: return new AmqpSenderSettleMode((amqp_sender_settle_mode_t)frameBuffer.getUint8());
                                default: throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "Invalid type code for \"sender-settle-mode\" field: 0x" << std::hex << (int)code));
                            }
                        case amqpPrimitiveType_t::RECEIVER_SETTLE_MODE_TYPE:
                            switch (code) {
                                case 0x50: return new AmqpReceiverSettleMode((amqp_receiver_settle_mode_t)frameBuffer.getUint8());
                                default: throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "Invalid type code for \"receiver-settle-mode\" field: 0x" << std::hex << (int)code));
                            }
                        case amqpPrimitiveType_t::SEQUENCE_NUMBER_TYPE:
                            switch (code) {
                                case 0x70: return new AmqpSequenceNum(frameBuffer.getUint32());
                                case 0x52: return new AmqpSequenceNum(frameBuffer.getUint8());
                                case 0x43: return new AmqpSequenceNum(0);
                                default: throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "Invalid type code for \"sequence-no\" field: 0x" << std::hex << (int)code));
                            }
                        case amqpPrimitiveType_t::DELIVERY_NUMBER_TYPE:
                            switch (code) {
                                case 0x70: return new AmqpDeliveryNum(frameBuffer.getUint32());
                                case 0x52: return new AmqpDeliveryNum(frameBuffer.getUint8());
                                case 0x43: return new AmqpDeliveryNum(0);
                                default: throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "Invalid type code for \"delivery-number\" field: 0x" << std::hex << (int)code));
                            }
                        case amqpPrimitiveType_t::DELIVERY_TAG_TYPE:
                            switch (code) {
                                case 0xa0: {
                                    AmqpDeliveryTag* d = new AmqpDeliveryTag();
                                    frameBuffer.getBinary(d->value(), frameBuffer.getUint8());
                                    return d;
                                }
                                case 0xb0: {
                                    AmqpDeliveryTag* d = new AmqpDeliveryTag();
                                    frameBuffer.getBinary(d->value(), frameBuffer.getUint32());
                                    return d;
                                }
                                default: throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "Invalid type code for \"delivery-tag\" field: 0x" << std::hex << (int)code));
                            }
                        case amqpPrimitiveType_t::MESSAGE_FORMAT_TYPE:
                            switch (code) {
                                case 0x70: return new AmqpUint(frameBuffer.getUint32());
                                case 0x52: return new AmqpUint(frameBuffer.getUint8());
                                case 0x43: return new AmqpUint(0);
                                default: throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "Invalid type code for \"message-format\" field: 0x" << std::hex << (int)code));
                            }
                        default:
                            return Type::decodePrimitive(code, frameBuffer);
                    }
                case FieldType::type::WILDCARD: {
                    CompositeType* compositeTypePtr = Type::decodeComposite(frameBuffer);
                    if (fieldType._requiresList.size() == 1) {
                        amqpRequiresProvides_t requires = fieldType._requiresList.front();
                        if (!compositeTypePtr->provides(requires, compositeTypePtr->providesList())) {
                            throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "Provides - requires mismatch: requires=\"" << ProvidesRequiresNames[requires] << "\""));
                        }
                    } else {
                        throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "fieldType._requiresList.size()!=1: " << fieldType._requiresList.size()));
                    }
                    return compositeTypePtr;
                }
                default:
                    throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "Unknown field type: 0x" << std::hex << (int)fieldType._unionType));
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
            FieldType("state", "*", false, false, {amqpRequiresProvides_t::DELIVERY_STATE}),
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
            FieldType("state", "*", false, false, {amqpRequiresProvides_t::DELIVERY_STATE}),
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
            FieldType("error", "error", false, false)
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
            FieldType("error", "error", false, false)
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
            FieldType("error", "error", false, false)
        };



    } /* namespace amqp10 */
} /* namespace amqpAnalyze */
