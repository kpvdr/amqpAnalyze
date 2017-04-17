/*
 * AmqpTypes.cpp
 *
 *  Created on: Apr 5, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/amqp10/Type.hpp>

#include <amqpAnalyze/amqp10/FieldType.hpp>
#include <amqpAnalyze/amqp10/FrameBuffer.hpp>
#include <amqpAnalyze/Error.hpp>

#include <ctime>
#include <iomanip>

namespace amqpAnalyze
{
    namespace amqp10
    {
        Type::Type() {}
        Type::~Type() {}
        std::string Type::typeValueStr(const char* valueDelim) const {
            std::ostringstream oss;
            oss << typeStr() << valueDelim[0] << valueStr() << valueDelim[1];
            return oss.str();
        }
        // static
        Type* Type::decode(FrameBuffer& frameBuffer) {
            uint8_t code = frameBuffer.getUint8();
            if (code > 0) return Type::decodePrimitive(code, frameBuffer);
            return Type::decodeComposite(frameBuffer);
        }
        // static
        PrimitiveType* Type::decodePrimitive(uint8_t code, FrameBuffer& frameBuffer) {
            switch(code) {

            // null
            case 0x40: return new AmqpNull();

            // boolean
            case 0x41: return new AmqpBoolean(true);
            case 0x42: return new AmqpBoolean(false);
            case 0x56: return new AmqpBoolean(frameBuffer.getBool());

            // ubyte
            case 0x50: return new AmqpUbyte(frameBuffer.getUint8());

            // ushort
            case 0x60: return new AmqpUshort(frameBuffer.getUint16());

            // uint
            case 0x70: return new AmqpUint(frameBuffer.getUint32());
            case 0x52: return new AmqpUint(frameBuffer.getUint8());
            case 0x43: return new AmqpUint(0);

            // ulong
            case 0x80: return new AmqpUlong(frameBuffer.getUint64());
            case 0x53: return new AmqpUlong(frameBuffer.getUint8());
            case 0x44: return new AmqpUlong(0);

            // byte
            case 0x51: return new AmqpByte(frameBuffer.getInt8());

            // short
            case 0x61: return new AmqpShort(frameBuffer.getInt16());

            // int
            case 0x71: return new AmqpInt(frameBuffer.getInt32());
            case 0x54: return new AmqpInt(frameBuffer.getInt8());

            // long
            case 0x81: return new AmqpLong(frameBuffer.getInt64());
            case 0x55: return new AmqpLong(frameBuffer.getInt8());

            // float
            case 0x72: return new AmqpFloat(frameBuffer.getFloat());

            // double
            case 0x82: return new AmqpDouble(frameBuffer.getDouble());

            // decimal32
            case 0x74: {
                AmqpDecimal32* d = new AmqpDecimal32();
                frameBuffer.getDecimal32(d->value());
                return d;
            }

            // decimal64
            case 0x84: {
                AmqpDecimal64* d = new AmqpDecimal64();
                frameBuffer.getDecimal64(d->value());
                return d;
            }

            // decimal128
            case 0x94: {
                AmqpDecimal128* d = new AmqpDecimal128();
                frameBuffer.getDecimal128(d->value());
                return d;
            }

            // char
            case 0x73: return new AmqpChar(frameBuffer.getChar());

            // timestamp
            case 0x83: return new AmqpTimestamp(frameBuffer.getUint64());

            // uuid
            case 0x98: {
                AmqpUuid* u = new AmqpUuid();
                frameBuffer.getUuid(u->value());
                return u;
            }

            // binary
            case 0xa0: {
                AmqpBinary* d = new AmqpBinary();
                frameBuffer.getBinary(d->value(), frameBuffer.getUint8());
                return d;
            }
            case 0xb0: {
                AmqpBinary* d = new AmqpBinary();
                frameBuffer.getBinary(d->value(), frameBuffer.getUint32());
                return d;
            }

            // string
            case 0xa1: {
                AmqpString* s = new AmqpString();
                frameBuffer.getString(s->value(), frameBuffer.getUint8());
                return s;
            }
            case 0xb1: {
                AmqpString* s = new AmqpString();
                frameBuffer.getString(s->value(), frameBuffer.getUint32());
                return s;
            }

            // symbol
            case 0xa3: {
                AmqpSymbol* s = new AmqpSymbol();
                frameBuffer.getSymbol(s->value(), frameBuffer.getUint8());
                return s;
            }
            case 0xb3: {
                AmqpSymbol* s = new AmqpSymbol();
                frameBuffer.getSymbol(s->value(), frameBuffer.getUint32());
                return s;
            }

            // list
            case 0x45: return new AmqpList();
            case 0xc0: {
                AmqpList* l = new AmqpList();
                const uint8_t size(frameBuffer.getUint8());
                const uint8_t count(frameBuffer.getUint8());
                frameBuffer.getList(l->value(), size - sizeof(count), count);
                return l;
            }
            case 0xd0: {
                AmqpList* l = new AmqpList();
                const uint32_t size(frameBuffer.getUint32());
                const uint32_t count(frameBuffer.getUint32());
                frameBuffer.getList(l->value(), size - sizeof(count), count);
                return l;
            }

            // map
            case 0xc1: {
                AmqpMap* m = new AmqpMap();
                const uint8_t size(frameBuffer.getUint8());
                const uint8_t count(frameBuffer.getUint8());
                frameBuffer.getMap(m->value(), size - sizeof(count), count);
                return m;
            }
            case 0xd1: {
                AmqpMap* m = new AmqpMap();
                const uint32_t size(frameBuffer.getUint32());
                const uint32_t count(frameBuffer.getUint32());
                frameBuffer.getMap(m->value(), size - sizeof(count), count);
                return m;
            }

            // array
            case 0xe0: {
                AmqpArray* a = new AmqpArray();
                const uint8_t size(frameBuffer.getUint8());
                const uint8_t count(frameBuffer.getUint8());
                frameBuffer.getArray(a->value(), size - sizeof(count), count);
                return a;
            }
            case 0xf0: {
                AmqpArray* a = new AmqpArray();
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
        CompositeType* Type::decodeComposite(FrameBuffer& frameBuffer) {
            std::unique_ptr<PrimitiveType> descriptorPtr((PrimitiveType*)Type::decode(frameBuffer));
            switch (descriptorPtr->type()) {
            case amqpPrimitiveType_t::ULONG_TYPE: {
                AmqpUlong* longDescriptorPtr = (AmqpUlong*)descriptorPtr.get(); // TODO: Ugly! Fix this! Works ok, descriptorPtr will delete when it goes out of scope.
                switch((amqpCompositeType_t)longDescriptorPtr->value()) {
                case amqpCompositeType_t::ERROR:
                    return new AmqpErrorRecord((AmqpList*)Type::decode(frameBuffer));
                case amqpCompositeType_t::RECEIVED:
                    return new AmqpReceived((AmqpList*)Type::decode(frameBuffer));
                case amqpCompositeType_t::ACCEPTED:
                    return new AmqpAccepted((AmqpList*)Type::decode(frameBuffer));
                case amqpCompositeType_t::REJECTED:
                    return new AmqpRejected((AmqpList*)Type::decode(frameBuffer));
                case amqpCompositeType_t::RELEASED:
                    return new AmqpReleased((AmqpList*)Type::decode(frameBuffer));
                case amqpCompositeType_t::MODIFIED:
                    return new AmqpModified((AmqpList*)Type::decode(frameBuffer));
                case amqpCompositeType_t::SOURCE:
                    return new AmqpSource((AmqpList*)Type::decode(frameBuffer));
                case amqpCompositeType_t::TARGET:
                    return new AmqpTarget((AmqpList*)Type::decode(frameBuffer));
                case amqpCompositeType_t::DELETE_ON_CLOSE:
                    return new AmqpDeleteOnClose((AmqpList*)Type::decode(frameBuffer));
                case amqpCompositeType_t::DELETE_ON_NO_LINKS:
                    return new AmqpDeleteOnNoLinks((AmqpList*)Type::decode(frameBuffer));
                case amqpCompositeType_t::DELETE_ON_NO_MESSAGES:
                    return new AmqpDeleteOnNoMessages((AmqpList*)Type::decode(frameBuffer));
                case amqpCompositeType_t::DELETE_ON_NO_LINKS_OR_MESSAGES:
                    return new AmqpDeleteOnNoLinksOrMessages((AmqpList*)Type::decode(frameBuffer));
                case amqpCompositeType_t::COORDINATOR:
                    return new AmqpCoordinator((AmqpList*)Type::decode(frameBuffer));
                case amqpCompositeType_t::DECLARE:
                    return new AmqpDeclare((AmqpList*)Type::decode(frameBuffer));
                case amqpCompositeType_t::DISCHARGE:
                    return new AmqpDischarge((AmqpList*)Type::decode(frameBuffer));
                case amqpCompositeType_t::DECLARED:
                    return new AmqpDeclared((AmqpList*)Type::decode(frameBuffer));
                case amqpCompositeType_t::TRANSACTIONAL_STATE:
                    return new AmqpTransactionalState((AmqpList*)Type::decode(frameBuffer));
                case amqpCompositeType_t::SASL_MECHANISMS:
                    return new AmqpSaslMechanisms((AmqpList*)Type::decode(frameBuffer));
                case amqpCompositeType_t::SASL_INIT:
                    return new AmqpSaslInit((AmqpList*)Type::decode(frameBuffer));
                case amqpCompositeType_t::SASL_CHALLENGE:
                    return new AmqpSaslChallenge((AmqpList*)Type::decode(frameBuffer));
                case amqpCompositeType_t::SASL_RESPONSE:
                    return new AmqpSaslResponse((AmqpList*)Type::decode(frameBuffer));
                case amqpCompositeType_t::SASL_OUTCOME:
                    return new AmqpSaslOutcome((AmqpList*)Type::decode(frameBuffer));
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
        bool Type::provides(amqpRequiresProvides_t requires, const amqp_provides_requires_list_t& prList) {
            bool found = false;
            for (amqp_provides_requires_list_citr_t i = prList.cbegin(); i!=prList.cend(); ++i) {
                if (*i == requires) {
                    found = true;
                    break;
                }
            }
            return found;
        }
       // static
        std::string Type::providesListAsString(const amqp_provides_requires_list_t& prList) {
            std::ostringstream oss;
            oss << "[";
            for (amqp_provides_requires_list_citr_t i=prList.cbegin(); i!=prList.cend(); ++i) {
                if (i != prList.cbegin()) oss << ", ";
                oss << ProvidesRequiresNames[*i];
            }
            oss << "]";
            return oss.str();
        }


        PrimitiveType::PrimitiveType(): Type() {}
        PrimitiveType::~PrimitiveType() {}
        // static
        amqp_provides_requires_list_t PrimitiveType::s_providesList = {};
        // static
        std::map<amqpPrimitiveType_t, const char*> PrimitiveType::s_amqpSimpleTypeNames = {
            // Primitive types
            {amqpPrimitiveType_t::NULL_TYPE, "null"},
            {amqpPrimitiveType_t::BOOLEAN_TYPE, "boolean"},
            {amqpPrimitiveType_t::UBYTE_TYPE, "ubyte"},
            {amqpPrimitiveType_t::USHORT_TYPE, "ushort"},
            {amqpPrimitiveType_t::UINT_TYPE, "uint"},
            {amqpPrimitiveType_t::ULONG_TYPE, "ulong"},
            {amqpPrimitiveType_t::BYTE_TYPE, "byte"},
            {amqpPrimitiveType_t::SHORT_TYPE, "short"},
            {amqpPrimitiveType_t::INT_TYPE, "int"},
            {amqpPrimitiveType_t::LONG_TYPE, "long"},
            {amqpPrimitiveType_t::FLOAT_TYPE, "float"},
            {amqpPrimitiveType_t::DOUBLE_TYPE, "double"},
            {amqpPrimitiveType_t::DECIMAL32_TYPE, "decimal32"},
            {amqpPrimitiveType_t::DECIMAL64_TYPE, "decimal64"},
            {amqpPrimitiveType_t::DECIMAL128_TYPE, "decimal128"},
            {amqpPrimitiveType_t::CHAR_TYPE, "char"},
            {amqpPrimitiveType_t::TIMESTAMP_TYPE, "timestamp"},
            {amqpPrimitiveType_t::UUID_TYPE, "uuid"},
            {amqpPrimitiveType_t::BINARY_TYPE, "binary"},
            {amqpPrimitiveType_t::STRING_TYPE, "string"},
            {amqpPrimitiveType_t::SYMBOL_TYPE, "symbol"},
            {amqpPrimitiveType_t::LIST_TYPE, "list"},
            {amqpPrimitiveType_t::MAP_TYPE, "map"},
            {amqpPrimitiveType_t::ARRAY_TYPE, "array"},
            // Restricted types
            {amqpPrimitiveType_t::ROLE_TYPE, "role"},
            {amqpPrimitiveType_t::SENDER_SETTLE_MODE_TYPE, "sender-settle-mode"},
            {amqpPrimitiveType_t::RECEIVER_SETTLE_MODE_TYPE, "reciever-settle-mode"},
            {amqpPrimitiveType_t::HANDLE_TYPE, "handle"},
            {amqpPrimitiveType_t::SECONDS_TYPE, "seconds"},
            {amqpPrimitiveType_t::MILLISECONDS_TYPE, "milliseconds"},
            {amqpPrimitiveType_t::DELIVERY_TAG_TYPE, "delivery-tag"},
            {amqpPrimitiveType_t::DELIVERY_NUMBER_TYPE, "delivery-number"},
            {amqpPrimitiveType_t::TRANSFER_NUMBER_TYPE, "transfer-number"},
            {amqpPrimitiveType_t::SEQUENCE_NUMBER_TYPE, "sequence-number"},
            {amqpPrimitiveType_t::MESSAGE_FORMAT_TYPE, "message-format"},
            {amqpPrimitiveType_t::LANGUAGE_TAG_TYPE, "language-ag"},
            {amqpPrimitiveType_t::FIELDS_TYPE, "fields"},
            {amqpPrimitiveType_t::AMQP_ERROR_TYPE, "amqp-error"},
            {amqpPrimitiveType_t::CONNECTION_ERROR_TYPE, "connection-error"},
            {amqpPrimitiveType_t::SESSION_ERROR_TYPE, "session-error"},
            {amqpPrimitiveType_t::LINK_ERROR_TYPE, "link-error"},
            {amqpPrimitiveType_t::ANNOTATIONS_TYPE, "annotations"},
            {amqpPrimitiveType_t::MESSAGE_ID_ULONG_TYPE, "message-id-ulong"},
            {amqpPrimitiveType_t::MESSAGE_ID_UUID_TYPE, "message-id-uuid"},
            {amqpPrimitiveType_t::MESSAGE_ID_BINARY_TYPE, "message-id-binary"},
            {amqpPrimitiveType_t::MESSAGE_ID_STRING_TYPE, "message-id-string"},
            {amqpPrimitiveType_t::ADDRESS_TYPE, "address-string"},
            {amqpPrimitiveType_t::TERMINUS_DURABILITY_TYPE, "terminus-durability"},
            {amqpPrimitiveType_t::TERMINUS_EXPIRY_POLICY_TYPE, "terminus-expiry-policy"},
            {amqpPrimitiveType_t::STD_DIST_MODE_TYPE, "std-dist-mode"},
            {amqpPrimitiveType_t::FILTER_SET_TYPE, "filter-set"},
            {amqpPrimitiveType_t::NODE_PROPERTIES_TYPE, "node-properties"},
            {amqpPrimitiveType_t::TRANSACTION_ID_TYPE, "transaction-id"},
            {amqpPrimitiveType_t::TRANSACTION_CAPABILITY_TYPE, "txn-capability"},
            {amqpPrimitiveType_t::TRANSACTION_ERROR_TYPE, "transaction-error"},
            {amqpPrimitiveType_t::SASL_CODE_TYPE, "sasl-code"}
        };


        AmqpNull::AmqpNull(): PrimitiveType() {}
        AmqpNull::~AmqpNull() {}
        std::string AmqpNull::valueStr() const {
            return "null";
        }


        AmqpBoolean::AmqpBoolean(bool v): PrimitiveType(), _value(v) {}
        AmqpBoolean::~AmqpBoolean() {}
        std::string AmqpBoolean::valueStr() const {
            return _value ? "true" : "false";
        }


        AmqpUbyte::AmqpUbyte(uint8_t v): PrimitiveType(), _value(v) {}
        AmqpUbyte::~AmqpUbyte() {}
        std::string AmqpUbyte::valueStr() const {
            std::stringstream oss;
            oss << "0x" << std::hex << (int)_value;
            return oss.str();
        }


        AmqpUshort::AmqpUshort(uint16_t v): PrimitiveType(), _value(v) {}
        AmqpUshort::~AmqpUshort() {}
        std::string AmqpUshort::valueStr() const {
            std::stringstream oss;
            oss << "0x" << std::hex << _value;
            return oss.str();
        }


        AmqpUint::AmqpUint(uint32_t v): PrimitiveType(), _value(v) {}
        AmqpUint::~AmqpUint() {}
        std::string AmqpUint::valueStr() const {
            std::ostringstream oss;
            oss<< "0x" << std::hex << _value;
            return oss.str();
        }


        AmqpUlong::AmqpUlong(uint64_t v): PrimitiveType(), _value(v) {}
        AmqpUlong::~AmqpUlong() {}
        std::string AmqpUlong::valueStr() const {
            std::ostringstream oss;
            oss << "0x" << std::hex << _value;
            return oss.str();
        }


        AmqpByte::AmqpByte(int8_t v): PrimitiveType(), _value(v) {}
        AmqpByte::~AmqpByte() {}
        std::string AmqpByte::valueStr() const {
            std::ostringstream oss;
            oss <<  "0x" << std::hex << (int)(uint8_t)_value;
            return oss.str();
        }


        AmqpShort::AmqpShort(int16_t v): PrimitiveType(), _value(v) {}
        AmqpShort::~AmqpShort() {}
        std::string AmqpShort::valueStr() const {
            std::ostringstream oss;
            oss << "0x" << std::hex << _value;
            return oss.str();
        }


        AmqpInt::AmqpInt(int32_t v): PrimitiveType(), _value(v) {}
        AmqpInt::~AmqpInt() {}
        std::string AmqpInt::valueStr() const {
            std::ostringstream oss;
            oss << "0x" << std::hex << _value;
            return oss.str();
        }


        AmqpLong::AmqpLong(int64_t v): PrimitiveType(), _value(v) {}
        AmqpLong::~AmqpLong() {}
        std::string AmqpLong::valueStr() const {
            std::ostringstream oss;
            oss << "0x" << std::hex << _value;
            return oss.str();
        }


        AmqpFloat::AmqpFloat(float v): PrimitiveType(), _value(v) {}
        AmqpFloat::~AmqpFloat() {}
        std::string AmqpFloat::valueStr() const {
            std::ostringstream oss;
            oss << _value;
            return oss.str();
        }


        AmqpDouble::AmqpDouble(double v): PrimitiveType(), _value(v) {}
        AmqpDouble::~AmqpDouble() {}
        std::string AmqpDouble::valueStr() const {
            std::ostringstream oss;
            oss << _value;
            return oss.str();
        }


        AmqpDecimal32::AmqpDecimal32(): PrimitiveType(), _value({0, 0, 0, 0}) {}
        AmqpDecimal32::~AmqpDecimal32() {}
        std::string AmqpDecimal32::valueStr() const {
            std::ostringstream oss;
            oss << std::hex << std::setfill('0');
            for (int i=0; i<4; ++i) {
                oss << (i>0 ? ", 0x" : "0x") << std::setw(2) << (int)_value[i];
            }
            return oss.str();
        }


        AmqpDecimal64::AmqpDecimal64(): PrimitiveType(), _value({0, 0, 0, 0, 0, 0, 0, 0}) {}
        AmqpDecimal64::~AmqpDecimal64() {}
        std::string AmqpDecimal64::valueStr() const {
            std::ostringstream oss;
            oss << std::hex << std::setfill('0');
            for (int i=0; i<8; ++i) {
                oss << (i>0 ? ", 0x" : "0x") << std::setw(2) << (int)_value[i];
            }
            return oss.str();
        }


        AmqpDecimal128::AmqpDecimal128(): PrimitiveType(), _value({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}) {}
        AmqpDecimal128::~AmqpDecimal128() {}
        std::string AmqpDecimal128::valueStr() const {
            std::ostringstream oss;
            oss << std::hex << std::setfill('0');
            for (int i=0; i<16; ++i) {
                oss << (i>0 ? ", 0x" : "0x") << std::setw(2) << (int)_value[i];
            }
            return oss.str();
        }


        AmqpChar::AmqpChar(char32_t v): PrimitiveType(), _value(v) {}
        AmqpChar::~AmqpChar() {}
        std::string AmqpChar::valueStr() const {
            std::ostringstream oss;
            oss << "0x" << std::hex << (int)_value << "='" << _value << "'";
            return oss.str();
        }


        AmqpTimestamp::AmqpTimestamp(uint64_t v): PrimitiveType(), _value(v) {}
        AmqpTimestamp::~AmqpTimestamp() {}
        std::string AmqpTimestamp::valueStr() const {
            std::ostringstream oss;
            std::time_t t = (std::time_t)(_value/1000);
            char buf[20];
            std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));
            char tzbuf[6];
            std::strftime(tzbuf, sizeof(tzbuf), "%z", std::localtime(&t));
            oss << "0x" << std::hex << _value;
            if (_value > 0) oss << "=" << buf << "." << std::setfill('0') << std::setw(3) << (_value%1000) << " " << tzbuf;
            return oss.str();
        }


        AmqpUuid::AmqpUuid(): PrimitiveType(), _value({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}) {}
        AmqpUuid::~AmqpUuid() {}
        std::string AmqpUuid::valueStr() const {
            std::ostringstream oss;
            oss << std::hex << std::setfill('0');
            for (int i=0; i<16; ++i) {
                if (i==4 || i==6 || i==8 || i==10) oss << '-';
                oss << std::setw(2) << (int)_value[i];
            }
            return oss.str();
        }


        AmqpBinary::AmqpBinary(): PrimitiveType(), _value() {}
        AmqpBinary::~AmqpBinary() {}
        std::string AmqpBinary::valueStr() const {
            std::ostringstream oss;
            oss << std::hex << std::setfill('0');
            for (int i=0; i<_value.size(); ++i) {
                if (i>0) oss << ' ';
                oss << std::setw(2) << (int)_value.at(i);
            }
            return oss.str();
        }


        AmqpString::AmqpString(): PrimitiveType(), _value() {}
        AmqpString::~AmqpString() {}
        std::string AmqpString::valueStr() const {
            std::stringstream oss;
            oss << "\"" << _value << "\"";
            return oss.str();
        }


        AmqpSymbol::AmqpSymbol(): PrimitiveType(), _value() {}
        AmqpSymbol::~AmqpSymbol() {}
        std::string AmqpSymbol::valueStr() const {
            std::stringstream oss;
            oss << "\"" << _value << "\"";
            return oss.str();
        }


        CompoundType::CompoundType(): PrimitiveType() {}
        CompoundType::~CompoundType() {}
        // static
        void CompoundType::stringAppendHandler(std::ostringstream& oss, Type* ptr, std::size_t margin) {
            CompoundType* cPtr(dynamic_cast<CompoundType*>(ptr));
            if (cPtr) {
                cPtr->appendString(oss, margin);
            } else {
                CompositeType* compositePtr(dynamic_cast<CompositeType*>(ptr));
                if (compositePtr) {
                    oss << compositePtr->toString(margin + 2);
                } else {
                    oss<< ptr->typeValueStr();
                }
            }
        }


        AmqpList::AmqpList(): CompoundType(), _value() {}
        AmqpList::~AmqpList() {
            for (amqp_list_itr_t i=_value.begin(); i!=_value.end(); ++i) {
                delete *i;
            }
            _value.clear();
        }
        std::string AmqpList::valueStr() const {
            std::ostringstream oss;
            for (amqp_list_citr_t i=_value.cbegin(); i!=_value.cend(); ++i) {
                if (i!=_value.cbegin()) oss << ", ";
                oss << (*i)->typeValueStr();
            }
            return oss.str();
        }
        void AmqpList::appendString(std::ostringstream& oss, std::size_t margin) const {
            std::string t(typeStr());
            std::size_t l(margin + t.length() + 3);
            std::string m(l, ' ');
            oss << t << ": [";
            for (amqp_list_citr_t i=_value.cbegin(); i<_value.cend(); ++i) {
                if (i!=_value.cbegin()) oss << std::endl << m;
                stringAppendHandler(oss, *i, l);
            }
            oss << "]";

        }


        AmqpMap::AmqpMap(): CompoundType(), _value() {}
        AmqpMap::~AmqpMap() {
            while (!_value.empty()) {
                amqp_map_itr_t i = _value.begin();
                Type* k = i->first;
                Type* v = i->second;
                _value.erase(i);
                delete k;
                delete v;
            }
        }
        std::string AmqpMap::valueStr() const {
            std::ostringstream oss;
            for (amqp_map_citr_t i=_value.cbegin(); i!=_value.cend(); ++i) {
                if (i!=_value.cbegin()) oss << ", ";
                oss << "{" << i->first->typeValueStr() << ": " << i->second->typeValueStr() << "}";
            }
            return oss.str();
        }
        void AmqpMap::appendString(std::ostringstream& oss, std::size_t margin) const {
            std::string t(typeStr());
            std::size_t l(margin + t.length() + 3);
            std::string m(l, ' ');
            oss << t << ": {";
            for (amqp_map_citr_t i=_value.cbegin(); i!=_value.cend(); ++i) {
                if (i!=_value.cbegin()) oss << std::endl << m;
                oss << "{";
                CompoundType* kPtr(dynamic_cast<CompoundType*>(i->first));
                if (kPtr != nullptr) {
                    // key is a compound type
                    kPtr->appendString(oss, l);
                    oss << std::endl << m << ":";
                } else {
                    std::string kStr(i->first->typeValueStr());
                    oss << kStr << ": ";
                    l += kStr.length() + 2;
                }
                stringAppendHandler(oss, i->second, l); // print value
                oss << "}";
            }
            oss << "}";
        }


        AmqpArray::AmqpArray(): CompoundType(), _value() {}
        AmqpArray::~AmqpArray() {
            for (amqp_array_itr_t i=_value.begin(); i!=_value.end(); ++i) {
                delete *i;
            }
            _value.clear();
        }
        std::string AmqpArray::valueStr() const {
            std::ostringstream oss;
            for (amqp_array_citr_t i=_value.cbegin(); i!=_value.cend(); ++i) {
                if (i != _value.cbegin()) oss << ", ";
                oss << (*i)->typeValueStr();
            }
            return oss.str();
        }
        void AmqpArray::appendString(std::ostringstream& oss, std::size_t margin) const {
            std::string t(typeStr());
            std::size_t l(margin + t.length() + 2);
            std::string m(l, ' ');
            oss << t << ": [";
            for (amqp_array_citr_t i=_value.cbegin(); i!=_value.cend(); ++i) {
                if (i!=_value.cbegin()) oss << std::endl << m;
                stringAppendHandler(oss, *i, l);
            }
            oss << "]";
        }


        // static
        std::map<amqp_role_t, const char*> AmqpRole::s_choiceNames = {
            {amqp_role_t::SENDER, "sender"},
            {amqp_role_t::RECEIVER, "receiver"}
        };
        AmqpRole::AmqpRole(amqp_role_t v): AmqpBoolean((amqp_boolean_t)v) {}
        AmqpRole:: ~AmqpRole() {}
        std::string AmqpRole::valueStr() const {
            return s_choiceNames.at((amqp_role_t)_value);
        }


        // static
        std::map<amqp_sender_settle_mode_t, const char*> AmqpSenderSettleMode::s_choiceNames = {
            {amqp_sender_settle_mode_t::UNSETTLED, "unsettled"},
            {amqp_sender_settle_mode_t::SETTLED, "settled"},
            {amqp_sender_settle_mode_t::MIXED, "mixed"}
        };
        AmqpSenderSettleMode::AmqpSenderSettleMode(amqp_sender_settle_mode_t v): AmqpUbyte((amqp_ubyte_t)v) {}
        AmqpSenderSettleMode::~AmqpSenderSettleMode() {}
        std::string AmqpSenderSettleMode::valueStr() const {
            return s_choiceNames.at((amqp_sender_settle_mode_t)_value);
        }


        // static
        std::map<amqp_receiver_settle_mode_t, const char*> AmqpReceiverSettleMode::s_choiceNames = {
            {amqp_receiver_settle_mode_t::FIRST, "first"},
            {amqp_receiver_settle_mode_t::SECOND, "second"}
        };
        AmqpReceiverSettleMode::AmqpReceiverSettleMode(amqp_receiver_settle_mode_t v): AmqpUbyte((amqp_ubyte_t)v) {}
        AmqpReceiverSettleMode::~AmqpReceiverSettleMode() {}
        std::string AmqpReceiverSettleMode::valueStr() const {
            return s_choiceNames.at((amqp_receiver_settle_mode_t)_value);
        }


        AmqpHandle::AmqpHandle(amqp_handle_t v): AmqpUint(v) {}
        AmqpHandle::~AmqpHandle() {}


        AmqpSeconds::AmqpSeconds(amqp_seconds_t v): AmqpUint(v) {}
        AmqpSeconds::~AmqpSeconds() {}


        AmqpMilliseconds::AmqpMilliseconds(amqp_milliseconds_t v): AmqpUint(v) {}
        AmqpMilliseconds::~AmqpMilliseconds() {}


        AmqpDeliveryTag::AmqpDeliveryTag(): AmqpBinary() {}
        AmqpDeliveryTag::~AmqpDeliveryTag() {}


        AmqpSequenceNum::AmqpSequenceNum(amqp_sequence_num_t v): AmqpUint(v) {}
        AmqpSequenceNum::~AmqpSequenceNum() {}


        AmqpDeliveryNum::AmqpDeliveryNum(amqp_delivery_num_t v): AmqpSequenceNum(v) {}
        AmqpDeliveryNum::~AmqpDeliveryNum() {}


        AmqpTransferNum::AmqpTransferNum(amqp_transfer_num_t v): AmqpSequenceNum(v) {}
        AmqpTransferNum::~AmqpTransferNum() {}


        AmqpMessageFormat::AmqpMessageFormat(amqp_message_format_t v): AmqpUint(v) {}
        AmqpMessageFormat::~AmqpMessageFormat() {}
        std::string AmqpMessageFormat::valueStr() const {
            std::ostringstream oss;
            oss << typeStr() << "fmt=0x" << std::hex << std::setfill('0') << std::setw(6) << getFormat()
                << " ver=0x" << std::setw(2) << (int)getVersion() << ")";
            return oss.str();
        }


        AmqpLanguageTag::AmqpLanguageTag(): AmqpSymbol() {}
        AmqpLanguageTag::~AmqpLanguageTag() {}


        AmqpFields::AmqpFields(): AmqpMap() {}
        AmqpFields::~AmqpFields() {}

    	// static
        amqp_provides_requires_list_t AmqpError::s_providesList = {amqpRequiresProvides_t::ERROR_CONDITION};
        // static
        amqp_error_list_t AmqpError::s_amqpErrors = {
            "amqp:internal-error",
            "amqp:not-found",
            "amqp:unauthorized-access",
            "amqp:decode-error",
            "amqp:resource-limit-exceeded",
            "amqp:not-allowed",
            "amqp:invalid-field",
            "amqp:not-implemented",
            "amqp:resource-locked",
            "amqp:precondition-failed",
            "amqp:resource-deleted",
            "amqp:illegal-state",
            "amqp:frame-size-too-small"
        };
        AmqpError::AmqpError(const amqp_error_t& v): AmqpSymbol() {
            bool found = false;
            for (amqp_error_list_citr_t i=s_amqpErrors.cbegin(); i!=s_amqpErrors.cend(); ++i) {
                if (v.compare(*i) == 0) {
                    found = true;
                    break;
                }
            }
            if (!found) throw amqpAnalyze::Error(MSG("Illegal value for AmqpError: \"" << v << "\""));
            _value.assign(v);
        }
        AmqpError::~AmqpError() {}


    	// static
        amqp_provides_requires_list_t AmqpConnectionError::s_providesList = {amqpRequiresProvides_t::ERROR_CONDITION};
        // static
        amqp_connection_error_list_t AmqpConnectionError::s_amqpConnectionErrors = {
            "amqp:connection:forced",
            "amqp:connection:framing-error",
            "amqp:connection:redirect"
        };
        AmqpConnectionError::AmqpConnectionError(const amqp_connection_error_t& v): AmqpSymbol() {
            bool found = false;
            for (amqp_connection_error_list_citr_t i=s_amqpConnectionErrors.cbegin(); i!=s_amqpConnectionErrors.cend(); ++i) {
                if (v.compare(*i) == 0) {
                    found = true;
                    break;
                }
            }
            if (!found) throw amqpAnalyze::Error(MSG("Illegal value for AmqpConnectionError: \"" << v << "\""));
            _value.assign(v);
        }
        AmqpConnectionError::~AmqpConnectionError() {}


    	// static
        amqp_provides_requires_list_t AmqpSessionError::s_providesList = {amqpRequiresProvides_t::ERROR_CONDITION};
        // static
        amqp_session_error_list_t AmqpSessionError::s_amqpSessionErrors = {
            "amqp:session:window-violation",
            "amqp:session:errant-link",
            "amqp:session:handle-in-use",
            "amqp:session:unattached-handle"
        };
        AmqpSessionError::AmqpSessionError(const amqp_session_error_t& v): AmqpSymbol() {
            bool found = false;
            for (amqp_session_error_list_citr_t i=s_amqpSessionErrors.cbegin(); i!=s_amqpSessionErrors.cend(); ++i) {
                if (v.compare(*i) == 0) {
                    found = true;
                    break;
                }
            }
            if (!found) throw amqpAnalyze::Error(MSG("Illegal value for AmqpSessionError: \"" << v << "\""));
            _value.assign(v);
        }
        AmqpSessionError::~AmqpSessionError() {}


    	// static
        amqp_provides_requires_list_t AmqpLinkError::s_providesList = {amqpRequiresProvides_t::ERROR_CONDITION};
        // static
        amqp_link_error_list_t AmqpLinkError::s_amqpLinkErrors = {
            "amqp:link:detach-forced",
            "amqp:link:transfer-limit-exceeded",
            "amqp:link:message-size-exceeded",
            "amqp:link:redirect",
            "amqp:link:stolen"
        };
        AmqpLinkError::AmqpLinkError(const amqp_link_error_t& v): AmqpSymbol() {
            bool found = false;
            for (amqp_link_error_list_citr_t i=s_amqpLinkErrors.cbegin(); i!=s_amqpLinkErrors.cend(); ++i) {
                if (v.compare(*i) == 0) {
                    found = true;
                    break;
                }
            }
            if (!found) throw amqpAnalyze::Error(MSG("Illegal value for AmqpLinkError: \"" << v << "\""));
            _value.assign(v);
        }
        AmqpLinkError::~AmqpLinkError() {}


        AmqpAnnotations::AmqpAnnotations(): AmqpMap() {}
        AmqpAnnotations::~AmqpAnnotations() {}


        // static
        amqp_provides_requires_list_t AmqpMessageIdUlong::s_providesList = {amqpRequiresProvides_t::MESSAGE_ID};
        AmqpMessageIdUlong::AmqpMessageIdUlong(amqp_msg_id_ulong_t v): AmqpUlong((amqp_ulong_t)v) {}
        AmqpMessageIdUlong::~AmqpMessageIdUlong() {}


        // static
        amqp_provides_requires_list_t AmqpMessageIdUuid::s_providesList = {amqpRequiresProvides_t::MESSAGE_ID};
        AmqpMessageIdUuid::AmqpMessageIdUuid(): AmqpUuid() {}
        AmqpMessageIdUuid::~AmqpMessageIdUuid() {}


        // static
        amqp_provides_requires_list_t AmqpMessageIdBinary::s_providesList = {amqpRequiresProvides_t::MESSAGE_ID};
        AmqpMessageIdBinary::AmqpMessageIdBinary(): AmqpBinary() {}
        AmqpMessageIdBinary::~AmqpMessageIdBinary() {}


        // static
        amqp_provides_requires_list_t AmqpMessageIdString::s_providesList = {amqpRequiresProvides_t::MESSAGE_ID};
        AmqpMessageIdString::AmqpMessageIdString(): AmqpString() {}
        AmqpMessageIdString::~AmqpMessageIdString() {}


        // static
        amqp_provides_requires_list_t AmqpAddress::s_providesList = {amqpRequiresProvides_t::ADDRESS};
        AmqpAddress::AmqpAddress(): AmqpString() {}
        AmqpAddress::~AmqpAddress() {}


        // static
        std::map<amqp_terminus_durability_t, const char*> AmqpTerminusDurability::s_choiceNames = {
            {amqp_terminus_durability_t::NONE, "none"},
            {amqp_terminus_durability_t::CONFIGURATION, "configuration"},
            {amqp_terminus_durability_t::UNSETTLED_STATE, "unsettled-state"}
        };
        AmqpTerminusDurability::AmqpTerminusDurability(amqp_terminus_durability_t v): AmqpUint((amqp_uint_t)v) {}
        AmqpTerminusDurability::~AmqpTerminusDurability() {}
        std::string AmqpTerminusDurability::valueStr() const {
            return s_choiceNames.at((amqp_terminus_durability_t)_value);
        }


        // static
        amqp_terminus_expiry_policy_list_t AmqpTerminusExpiryPolicy::s_terminusExpiryPolicyList = {
             "link-detach",
             "session-end",
             "connection-close",
             "never"
        };
        AmqpTerminusExpiryPolicy::AmqpTerminusExpiryPolicy(const amqp_terminus_expiry_policy_t& v): AmqpSymbol() {
            bool found = false;
            for (amqp_terminus_expiry_policy_list_citr_t i=s_terminusExpiryPolicyList.cbegin(); i!=s_terminusExpiryPolicyList.cend(); ++i) {
                if (v.compare(*i) == 0) {
                    found = true;
                    break;
                }
            }
            if (!found) throw amqpAnalyze::Error(MSG("Illegal value for AmqpTerminusExpiryPolicy: \"" << v << "\""));
            _value.assign(v);
        }
        AmqpTerminusExpiryPolicy::~AmqpTerminusExpiryPolicy() {}


        // static
        amqp_std_dist_mode_list_t AmqpStandardDistributionMode::s_StandardDistributionModeList {
            "move",
            "copy"
        };
        // static
        amqp_provides_requires_list_t AmqpStandardDistributionMode::s_providesList= {
        	amqpRequiresProvides_t::DISTRIBUTION_MODE
        };
        AmqpStandardDistributionMode::AmqpStandardDistributionMode(const amqp_std_dist_mode_t& v): AmqpSymbol() {
            bool found = false;
            for (amqp_std_dist_mode_list_citr_tt i=s_StandardDistributionModeList.cbegin(); i!=s_StandardDistributionModeList.cend(); ++i) {
                if (v.compare(*i) == 0) {
                    found = true;
                    break;
                }
            }
            if (!found) throw amqpAnalyze::Error(MSG("Illegal value for AmqpStandardDistributionMode: \"" << v << "\""));
            _value.assign(v);
        }
        AmqpStandardDistributionMode::~AmqpStandardDistributionMode() {}


        AmqpFilterSet::AmqpFilterSet(): AmqpMap() {}
        AmqpFilterSet::~AmqpFilterSet() {}


        AmqpNodeProperties::AmqpNodeProperties(): AmqpFields() {}
        AmqpNodeProperties::~AmqpNodeProperties() {}


        AmqpTransactionId::AmqpTransactionId(): AmqpBinary() {}
        AmqpTransactionId::~AmqpTransactionId() {}
        // static
        amqp_provides_requires_list_t AmqpTransactionId::s_providesList = {
        	amqpRequiresProvides_t::TXN_ID
        };

        // static
        amqp_transaction_capability_list_t AmqpTransactionalCapability::s_transactionCapabilityList = {
            "local-transactions",
            "distributed-transactions",
            "promotable-transactions",
            "multi-txns-per-ssn",
            "multi-ssns-per-txn"
        };
        // static
        amqp_provides_requires_list_t AmqpTransactionalCapability::s_providesList = {
        	amqpRequiresProvides_t::TXN_CAPABILITY
        };
        AmqpTransactionalCapability::AmqpTransactionalCapability(const amqp_transaction_capability_t& v): AmqpSymbol() {
            bool found = false;
            for (amqp_transaction_capability_list_citr_t i=s_transactionCapabilityList.cbegin(); i!=s_transactionCapabilityList.cend(); ++i) {
                if (v.compare(*i) == 0) {
                    found = true;
                    break;
                }
            }
            if (!found) throw amqpAnalyze::Error(MSG("Illegal value for TransactionalCapability: \"" << v << "\""));
            _value.assign(v);
        }
        AmqpTransactionalCapability::~AmqpTransactionalCapability() {}


        // static
        amqp_transaction_error_list_t AmqpTransactionError::s_transactionErrorList = {
            "unknown-id",
            "transaction-rollback",
            "transaction-timeout"
        };
        // static
        amqp_provides_requires_list_t AmqpTransactionError::s_providesList = {
        	amqpRequiresProvides_t::ERROR_CONDITION
        };
        AmqpTransactionError::AmqpTransactionError(const amqp_transaction_error_t& v): AmqpSymbol() {
            bool found = false;
            for (amqp_transaction_error_list_citr_t i=s_transactionErrorList.cbegin(); i!=s_transactionErrorList.cend(); ++i) {
                if (v.compare(*i) == 0) {
                    found = true;
                    break;
                }
            }
            if (!found) throw amqpAnalyze::Error(MSG("Illegal value for TransactionalCapability: \"" << v << "\""));
            _value.assign(v);
        }
        AmqpTransactionError::~AmqpTransactionError() {}


        // static
        std::map<amqp_sasl_code_t, const char*> AmqpSaslCode::s_choiceNames = {
            {amqp_sasl_code_t::OK, "ok"},
            {amqp_sasl_code_t::AUTH, "auth"},
            {amqp_sasl_code_t::SYS, "sys"},
            {amqp_sasl_code_t::SYS_PERM, "sys-perm"},
            {amqp_sasl_code_t::SYS_TEMP, "sys-temp"}
        };
        AmqpSaslCode::AmqpSaslCode(amqp_sasl_code_t v): AmqpUbyte((amqp_ubyte_t)v) {}
        AmqpSaslCode::~AmqpSaslCode() {}
        std::string AmqpSaslCode::valueStr() const {
            return s_choiceNames.at((amqp_sasl_code_t)_value);
        }


        // static
        std::map<amqpCompositeType_t, const char*> CompositeType::s_amqpCompositeTypeNames = {
                        {amqpCompositeType_t::ERROR, "error"},
                        {amqpCompositeType_t::RECEIVED, "received"},
                        {amqpCompositeType_t::ACCEPTED, "accepted"},
                        {amqpCompositeType_t::REJECTED, "rejected"},
                        {amqpCompositeType_t::RELEASED, "released"},
                        {amqpCompositeType_t::MODIFIED, "modified"},
                        {amqpCompositeType_t::SOURCE, "source"},
                        {amqpCompositeType_t::TARGET, "target"},
                        {amqpCompositeType_t::DELETE_ON_CLOSE, "delete-on-close"},
                        {amqpCompositeType_t::DELETE_ON_NO_LINKS, "delete-on-no-links"},
                        {amqpCompositeType_t::DELETE_ON_NO_MESSAGES, "delete-on-no-messages"},
                        {amqpCompositeType_t::DELETE_ON_NO_LINKS_OR_MESSAGES, "delete-on-no-links-or-messages"},
                        {amqpCompositeType_t::COORDINATOR, "coordinator"},
                        {amqpCompositeType_t::DECLARE, "declare"},
                        {amqpCompositeType_t::DISCHARGE, "discharge"},
                        {amqpCompositeType_t::DECLARED, "declared"},
                        {amqpCompositeType_t::TRANSACTIONAL_STATE, "transactional-state"},
                        {amqpCompositeType_t::SASL_MECHANISMS, "sasl-mechanisms"},
                        {amqpCompositeType_t::SASL_INIT, "sasl-init"},
                        {amqpCompositeType_t::SASL_CHALLENGE, "sasl-challenge"},
                        {amqpCompositeType_t::SASL_RESPONSE, "sasl-response"},
                        {amqpCompositeType_t::SASL_OUTCOME, "sasl-outcome"}
        };
        CompositeType::CompositeType(AmqpList* fieldListPtr): Type(), _fieldListPtr(fieldListPtr) {}
        CompositeType::~CompositeType() {
            if (_fieldListPtr != nullptr) {
                delete _fieldListPtr;
                _fieldListPtr = nullptr;
            }
        }
        std::string CompositeType::toString(std::size_t margin) const {
            std::ostringstream oss;
            oss << typeStr() << ":" << std::endl << std::string(margin, ' ');
            _fieldListPtr->appendString(oss, margin);
            return oss.str();
        }
        std::string CompositeType::valueStr() const {
            return _fieldListPtr->valueStr();
        }
        // static
        amqp_provides_requires_list_t CompositeType::s_providesList = {};


        AmqpErrorRecord::AmqpErrorRecord(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpErrorRecord::~AmqpErrorRecord() {}
        // static
        fieldTypeList_t AmqpErrorRecord::s_fieldTypeList = {
            FieldType("condition", amqpPrimitiveType_t::SYMBOL_TYPE, true, false, {amqpRequiresProvides_t::ERROR_CONDITION}),
            FieldType("description", amqpPrimitiveType_t::STRING_TYPE, false, false),
            FieldType("info", amqpPrimitiveType_t::FIELDS_TYPE, false, false)
        };


        AmqpReceived::AmqpReceived(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpReceived::~AmqpReceived() {}
        // static
        fieldTypeList_t AmqpReceived::s_fieldTypeList = {
            FieldType("section-number", amqpPrimitiveType_t::UINT_TYPE, true, false),
            FieldType("section-offset", amqpPrimitiveType_t::ULONG_TYPE, true, false)
        };
        // static
        amqp_provides_requires_list_t AmqpReceived::s_providesList = {
        	amqpRequiresProvides_t::DELIVERY_STATE
        };

        AmqpAccepted::AmqpAccepted(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpAccepted::~AmqpAccepted() {}
        // static
        fieldTypeList_t AmqpAccepted::s_fieldTypeList = {};
        // static
        amqp_provides_requires_list_t AmqpAccepted::s_providesList = {
        	amqpRequiresProvides_t::DELIVERY_STATE,
        	amqpRequiresProvides_t::OUTCOME
        };


        AmqpRejected::AmqpRejected(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpRejected::~AmqpRejected() {}
        // static
        fieldTypeList_t AmqpRejected::s_fieldTypeList = {
            FieldType("error", amqpPrimitiveType_t::AMQP_ERROR_TYPE, false, false)
        };
        // static
        amqp_provides_requires_list_t AmqpRejected::s_providesList = {
        	amqpRequiresProvides_t::DELIVERY_STATE,
        	amqpRequiresProvides_t::OUTCOME
        };


        AmqpReleased::AmqpReleased(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpReleased::~AmqpReleased() {}
        // static
        fieldTypeList_t AmqpReleased::s_fieldTypeList = {};
        // static
        amqp_provides_requires_list_t AmqpReleased::s_providesList = {
        	amqpRequiresProvides_t::DELIVERY_STATE,
        	amqpRequiresProvides_t::OUTCOME
        };


        AmqpModified::AmqpModified(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpModified::~AmqpModified() {}
        // static
        fieldTypeList_t AmqpModified::s_fieldTypeList = {
            FieldType("delivery-failed", amqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType("undeliverable-here", amqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType("message-annotations", amqpPrimitiveType_t::FIELDS_TYPE, false, false)
        };
        // static
        amqp_provides_requires_list_t AmqpModified::s_providesList = {
        	amqpRequiresProvides_t::DELIVERY_STATE,
        	amqpRequiresProvides_t::OUTCOME
        };


        AmqpSource::AmqpSource(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpSource::~AmqpSource() {}
        // static
        fieldTypeList_t AmqpSource::s_fieldTypeList = {
            FieldType("address", "*", false, false, {amqpRequiresProvides_t::ADDRESS}),
            FieldType("durable", amqpPrimitiveType_t::TERMINUS_DURABILITY_TYPE, false, false),
            FieldType("expiry-policy", amqpPrimitiveType_t::TERMINUS_EXPIRY_POLICY_TYPE, false, false),
            FieldType("timeout", amqpPrimitiveType_t::SECONDS_TYPE, false, false),
            FieldType("dynamic", amqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType("dynamic-node-properties", amqpPrimitiveType_t::NODE_PROPERTIES_TYPE, false, false),
            FieldType("distribution-mode", amqpPrimitiveType_t::SYMBOL_TYPE, false, false, {amqpRequiresProvides_t::DISTRIBUTION_MODE}),
            FieldType("filter", amqpPrimitiveType_t::FILTER_SET_TYPE, false, false),
            FieldType("default-outcome", "*", false, false, {amqpRequiresProvides_t::OUTCOME}),
            FieldType("outcomes", amqpPrimitiveType_t::SYMBOL_TYPE, false, true),
            FieldType("capabilities", amqpPrimitiveType_t::SYMBOL_TYPE, false, true)
        };
        // static
        amqp_provides_requires_list_t AmqpSource::s_providesList = {
        	amqpRequiresProvides_t::SOURCE
        };


        AmqpTarget::AmqpTarget(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpTarget::~AmqpTarget() {}
        // static
        fieldTypeList_t AmqpTarget::s_fieldTypeList = {
            FieldType("address", "*", false, false, {amqpRequiresProvides_t::ADDRESS}),
            FieldType("durable", amqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType("expiry-policy", amqpPrimitiveType_t::TERMINUS_EXPIRY_POLICY_TYPE, false, false),
            FieldType("timeout", amqpPrimitiveType_t::SECONDS_TYPE, false, false),
            FieldType("dynamic", amqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType("dynamic-node-properties", amqpPrimitiveType_t::NODE_PROPERTIES_TYPE, false, false),
            FieldType("capabilities", amqpPrimitiveType_t::SYMBOL_TYPE, false, true)
        };
        // static
        amqp_provides_requires_list_t AmqpTarget::s_providesList = {
        	amqpRequiresProvides_t::TARGET
        };

        AmqpDeleteOnClose::AmqpDeleteOnClose(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpDeleteOnClose::~AmqpDeleteOnClose() {}
        // static
        fieldTypeList_t AmqpDeleteOnClose::s_fieldTypeList = {};


        AmqpDeleteOnNoLinks::AmqpDeleteOnNoLinks(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpDeleteOnNoLinks::~AmqpDeleteOnNoLinks() {}
        // static
        fieldTypeList_t AmqpDeleteOnNoLinks::s_fieldTypeList = {};


        AmqpDeleteOnNoMessages::AmqpDeleteOnNoMessages(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpDeleteOnNoMessages::~AmqpDeleteOnNoMessages() {}
        // static
        fieldTypeList_t AmqpDeleteOnNoMessages::s_fieldTypeList = {};


        AmqpDeleteOnNoLinksOrMessages::AmqpDeleteOnNoLinksOrMessages(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpDeleteOnNoLinksOrMessages::~AmqpDeleteOnNoLinksOrMessages() {}
        // static
        fieldTypeList_t AmqpDeleteOnNoLinksOrMessages::s_fieldTypeList = {};


        AmqpCoordinator::AmqpCoordinator(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpCoordinator::~AmqpCoordinator() {}
        // static
        fieldTypeList_t AmqpCoordinator::s_fieldTypeList = {
            FieldType("capabilities", amqpPrimitiveType_t::SYMBOL_TYPE, false, true, {amqpRequiresProvides_t::TXN_CAPABILITY})
        };
        // static
        amqp_provides_requires_list_t AmqpCoordinator::s_providesList = {
        	amqpRequiresProvides_t::TARGET
        };


        AmqpDeclare::AmqpDeclare(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpDeclare::~AmqpDeclare() {}
        // static
        fieldTypeList_t AmqpDeclare::s_fieldTypeList = {
            FieldType("global-id", "*", false, false, {amqpRequiresProvides_t::GLOBAL_TX_ID}) // NOTE: GLOBAL_TX_ID is not yet defined by AMQP 1.0 spec
        };


        AmqpDischarge::AmqpDischarge(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpDischarge::~AmqpDischarge() {}
        // static
        fieldTypeList_t AmqpDischarge::s_fieldTypeList = {
            FieldType("txn-id", "*", true, false, {amqpRequiresProvides_t::TXN_ID}),
            FieldType("fail", amqpPrimitiveType_t::BOOLEAN_TYPE, false, false)
        };


        AmqpDeclared::AmqpDeclared(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpDeclared::~AmqpDeclared() {}
        // static
        fieldTypeList_t AmqpDeclared::s_fieldTypeList = {
            FieldType("txn-id", "*", true, false, {amqpRequiresProvides_t::TXN_ID})
        };
        // static
        amqp_provides_requires_list_t AmqpDeclared::s_providesList = {
        	amqpRequiresProvides_t::DELIVERY_STATE,
        	amqpRequiresProvides_t::OUTCOME
        };


        AmqpTransactionalState::AmqpTransactionalState(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpTransactionalState::~AmqpTransactionalState() {}
        // static
        fieldTypeList_t AmqpTransactionalState::s_fieldTypeList = {
            FieldType("txn-id", "*", true, false, {amqpRequiresProvides_t::TXN_ID}),
            FieldType("outcome", "*", false, false, {amqpRequiresProvides_t::OUTCOME})
        };
        // static
        amqp_provides_requires_list_t AmqpTransactionalState::s_providesList = {
        	amqpRequiresProvides_t::DELIVERY_STATE
        };


        AmqpSaslMechanisms::AmqpSaslMechanisms(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpSaslMechanisms::~AmqpSaslMechanisms() {}
        // static
        fieldTypeList_t AmqpSaslMechanisms::s_fieldTypeList = {
            FieldType("sasl-server-mechanisms", amqpPrimitiveType_t::SYMBOL_TYPE, true, true)
        };


        AmqpSaslInit::AmqpSaslInit(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpSaslInit::~AmqpSaslInit() {}
        // static
        fieldTypeList_t AmqpSaslInit::s_fieldTypeList = {
            FieldType("mechanism", amqpPrimitiveType_t::SYMBOL_TYPE, true, false),
            FieldType("initial-response", amqpPrimitiveType_t::BINARY_TYPE, false, false),
            FieldType("hostname", amqpPrimitiveType_t::STRING_TYPE, false, false)
        };


        AmqpSaslChallenge::AmqpSaslChallenge(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpSaslChallenge::~AmqpSaslChallenge() {}
        // static
        fieldTypeList_t AmqpSaslChallenge::s_fieldTypeList = {
            FieldType("challenge", amqpPrimitiveType_t::BINARY_TYPE, true, false)
        };


        AmqpSaslResponse::AmqpSaslResponse(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpSaslResponse::~AmqpSaslResponse() {}
        // static
        fieldTypeList_t AmqpSaslResponse::s_fieldTypeList = {
            FieldType("response", amqpPrimitiveType_t::BINARY_TYPE, true, false)
        };


        AmqpSaslOutcome::AmqpSaslOutcome(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpSaslOutcome::~AmqpSaslOutcome() {}
        // static
        fieldTypeList_t AmqpSaslOutcome::s_fieldTypeList = {
            FieldType("code", amqpPrimitiveType_t::SASL_CODE_TYPE, true, false),
            FieldType("additional-data", amqpPrimitiveType_t::BINARY_TYPE, false, false)
        };


    } /* namespace amqp10 */
} /* namespace amqpAnalyze */
