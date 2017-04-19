/*
 * Decoder.cpp
 *
 *  Created on: Apr 18, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/amqp10/Decoder.hpp>

#include <amqpAnalyze/amqp10/FieldType.hpp>
#include <amqpAnalyze/amqp10/FrameBuffer.hpp>
#include <amqpAnalyze/amqp10/Performative.hpp>
#include <amqpAnalyze/amqp10/Type.hpp>
#include <amqpAnalyze/Error.hpp>
#include <memory>

// debug
#include <iostream>

namespace amqpAnalyze
{
    namespace amqp10
    {

        Decoder::Decoder() {}

        Decoder::~Decoder() {}

        // static
        Type* Decoder::decode(FrameBuffer& frameBuffer) {
            uint8_t code = frameBuffer.getUint8();
            if (code > 0) return Decoder::decodePrimitive(code, frameBuffer);
            return Decoder::decodeComposite(frameBuffer);
        }

        // static
        PrimitiveType* Decoder::decodePrimitive(uint8_t code, FrameBuffer& frameBuffer, const char* fieldName) {
            switch(code) {

            // null
            case 0x40: return new AmqpNull(fieldName);

            // boolean
            case 0x41: return new AmqpBoolean(true, fieldName);
            case 0x42: return new AmqpBoolean(false, fieldName);
            case 0x56: return new AmqpBoolean(frameBuffer.getBool(), fieldName);

            // ubyte
            case 0x50: return new AmqpUbyte(frameBuffer.getUint8(), fieldName);

            // ushort
            case 0x60: return new AmqpUshort(frameBuffer.getUint16(), fieldName);

            // uint
            case 0x70: return new AmqpUint(frameBuffer.getUint32(), fieldName);
            case 0x52: return new AmqpUint(frameBuffer.getUint8(), fieldName);
            case 0x43: return new AmqpUint(0, fieldName);

            // ulong
            case 0x80: return new AmqpUlong(frameBuffer.getUint64(), fieldName);
            case 0x53: return new AmqpUlong(frameBuffer.getUint8(), fieldName);
            case 0x44: return new AmqpUlong(0, fieldName);

            // byte
            case 0x51: return new AmqpByte(frameBuffer.getInt8(), fieldName);

            // short
            case 0x61: return new AmqpShort(frameBuffer.getInt16(), fieldName);

            // int
            case 0x71: return new AmqpInt(frameBuffer.getInt32(), fieldName);
            case 0x54: return new AmqpInt(frameBuffer.getInt8(), fieldName);

            // long
            case 0x81: return new AmqpLong(frameBuffer.getInt64(), fieldName);
            case 0x55: return new AmqpLong(frameBuffer.getInt8(), fieldName);

            // float
            case 0x72: return new AmqpFloat(frameBuffer.getFloat(), fieldName);

            // double
            case 0x82: return new AmqpDouble(frameBuffer.getDouble(), fieldName);

            // decimal32
            case 0x74: {
                AmqpDecimal32* d = new AmqpDecimal32(fieldName);
                frameBuffer.getDecimal32(d->value());
                return d;
            }

            // decimal64
            case 0x84: {
                AmqpDecimal64* d = new AmqpDecimal64(fieldName);
                frameBuffer.getDecimal64(d->value());
                return d;
            }

            // decimal128
            case 0x94: {
                AmqpDecimal128* d = new AmqpDecimal128(fieldName);
                frameBuffer.getDecimal128(d->value());
                return d;
            }

            // char
            case 0x73: return new AmqpChar(frameBuffer.getChar(), fieldName);

            // timestamp
            case 0x83: return new AmqpTimestamp(frameBuffer.getUint64(), fieldName);

            // uuid
            case 0x98: {
                AmqpUuid* u = new AmqpUuid(fieldName);
                frameBuffer.getUuid(u->value());
                return u;
            }

            // binary
            case 0xa0: {
                AmqpBinary* d = new AmqpBinary(fieldName);
                frameBuffer.getBinary(d->value(), frameBuffer.getUint8());
                return d;
            }
            case 0xb0: {
                AmqpBinary* d = new AmqpBinary(fieldName);
                frameBuffer.getBinary(d->value(), frameBuffer.getUint32());
                return d;
            }

            // string
            case 0xa1: {
                AmqpString* s = new AmqpString(fieldName);
                frameBuffer.getString(s->value(), frameBuffer.getUint8());
                return s;
            }
            case 0xb1: {
                AmqpString* s = new AmqpString(fieldName);
                frameBuffer.getString(s->value(), frameBuffer.getUint32());
                return s;
            }

            // symbol
            case 0xa3: {
                AmqpSymbol* s = new AmqpSymbol(fieldName);
                frameBuffer.getSymbol(s->value(), frameBuffer.getUint8());
                return s;
            }
            case 0xb3: {
                AmqpSymbol* s = new AmqpSymbol(fieldName);
                frameBuffer.getSymbol(s->value(), frameBuffer.getUint32());
                return s;
            }

            // list
            case 0x45: return new AmqpList(fieldName);
            case 0xc0: {
                AmqpList* l = new AmqpList(fieldName);
                const uint8_t size(frameBuffer.getUint8());
                const uint8_t count(frameBuffer.getUint8());
                frameBuffer.getList(l->value(), size - sizeof(count), count);
                return l;
            }
            case 0xd0: {
                AmqpList* l = new AmqpList(fieldName);
                const uint32_t size(frameBuffer.getUint32());
                const uint32_t count(frameBuffer.getUint32());
                frameBuffer.getList(l->value(), size - sizeof(count), count);
                return l;
            }

            // map
            case 0xc1: {
                AmqpMap* m = new AmqpMap(fieldName);
                const uint8_t size(frameBuffer.getUint8());
                const uint8_t count(frameBuffer.getUint8());
                frameBuffer.getMap(m->value(), size - sizeof(count), count);
                return m;
            }
            case 0xd1: {
                AmqpMap* m = new AmqpMap(fieldName);
                const uint32_t size(frameBuffer.getUint32());
                const uint32_t count(frameBuffer.getUint32());
                frameBuffer.getMap(m->value(), size - sizeof(count), count);
                return m;
            }

            // array
            case 0xe0: {
                AmqpArray* a = new AmqpArray(fieldName);
                const uint8_t size(frameBuffer.getUint8());
                const uint8_t count(frameBuffer.getUint8());
                frameBuffer.getArray(a->value(), size - sizeof(count), count);
                return a;
            }
            case 0xf0: {
                AmqpArray* a = new AmqpArray(fieldName);
                const uint32_t size(frameBuffer.getUint32());
                const uint32_t count(frameBuffer.getUint32());
                frameBuffer.getArray(a->value(), size - sizeof(count), count);
                return a;
            }

            default:
                throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "Invalid AMQP primitive type code 0x" << std::hex << (int)code));
            }
        }

        // static
        CompositeType* Decoder::decodeComposite(FrameBuffer& frameBuffer, const char* fieldName) {
            std::unique_ptr<PrimitiveType> descriptorPtr((PrimitiveType*)Decoder::decode(frameBuffer));
            switch (descriptorPtr->type()) {
            case amqpPrimitiveType_t::ULONG_TYPE: {
                AmqpUlong* longDescriptorPtr = (AmqpUlong*)descriptorPtr.get(); // TODO: Ugly! Fix this! Works ok, descriptorPtr will delete when it goes out of scope.
                switch((amqpCompositeType_t)longDescriptorPtr->value()) {
                case amqpCompositeType_t::ERROR:
                    return new AmqpErrorRecord(Decoder::decodeFieldList(frameBuffer, AmqpErrorRecord::s_fieldTypeList), fieldName);
                case amqpCompositeType_t::RECEIVED:
                    return new AmqpReceived(Decoder::decodeFieldList(frameBuffer, AmqpReceived::s_fieldTypeList), fieldName);
                case amqpCompositeType_t::ACCEPTED:
                    return new AmqpAccepted(Decoder::decodeFieldList(frameBuffer, AmqpAccepted::s_fieldTypeList), fieldName);
                case amqpCompositeType_t::REJECTED:
                    return new AmqpRejected(Decoder::decodeFieldList(frameBuffer, AmqpRejected::s_fieldTypeList), fieldName);
                case amqpCompositeType_t::RELEASED:
                    return new AmqpReleased(Decoder::decodeFieldList(frameBuffer, AmqpReleased::s_fieldTypeList), fieldName);
                case amqpCompositeType_t::MODIFIED:
                    return new AmqpModified(Decoder::decodeFieldList(frameBuffer, AmqpModified::s_fieldTypeList), fieldName);
                case amqpCompositeType_t::SOURCE:
                    return new AmqpSource(Decoder::decodeFieldList(frameBuffer, AmqpSource::s_fieldTypeList), fieldName);
                case amqpCompositeType_t::TARGET:
                    return new AmqpTarget(Decoder::decodeFieldList(frameBuffer, AmqpTarget::s_fieldTypeList), fieldName);
                case amqpCompositeType_t::DELETE_ON_CLOSE:
                    return new AmqpDeleteOnClose(Decoder::decodeFieldList(frameBuffer, AmqpDeleteOnClose::s_fieldTypeList), fieldName);
                case amqpCompositeType_t::DELETE_ON_NO_LINKS:
                    return new AmqpDeleteOnNoLinks(Decoder::decodeFieldList(frameBuffer, AmqpDeleteOnNoLinks::s_fieldTypeList), fieldName);
                case amqpCompositeType_t::DELETE_ON_NO_MESSAGES:
                    return new AmqpDeleteOnNoMessages(Decoder::decodeFieldList(frameBuffer, AmqpDeleteOnNoMessages::s_fieldTypeList), fieldName);
                case amqpCompositeType_t::DELETE_ON_NO_LINKS_OR_MESSAGES:
                    return new AmqpDeleteOnNoLinksOrMessages(Decoder::decodeFieldList(frameBuffer, AmqpDeleteOnNoLinksOrMessages::s_fieldTypeList), fieldName);
                case amqpCompositeType_t::COORDINATOR:
                    return new AmqpCoordinator(Decoder::decodeFieldList(frameBuffer, AmqpCoordinator::s_fieldTypeList), fieldName);
                case amqpCompositeType_t::DECLARE:
                    return new AmqpDeclare(Decoder::decodeFieldList(frameBuffer, AmqpDeclare::s_fieldTypeList), fieldName);
                case amqpCompositeType_t::DISCHARGE:
                    return new AmqpDischarge(Decoder::decodeFieldList(frameBuffer, AmqpDischarge::s_fieldTypeList), fieldName);
                case amqpCompositeType_t::DECLARED:
                    return new AmqpDeclared(Decoder::decodeFieldList(frameBuffer, AmqpDeclared::s_fieldTypeList), fieldName);
                case amqpCompositeType_t::TRANSACTIONAL_STATE:
                    return new AmqpTransactionalState(Decoder::decodeFieldList(frameBuffer, AmqpTransactionalState::s_fieldTypeList), fieldName);
                case amqpCompositeType_t::SASL_MECHANISMS:
                    return new AmqpSaslMechanisms(Decoder::decodeFieldList(frameBuffer, AmqpSaslMechanisms::s_fieldTypeList), fieldName);
                case amqpCompositeType_t::SASL_INIT:
                    return new AmqpSaslInit(Decoder::decodeFieldList(frameBuffer, AmqpSaslInit::s_fieldTypeList), fieldName);
                case amqpCompositeType_t::SASL_CHALLENGE:
                    return new AmqpSaslChallenge(Decoder::decodeFieldList(frameBuffer, AmqpSaslChallenge::s_fieldTypeList), fieldName);
                case amqpCompositeType_t::SASL_RESPONSE:
                    return new AmqpSaslResponse(Decoder::decodeFieldList(frameBuffer, AmqpSaslResponse::s_fieldTypeList), fieldName);
                case amqpCompositeType_t::SASL_OUTCOME:
                    return new AmqpSaslOutcome(Decoder::decodeFieldList(frameBuffer, AmqpSaslOutcome::s_fieldTypeList), fieldName);
                default:
                    throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "Invalid AMQP composite type code " << longDescriptorPtr->valueStr()));
                }}
            case amqpPrimitiveType_t::SYMBOL_TYPE:
                throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "AMQP symbol descriptors not handled (" << descriptorPtr->typeStr() << ")"));
            default:
                throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "Invalid AMQP composite type descriptor " << descriptorPtr->typeStr()));
            }
        }

        // static
        Performative* Decoder::decodePerformative(FrameBuffer& frameBuffer) {
            frameBuffer.setFrameOffsetSnapshot();
            const uint8_t lfb = frameBuffer.getUint8();
            if (lfb != 0) {
                throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "Unexpected leading frame byte: expected 0x0, found 0x" << (int)lfb));
            }
            Performative* performativePtr = nullptr;
            std::unique_ptr<PrimitiveType> descriptorPtr((PrimitiveType*)Decoder::decode(frameBuffer));
            switch (descriptorPtr->type()) {
                case amqpPrimitiveType_t::ULONG_TYPE: {
                    AmqpUlong* longDescriptorPtr = (AmqpUlong*)descriptorPtr.get();
                    switch((performativeType_t)longDescriptorPtr->value()) {
                        case performativeType_t::OPEN:
                            performativePtr = new AmqpOpen(frameBuffer.getFrameOffsetSnapshot(),
                                                           decodeFieldList(frameBuffer, AmqpOpen::s_fieldTypeList));
                            break;
                        case performativeType_t::BEGIN:
                            performativePtr = new AmqpBegin(frameBuffer.getFrameOffsetSnapshot(),
                                                            decodeFieldList(frameBuffer, AmqpBegin::s_fieldTypeList));
                            break;
                        case performativeType_t::ATTACH:
                            performativePtr = new AmqpAttach(frameBuffer.getFrameOffsetSnapshot(),
                                                             decodeFieldList(frameBuffer, AmqpAttach::s_fieldTypeList));
                            break;
                        case performativeType_t::FLOW:
                            performativePtr = new AmqpFlow(frameBuffer.getFrameOffsetSnapshot(),
                                                           decodeFieldList(frameBuffer, AmqpFlow::s_fieldTypeList));
                            break;
                        case performativeType_t::TRANSFER:
                            performativePtr = new AmqpTransfer(frameBuffer.getFrameOffsetSnapshot(),
                                                               decodeFieldList(frameBuffer, AmqpTransfer::s_fieldTypeList));
                            break;
                        case performativeType_t::DISPOSITION:
                            performativePtr = new AmqpDisposition(frameBuffer.getFrameOffsetSnapshot(),
                                                                  decodeFieldList(frameBuffer, AmqpDisposition::s_fieldTypeList));
                            break;
                        case performativeType_t::DETACH:
                            performativePtr = new AmqpDetach(frameBuffer.getFrameOffsetSnapshot(),
                                                             decodeFieldList(frameBuffer, AmqpDetach::s_fieldTypeList));
                            break;
                        case performativeType_t::END:
                            performativePtr = new AmqpEnd(frameBuffer.getFrameOffsetSnapshot(),
                                                          decodeFieldList(frameBuffer, AmqpEnd::s_fieldTypeList));
                            break;
                        case performativeType_t::CLOSE:
                            performativePtr = new AmqpClose(frameBuffer.getFrameOffsetSnapshot(),
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
            return performativePtr;
        }


        // static
        AmqpList* Decoder::decodeFieldList(FrameBuffer& frameBuffer, fieldTypeList_t& fieldTypeList) {
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
        AmqpList* Decoder::decodeFieldList(std::size_t size, std::size_t count, FrameBuffer& frameBuffer, fieldTypeList_t& fieldTypeList) {
            if (count > fieldTypeList.size()) {
                throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "FieldList too large for fieldTypeList: FieldList size="
                                         << count << "; fieldTypeList size=" << fieldTypeList.size()));
            }
            const size_t startOffs = frameBuffer.getOffset();
            AmqpList* parameterListPtr = new AmqpList();
            for (std::size_t i=0; i<count; ++i) {
                parameterListPtr->value().push_back(decodeField(frameBuffer, fieldTypeList[i]));
            }
            if (frameBuffer.getOffset() - startOffs != size) {
                throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "List size mismatch: expected 0x" << std::hex
                                         << size << ", found 0x" << (frameBuffer.getOffset() - startOffs)));
            }
            return parameterListPtr;
        }


        // static
        Type* Decoder::decodeField(FrameBuffer& frameBuffer, const FieldType& fieldType) {
            uint8_t code = frameBuffer.getUint8();
            if (code == 0x40) {
                if (fieldType._mandatoryFlag) throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "Null found in mandatory field"));
                return new AmqpNull(fieldType._fieldName);
            }
            switch (fieldType._unionType) {
                case FieldType::type::PRIMITIVE:
                    switch (fieldType._types._primitiveType) {
                        case amqpPrimitiveType_t::MILLISECONDS_TYPE:
                            switch (code) {
                                case 0x43: return new AmqpMilliseconds(0, fieldType._fieldName);
                                case 0x52: return new AmqpMilliseconds(frameBuffer.getUint8(), fieldType._fieldName);
                                case 0x70: return new AmqpMilliseconds(frameBuffer.getUint32(), fieldType._fieldName);
                                default: throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "Invalid type code for \"milliseconds\" field: 0x"
                                                                  << std::hex << (int)code));
                            }
                        case amqpPrimitiveType_t::LANGUAGE_TAG_TYPE:
                            switch (code) {
                                case 0xa3: {
                                    AmqpLanguageTag* lt = new AmqpLanguageTag(fieldType._fieldName);
                                    frameBuffer.getSymbol(lt->value(), frameBuffer.getUint8());
                                    return lt;
                                }
                                case 0xb3: {
                                    AmqpLanguageTag* lt = new AmqpLanguageTag(fieldType._fieldName);
                                    frameBuffer.getSymbol(lt->value(), frameBuffer.getUint32());
                                    return lt;
                                }
                                default: throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "Invalid type code for \"ietf-language-tag\" field: 0x"
                                                                  << std::hex << (int)code));
                            }
                        case amqpPrimitiveType_t::FIELDS_TYPE:
                                switch (code) {
                                case 0xc1: {
                                    AmqpFields* f = new AmqpFields(fieldType._fieldName);
                                    const uint8_t size(frameBuffer.getUint8());
                                    const uint8_t count(frameBuffer.getUint8());
                                    frameBuffer.getMap(f->value(), size - sizeof(count), count);
                                    return f;
                                }
                                case 0xd1: {
                                    AmqpFields* f = new AmqpFields(fieldType._fieldName);
                                    const uint32_t size(frameBuffer.getUint32());
                                    const uint32_t count(frameBuffer.getUint32());
                                    frameBuffer.getMap(f->value(), size - sizeof(count), count);
                                    return f;
                                }
                                default: throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "Invalid type code for \"fields\" field: 0x" << std::hex << (int)code));
                            }
                        case amqpPrimitiveType_t::TRANSFER_NUMBER_TYPE:
                            switch (code) {
                                case 0x70: return new AmqpTransferNum(frameBuffer.getUint32(), fieldType._fieldName);
                                case 0x52: return new AmqpTransferNum(frameBuffer.getUint8(), fieldType._fieldName);
                                case 0x43: return new AmqpTransferNum(0, fieldType._fieldName);
                                default: throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "Invalid type code for \"transfer-number\" field: 0x" << std::hex << (int)code));
                            }
                        case amqpPrimitiveType_t::HANDLE_TYPE:
                            switch (code) {
                                case 0x70: return new AmqpHandle(frameBuffer.getUint32(), fieldType._fieldName);
                                case 0x52: return new AmqpHandle(frameBuffer.getUint8(), fieldType._fieldName);
                                case 0x43: return new AmqpHandle(0, fieldType._fieldName);
                                default: throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "Invalid type code for \"handle\" field: 0x" << std::hex << (int)code));
                            }
                        case amqpPrimitiveType_t::ROLE_TYPE:
                            switch (code) {
                                case 0x41: return new AmqpRole((amqp_role_t)true, fieldType._fieldName);
                                case 0x42: return new AmqpRole((amqp_role_t)false, fieldType._fieldName);
                                case 0x56: return new AmqpRole((amqp_role_t)frameBuffer.getBool(), fieldType._fieldName);
                                default: throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "Invalid type code for \"role\" field: 0x" << std::hex << (int)code));
                            }
                        case amqpPrimitiveType_t::SENDER_SETTLE_MODE_TYPE:
                            switch (code) {
                                case 0x50: return new AmqpSenderSettleMode((amqp_sender_settle_mode_t)frameBuffer.getUint8(), fieldType._fieldName);
                                default: throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "Invalid type code for \"sender-settle-mode\" field: 0x" << std::hex << (int)code));
                            }
                        case amqpPrimitiveType_t::RECEIVER_SETTLE_MODE_TYPE:
                            switch (code) {
                                case 0x50: return new AmqpReceiverSettleMode((amqp_receiver_settle_mode_t)frameBuffer.getUint8(), fieldType._fieldName);
                                default: throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "Invalid type code for \"receiver-settle-mode\" field: 0x" << std::hex << (int)code));
                            }
                        case amqpPrimitiveType_t::SEQUENCE_NUMBER_TYPE:
                            switch (code) {
                                case 0x70: return new AmqpSequenceNum(frameBuffer.getUint32(), fieldType._fieldName);
                                case 0x52: return new AmqpSequenceNum(frameBuffer.getUint8(), fieldType._fieldName);
                                case 0x43: return new AmqpSequenceNum(0, fieldType._fieldName);
                                default: throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "Invalid type code for \"sequence-no\" field: 0x" << std::hex << (int)code));
                            }
                        case amqpPrimitiveType_t::DELIVERY_NUMBER_TYPE:
                            switch (code) {
                                case 0x70: return new AmqpDeliveryNum(frameBuffer.getUint32(), fieldType._fieldName);
                                case 0x52: return new AmqpDeliveryNum(frameBuffer.getUint8(), fieldType._fieldName);
                                case 0x43: return new AmqpDeliveryNum(0, fieldType._fieldName);
                                default: throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "Invalid type code for \"delivery-number\" field: 0x" << std::hex << (int)code));
                            }
                        case amqpPrimitiveType_t::DELIVERY_TAG_TYPE:
                            switch (code) {
                                case 0xa0: {
                                    AmqpDeliveryTag* d = new AmqpDeliveryTag(fieldType._fieldName);
                                    frameBuffer.getBinary(d->value(), frameBuffer.getUint8());
                                    return d;
                                }
                                case 0xb0: {
                                    AmqpDeliveryTag* d = new AmqpDeliveryTag(fieldType._fieldName);
                                    frameBuffer.getBinary(d->value(), frameBuffer.getUint32());
                                    return d;
                                }
                                default: throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "Invalid type code for \"delivery-tag\" field: 0x" << std::hex << (int)code));
                            }
                        case amqpPrimitiveType_t::MESSAGE_FORMAT_TYPE:
                            switch (code) {
                                case 0x70: return new AmqpUint(frameBuffer.getUint32(), fieldType._fieldName);
                                case 0x52: return new AmqpUint(frameBuffer.getUint8(), fieldType._fieldName);
                                case 0x43: return new AmqpUint(0, fieldType._fieldName);
                                default: throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "Invalid type code for \"message-format\" field: 0x" << std::hex << (int)code));
                            }
                        default:
                            return Decoder::decodePrimitive(code, frameBuffer, fieldType._fieldName);
                    }
                case FieldType::type::WILDCARD: {
                    Type* typePtr = nullptr;
                    if (code > 0) {
                        typePtr = Decoder::decodePrimitive(code, frameBuffer, fieldType._fieldName);
                    } else {
                        typePtr = Decoder::decodeComposite(frameBuffer, fieldType._fieldName);
                    }

                    // Check that field requires (if present) matches provides in decoded type
                    if (fieldType._requiresList.size() == 1) {
                        amqpRequiresProvides_t requires = fieldType._requiresList.front();
                        if (!typePtr->provides(requires, typePtr->providesList())) {
                            // throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "Provides - requires mismatch: requires=\"" << ProvidesRequiresNames[requires] << "\""));
                            std::cout << frameBuffer.getErrorPrefix() << "Provides - requires mismatch: requires=\"" << ProvidesRequiresNames[requires] << "\"" << std::endl;
                        }
                    } else {
                        throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "fieldType._requiresList.size()!=1: " << fieldType._requiresList.size()));
                    }
                    return typePtr;
                }
                default:
                    throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "Unknown field type: 0x" << std::hex << (int)fieldType._unionType));
            }
        }

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */
