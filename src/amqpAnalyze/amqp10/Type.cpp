/*
 * AmqpTypes.cpp
 *
 *  Created on: Apr 5, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/amqp10/Type.hpp>

#include <amqpAnalyze/amqp10/AmqpBlock.hpp>
#include <amqpAnalyze/amqp10/FieldType.hpp>
#include <amqpAnalyze/amqp10/ProvidesRequires.hpp>
#include <amqpAnalyze/Color.hpp>
#include <cstring>
#include <iomanip>

namespace amqpAnalyze
{
    namespace amqp10
    {
        Type::Type(): _name(nullptr) {}
        Type::Type(const char* name): _name(name) {}
        Type::~Type() {}
        bool Type::hasName() const {
            return _name != nullptr;
        }
        const char* Type::name() const {
            return _name;
        }
        std::string Type::nameValueStr(bool colorFlag, const char* valueDelim) const {
            std::ostringstream oss;
            oss << (_name == nullptr ? "<no-name>" : _name) << valueDelim[0] << valueStr(colorFlag) << valueDelim[1];
            return oss.str();
        }
        std::string Type::nameTypeValueStr(bool colorFlag, const char* valueDelim) const {
            std::ostringstream oss;
            oss << (_name == nullptr ? "<no-name>" : _name) << ":" << typeStr() << valueDelim[0] << valueStr(colorFlag) << valueDelim[1];
            return oss.str();
        }
        std::string Type::typeValueStr(bool colorFlag, const char* valueDelim) const {
            std::ostringstream oss;
            oss << typeStr() << valueDelim[0] << valueStr(colorFlag) << valueDelim[1];
            return oss.str();
        }
        // static
        bool Type::provides(AmqpRequiresProvides_t requires, const AmqpProvidesRequiresList_t& prList) {
            bool found = false;
            for (AmqpProvidesRequiresListCitr_t i = prList.cbegin(); i!=prList.cend(); ++i) {
                if (*i == requires) {
                    found = true;
                    break;
                }
            }
            return found;
        }
       // static
        std::string Type::providesListAsString(const AmqpProvidesRequiresList_t& prList) {
            std::ostringstream oss;
            oss << "[";
            for (AmqpProvidesRequiresListCitr_t i=prList.cbegin(); i!=prList.cend(); ++i) {
                if (i != prList.cbegin()) oss << ", ";
                oss << providesRequiresNames[*i];
            }
            oss << "]";
            return oss.str();
        }


        PrimitiveType::PrimitiveType(): Type() {}
        PrimitiveType::PrimitiveType(const char* name): Type(name) {}
        PrimitiveType::~PrimitiveType() {}
        // static
        AmqpProvidesRequiresList_t PrimitiveType::s_providesList = {};
        // static
        std::map<AmqpPrimitiveType_t, const char*> PrimitiveType::s_amqpSimpleTypeNames = {
            // Primitive types
            {AmqpPrimitiveType_t::NULL_TYPE, "null"},
            {AmqpPrimitiveType_t::BOOLEAN_TYPE, "boolean"},
            {AmqpPrimitiveType_t::UBYTE_TYPE, "ubyte"},
            {AmqpPrimitiveType_t::USHORT_TYPE, "ushort"},
            {AmqpPrimitiveType_t::UINT_TYPE, "uint"},
            {AmqpPrimitiveType_t::ULONG_TYPE, "ulong"},
            {AmqpPrimitiveType_t::BYTE_TYPE, "byte"},
            {AmqpPrimitiveType_t::SHORT_TYPE, "short"},
            {AmqpPrimitiveType_t::INT_TYPE, "int"},
            {AmqpPrimitiveType_t::LONG_TYPE, "long"},
            {AmqpPrimitiveType_t::FLOAT_TYPE, "float"},
            {AmqpPrimitiveType_t::DOUBLE_TYPE, "double"},
            {AmqpPrimitiveType_t::DECIMAL32_TYPE, "decimal32"},
            {AmqpPrimitiveType_t::DECIMAL64_TYPE, "decimal64"},
            {AmqpPrimitiveType_t::DECIMAL128_TYPE, "decimal128"},
            {AmqpPrimitiveType_t::CHAR_TYPE, "char"},
            {AmqpPrimitiveType_t::TIMESTAMP_TYPE, "timestamp"},
            {AmqpPrimitiveType_t::UUID_TYPE, "uuid"},
            {AmqpPrimitiveType_t::BINARY_TYPE, "binary"},
            {AmqpPrimitiveType_t::STRING_TYPE, "string"},
            {AmqpPrimitiveType_t::SYMBOL_TYPE, "symbol"},
            {AmqpPrimitiveType_t::LIST_TYPE, "list"},
            {AmqpPrimitiveType_t::MAP_TYPE, "map"},
            {AmqpPrimitiveType_t::ARRAY_TYPE, "array"},
            // Restricted types
            {AmqpPrimitiveType_t::ROLE_TYPE, "role"},
            {AmqpPrimitiveType_t::SENDER_SETTLE_MODE_TYPE, "sender-settle-mode"},
            {AmqpPrimitiveType_t::RECEIVER_SETTLE_MODE_TYPE, "receiver-settle-mode"},
            {AmqpPrimitiveType_t::HANDLE_TYPE, "handle"},
            {AmqpPrimitiveType_t::SECONDS_TYPE, "seconds"},
            {AmqpPrimitiveType_t::MILLISECONDS_TYPE, "milliseconds"},
            {AmqpPrimitiveType_t::DELIVERY_TAG_TYPE, "delivery-tag"},
            {AmqpPrimitiveType_t::DELIVERY_NUMBER_TYPE, "delivery-number"},
            {AmqpPrimitiveType_t::TRANSFER_NUMBER_TYPE, "transfer-number"},
            {AmqpPrimitiveType_t::SEQUENCE_NUMBER_TYPE, "sequence-number"},
            {AmqpPrimitiveType_t::MESSAGE_FORMAT_TYPE, "message-format"},
            {AmqpPrimitiveType_t::LANGUAGE_TAG_TYPE, "language-tag"},
            {AmqpPrimitiveType_t::FIELDS_TYPE, "fields"},
            {AmqpPrimitiveType_t::AMQP_ERROR_TYPE, "amqp-error"},
            {AmqpPrimitiveType_t::CONNECTION_ERROR_TYPE, "connection-error"},
            {AmqpPrimitiveType_t::SESSION_ERROR_TYPE, "session-error"},
            {AmqpPrimitiveType_t::LINK_ERROR_TYPE, "link-error"},
            {AmqpPrimitiveType_t::ANNOTATIONS_TYPE, "annotations"},
            {AmqpPrimitiveType_t::MESSAGE_ID_ULONG_TYPE, "message-id-ulong"},
            {AmqpPrimitiveType_t::MESSAGE_ID_UUID_TYPE, "message-id-uuid"},
            {AmqpPrimitiveType_t::MESSAGE_ID_BINARY_TYPE, "message-id-binary"},
            {AmqpPrimitiveType_t::MESSAGE_ID_STRING_TYPE, "message-id-string"},
            {AmqpPrimitiveType_t::ADDRESS_TYPE, "address-string"},
            {AmqpPrimitiveType_t::TERMINUS_DURABILITY_TYPE, "terminus-durability"},
            {AmqpPrimitiveType_t::TERMINUS_EXPIRY_POLICY_TYPE, "terminus-expiry-policy"},
            {AmqpPrimitiveType_t::STD_DIST_MODE_TYPE, "std-dist-mode"},
            {AmqpPrimitiveType_t::FILTER_SET_TYPE, "filter-set"},
            {AmqpPrimitiveType_t::NODE_PROPERTIES_TYPE, "node-properties"},
            {AmqpPrimitiveType_t::TRANSACTION_ID_TYPE, "transaction-id"},
            {AmqpPrimitiveType_t::TRANSACTION_CAPABILITY_TYPE, "txn-capability"},
            {AmqpPrimitiveType_t::TRANSACTION_ERROR_TYPE, "transaction-error"},
            {AmqpPrimitiveType_t::SASL_CODE_TYPE, "sasl-code"}
        };


        AmqpNull::AmqpNull(): PrimitiveType() {}
        AmqpNull::AmqpNull(const char* name): PrimitiveType(name) {}
        AmqpNull::~AmqpNull() {}
        std::string AmqpNull::nameValueStr(bool colorFlag, const char* valueDelim) const {
            std::ostringstream oss;
            oss << (_name == nullptr ? "<no-name>" : _name) << valueDelim[0] << valueStr(colorFlag) << valueDelim[1];
            return oss.str();
        }
        std::string AmqpNull::nameTypeValueStr(bool colorFlag, const char* valueDelim) const {
            std::ostringstream oss;
            oss << (_name == nullptr ? "<no-name>" : _name) << ":" << typeStr();
            return oss.str();
        }
        std::string AmqpNull::typeValueStr(bool colorFlag, const char* valueDelim) const {
            return valueStr(colorFlag);
        }
        std::string AmqpNull::valueStr(bool colorFlag) const {
            return Color::color(DisplayColorType_t::AMQP_TYPE_NULL, "null");
        }


        AmqpBoolean::AmqpBoolean(bool v): PrimitiveType(), _value(v) {}
        AmqpBoolean::AmqpBoolean(bool v, const char* name): PrimitiveType(name), _value(v) {}
        AmqpBoolean::~AmqpBoolean() {}
        std::string AmqpBoolean::valueStr(bool colorFlag) const {
            return Color::color(DisplayColorType_t::AMQP_TYPE_BOOLEAN, (_value ? "true" : "false"));
        }


        AmqpUbyte::AmqpUbyte(uint8_t v): PrimitiveType(), _value(v) {}
        AmqpUbyte::AmqpUbyte(uint8_t v, const char* name): PrimitiveType(name), _value(v) {}
        AmqpUbyte::~AmqpUbyte() {}
        std::string AmqpUbyte::valueStr(bool colorFlag) const {
            std::stringstream oss;
            oss << "0x" << std::hex << (int)_value;
            return Color::color(DisplayColorType_t::AMQP_TYPE_NUMBER, oss.str());
        }


        AmqpUshort::AmqpUshort(uint16_t v): PrimitiveType(), _value(v) {}
        AmqpUshort::AmqpUshort(uint16_t v, const char* name): PrimitiveType(name), _value(v) {}
        AmqpUshort::~AmqpUshort() {}
        std::string AmqpUshort::valueStr(bool colorFlag) const {
            std::stringstream oss;
            oss << "0x" << std::hex << _value;
            return Color::color(DisplayColorType_t::AMQP_TYPE_NUMBER, oss.str());
        }


        AmqpUint::AmqpUint(uint32_t v): PrimitiveType(), _value(v) {}
        AmqpUint::AmqpUint(uint32_t v, const char* name): PrimitiveType(name), _value(v) {}
        AmqpUint::~AmqpUint() {}
        std::string AmqpUint::valueStr(bool colorFlag) const {
            std::ostringstream oss;
            oss<< "0x" << std::hex << _value;
            return Color::color(DisplayColorType_t::AMQP_TYPE_NUMBER, oss.str());
        }


        AmqpUlong::AmqpUlong(uint64_t v): PrimitiveType(), _value(v) {}
        AmqpUlong::AmqpUlong(uint64_t v, const char* name): PrimitiveType(name), _value(v) {}
        AmqpUlong::~AmqpUlong() {}
        std::string AmqpUlong::valueStr(bool colorFlag) const {
            std::ostringstream oss;
            oss << "0x" << std::hex << _value;
            return Color::color(DisplayColorType_t::AMQP_TYPE_NUMBER, oss.str());
        }


        AmqpByte::AmqpByte(int8_t v): PrimitiveType(), _value(v) {}
        AmqpByte::AmqpByte(int8_t v, const char* name): PrimitiveType(name), _value(v) {}
        AmqpByte::~AmqpByte() {}
        std::string AmqpByte::valueStr(bool colorFlag) const {
            std::ostringstream oss;
            oss <<  "0x" << std::hex << (int)(uint8_t)_value;
            return Color::color(DisplayColorType_t::AMQP_TYPE_NUMBER, oss.str());
        }


        AmqpShort::AmqpShort(int16_t v): PrimitiveType(), _value(v) {}
        AmqpShort::AmqpShort(int16_t v, const char* name): PrimitiveType(name), _value(v) {}
        AmqpShort::~AmqpShort() {}
        std::string AmqpShort::valueStr(bool colorFlag) const {
            std::ostringstream oss;
            oss << "0x" << std::hex << _value;
            return Color::color(DisplayColorType_t::AMQP_TYPE_NUMBER, oss.str());
        }


        AmqpInt::AmqpInt(int32_t v): PrimitiveType(), _value(v) {}
        AmqpInt::AmqpInt(int32_t v, const char* name): PrimitiveType(name), _value(v) {}
        AmqpInt::~AmqpInt() {}
        std::string AmqpInt::valueStr(bool colorFlag) const {
            std::ostringstream oss;
            oss << "0x" << std::hex << _value;
            return Color::color(DisplayColorType_t::AMQP_TYPE_NUMBER, oss.str());
        }


        AmqpLong::AmqpLong(int64_t v): PrimitiveType(), _value(v) {}
        AmqpLong::AmqpLong(int64_t v, const char* name): PrimitiveType(name), _value(v) {}
        AmqpLong::~AmqpLong() {}
        std::string AmqpLong::valueStr(bool colorFlag) const {
            std::ostringstream oss;
            oss << "0x" << std::hex << _value;
            return Color::color(DisplayColorType_t::AMQP_TYPE_NUMBER, oss.str());
        }


        AmqpFloat::AmqpFloat(float v): PrimitiveType(), _value(v) {}
        AmqpFloat::AmqpFloat(float v, const char* name): PrimitiveType(name), _value(v) {}
        AmqpFloat::~AmqpFloat() {}
        std::string AmqpFloat::valueStr(bool colorFlag) const {
            std::ostringstream oss;
            oss << _value;
            return Color::color(DisplayColorType_t::AMQP_TYPE_NUMBER, oss.str());
        }


        AmqpDouble::AmqpDouble(double v): PrimitiveType(), _value(v) {}
        AmqpDouble::AmqpDouble(double v, const char* name): PrimitiveType(name), _value(v) {}
        AmqpDouble::~AmqpDouble() {}
        std::string AmqpDouble::valueStr(bool colorFlag) const {
            std::ostringstream oss;
            oss << _value;
            return Color::color(DisplayColorType_t::AMQP_TYPE_NUMBER, oss.str());
        }


        AmqpDecimal32::AmqpDecimal32(): PrimitiveType(), _value({0, 0, 0, 0}) {}
        AmqpDecimal32::AmqpDecimal32(const char* name): PrimitiveType(name), _value({0, 0, 0, 0}) {}
        AmqpDecimal32::~AmqpDecimal32() {}
        std::string AmqpDecimal32::valueStr(bool colorFlag) const {
            std::ostringstream oss;
            oss << std::hex << std::setfill('0');
            for (int i=0; i<4; ++i) {
                oss << (i>0 ? ", 0x" : "0x") << std::setw(2) << (int)_value[i];
            }
            return Color::color(DisplayColorType_t::AMQP_TYPE_NUMBER, oss.str());
        }


        AmqpDecimal64::AmqpDecimal64(): PrimitiveType(), _value({0, 0, 0, 0, 0, 0, 0, 0}) {}
        AmqpDecimal64::AmqpDecimal64(const char* name): PrimitiveType(name), _value({0, 0, 0, 0, 0, 0, 0, 0}) {}
        AmqpDecimal64::~AmqpDecimal64() {}
        std::string AmqpDecimal64::valueStr(bool colorFlag) const {
            std::ostringstream oss;
            oss << std::hex << std::setfill('0');
            for (int i=0; i<8; ++i) {
                oss << (i>0 ? ", 0x" : "0x") << std::setw(2) << (int)_value[i];
            }
            return Color::color(DisplayColorType_t::AMQP_TYPE_NUMBER, oss.str());
        }


        AmqpDecimal128::AmqpDecimal128(): PrimitiveType(), _value({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}) {}
        AmqpDecimal128::AmqpDecimal128(const char* name): PrimitiveType(name), _value({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}) {}
        AmqpDecimal128::~AmqpDecimal128() {}
        std::string AmqpDecimal128::valueStr(bool colorFlag) const {
            std::ostringstream oss;
            oss << std::hex << std::setfill('0');
            for (int i=0; i<16; ++i) {
                oss << (i>0 ? ", 0x" : "0x") << std::setw(2) << (int)_value[i];
            }
            return Color::color(DisplayColorType_t::AMQP_TYPE_NUMBER, oss.str());
        }


        AmqpChar::AmqpChar(char32_t v): PrimitiveType(), _value(v) {}
        AmqpChar::AmqpChar(char32_t v, const char* name): PrimitiveType(name), _value(v) {}
        AmqpChar::~AmqpChar() {}
        std::string AmqpChar::valueStr(bool colorFlag) const {
            std::ostringstream oss;
            oss << "0x" << std::hex << (int)_value << "='" << _value << "'";
            return Color::color(DisplayColorType_t::AMQP_TYPE_CHAR, oss.str());
        }


        AmqpTimestamp::AmqpTimestamp(uint64_t v): PrimitiveType(), _value(v) {}
        AmqpTimestamp::AmqpTimestamp(uint64_t v, const char* name): PrimitiveType(name), _value(v) {}
        AmqpTimestamp::~AmqpTimestamp() {}
        std::string AmqpTimestamp::valueStr(bool colorFlag) const {
            std::ostringstream oss;
            std::time_t t = (std::time_t)(_value/1000);
            char buf[20];
            std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));
            char tzbuf[6];
            std::strftime(tzbuf, sizeof(tzbuf), "%z", std::localtime(&t));
            oss << Color::color(DisplayColorType_t::AMQP_TYPE_NUMBER, MSG("0x" << std::hex << _value));
            if (_value > 0) {
                oss << "=" << Color::color(DisplayColorType_t::AMQP_TYPE_TIMESTAMP, MSG(buf << "." << std::setfill('0') << std::setw(3) << (_value%1000) << " " << tzbuf));
            }
            return oss.str();
        }


        AmqpUuid::AmqpUuid(): PrimitiveType(), _value({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}) {}
        AmqpUuid::AmqpUuid(const char* name): PrimitiveType(name), _value({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}) {}
        AmqpUuid::~AmqpUuid() {}
        std::string AmqpUuid::valueStr(bool colorFlag) const {
            std::ostringstream oss;
            oss << std::hex << std::setfill('0');
            for (int i=0; i<16; ++i) {
                if (i==4 || i==6 || i==8 || i==10) oss << '-';
                oss << std::setw(2) << (int)_value[i];
            }
            return Color::color(DisplayColorType_t::AMQP_TYPE_UUID, oss.str());
        }


        AmqpBinary::AmqpBinary(): PrimitiveType(), _value() {}
        AmqpBinary::AmqpBinary(const char* name): PrimitiveType(name), _value() {}
        AmqpBinary::~AmqpBinary() {}
        std::string AmqpBinary::valueStr(bool colorFlag) const {
            std::ostringstream oss;
            oss << std::hex << std::setfill('0');
            for (int i=0; i<_value.size(); ++i) {
                if (i>0) oss << ' ';
                oss << std::setw(2) << (int)_value.at(i);
            }
            return Color::color(DisplayColorType_t::AMQP_TYPE_BINARY, oss.str());
        }


        AmqpString::AmqpString(): PrimitiveType(), _value() {}
        AmqpString::AmqpString(const char* name): PrimitiveType(name), _value() {}
        AmqpString::~AmqpString() {}
        std::string AmqpString::valueStr(bool colorFlag) const {
            std::stringstream oss;
            oss << "\"" << _value << "\"";
            return Color::color(DisplayColorType_t::AMQP_TYPE_STRING, oss.str());
        }


        AmqpSymbol::AmqpSymbol(): PrimitiveType(), _value() {}
        AmqpSymbol::AmqpSymbol(const char* name): PrimitiveType(name), _value() {}
        AmqpSymbol::~AmqpSymbol() {}
        std::string AmqpSymbol::valueStr(bool colorFlag) const {
            std::stringstream oss;
            oss << "\"" << _value << "\"";
            return Color::color(DisplayColorType_t::AMQP_TYPE_SYMBOL, oss.str());
        }


        CompoundType::CompoundType(): PrimitiveType() {}
        CompoundType::CompoundType(const char* name): PrimitiveType(name) {}
        CompoundType::~CompoundType() {}
        // static
        void CompoundType::stringAppendHandler(std::ostringstream& oss, Type* ptr, std::size_t margin, bool nameFlag, bool colorFlag) {
            CompositeType* compositePtr(dynamic_cast<CompositeType*>(ptr));
            if (compositePtr) {
                compositePtr->appendString(oss, margin, true, colorFlag);
            } else {
                CompoundType* compoundPtr(dynamic_cast<CompoundType*>(ptr));
                if (compoundPtr) {
                    compoundPtr->appendString(oss, margin, true, colorFlag);
                } else {
                    oss << (nameFlag ? ptr->nameTypeValueStr(colorFlag) : ptr->typeValueStr(colorFlag));
                }
            }
        }


        AmqpList::AmqpList(): CompoundType(), _value() {}
        AmqpList::AmqpList(const char* name): CompoundType(name), _value() {}
        AmqpList::~AmqpList() {
            for (AmqpListItr_t i=_value.begin(); i!=_value.end(); ++i) {
                delete *i;
            }
            _value.clear();
        }
        std::string AmqpList::valueStr(bool colorFlag) const {
            std::ostringstream oss;
            for (AmqpListCitr_t i=_value.cbegin(); i!=_value.cend(); ++i) {
                if (i!=_value.cbegin()) oss << ", ";
                oss << (*i)->typeValueStr(colorFlag);
            }
            return oss.str();
        }
        void AmqpList::appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin, bool colorFlag) const {
            std::string m(margin + 3, ' ');
            if (!ignoreFirstMargin) oss << "\n" << m;
            oss << ": [";
            for (AmqpListCitr_t i=_value.cbegin(); i<_value.cend(); ++i) {
                if (i!=_value.cbegin()) oss << "\n" << m;
                stringAppendHandler(oss, *i, margin + 3, true, colorFlag);
            }
            oss << "]";
        }
        void AmqpList::validate(const FieldTypeList_t& fieldTypeList, addErrorFp errorHandler, AmqpBlock* errorHandlerInstance) {
            // 1. Check field list not larger than fieldTypeList
            if (_value.size() > fieldTypeList.size()) {
                (errorHandlerInstance->*errorHandler)(new AmqpValidationError(ErrorSeverity_t::ERROR,
                                errorHandlerInstance->packetNum(),
                                errorHandlerInstance->dataOffset(),
                                MSG("FieldTypeList size mismatch: FieldTypeList size=" << fieldTypeList.size() << ", fieldList size=" << _value.size())));
            }

            // 2. Iterate through fields
            int index = 0;
            for (AmqpListCitr_t i=_value.cbegin(); i!=_value.cend(); ++i) {
                const FieldType& fieldType = fieldTypeList[index++];

                // 3. Check that null not present for required field
                if (fieldType._mandatoryFlag && (*i)->isNull()) {
                    (errorHandlerInstance->*errorHandler)(new AmqpValidationError(errorHandlerInstance->packetNum(),
                                    errorHandlerInstance->dataOffset(),
                                    MSG("Mandatory field \"" << (*i)->name() << "\" is null")));
                }

                // 4. Check that fieldTypeList requires is met in value (if not null)
                if (!(*i)->isNull()) {
                    for (AmqpProvidesRequiresListCitr_t j=fieldType._requiresList.cbegin(); j!=fieldType._requiresList.cend(); ++j) {
                        if (!(*i)->provides(*j, (*i)->providesList())) {
                            (errorHandlerInstance->*errorHandler)(new AmqpValidationError(errorHandlerInstance->packetNum(),
                                            errorHandlerInstance->dataOffset(),
                                            MSG("Field \"" << fieldType._fieldName << "\" requires \"" << providesRequiresNames[*j]
                                                << "\", but is not provided by value type \"" << (*i)->typeStr() << "\"")));
                        }
                    }
                }

                // 5. Check that fieldTypeList multiple has array type
                if (!(*i)->isNull() && fieldType._multipleFlag) {
                    if (std::strncmp((*i)->typeStr(), "array", 5) != 0) {
                        (errorHandlerInstance->*errorHandler)(new AmqpValidationError(errorHandlerInstance->packetNum(),
                                        errorHandlerInstance->dataOffset(),
                                        MSG("Field \"" << fieldType._fieldName << "\" allows multiple values, but is not of type \"array\"")));
                    }
                }

                // 6. Validate value itself
                (*i)->validate(errorHandler, errorHandlerInstance);

            }
        }


        AmqpMap::AmqpMap(): CompoundType(), _value() {}
        AmqpMap::AmqpMap(const char* name): CompoundType(name), _value() {}
        AmqpMap::~AmqpMap() {
            while (!_value.empty()) {
                AmqpMapItr_t i = _value.begin();
                Type* k = i->first;
                Type* v = i->second;
                _value.erase(i);
                delete k;
                delete v;
            }
        }
        std::string AmqpMap::valueStr(bool colorFlag) const {
            std::ostringstream oss;
            for (AmqpMapCitr_t i=_value.cbegin(); i!=_value.cend(); ++i) {
                if (i!=_value.cbegin()) oss << ", ";
                oss << "{" << i->first->typeValueStr(colorFlag) << ": " << i->second->typeValueStr(colorFlag) << "}";
            }
            return oss.str();
        }
        void AmqpMap::appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin, bool colorFlag) const {
            std::string n(name());
            std::string t(typeStr());
            std::size_t l(margin + n.size() + t.size() + 4);
            std::string m(l, ' ');
            if (!ignoreFirstMargin) oss << "\n" << m;
            oss << n << ":" << t << ": {";
            for (AmqpMapCitr_t i=_value.cbegin(); i!=_value.cend(); ++i) {
                if (i!=_value.cbegin()) oss << "\n" << m;
                oss << "{";
                CompoundType* kPtr(dynamic_cast<CompoundType*>(i->first));
                if (kPtr != nullptr) {
                    // key is a compound type
                    kPtr->appendString(oss, l, ignoreFirstMargin, colorFlag);
                    oss << "\n" << m << ":";
                } else {
                    std::string kStr(i->first->typeValueStr(colorFlag));
                    oss << kStr << ": ";
                    l += kStr.length() + 2;
                }
                stringAppendHandler(oss, i->second, l, false, colorFlag); // print value
                oss << "}";
            }
            oss << "}";
        }


        AmqpArray::AmqpArray(): CompoundType(), _value() {}
        AmqpArray::AmqpArray(const char* name): CompoundType(name), _value() {}
        AmqpArray::~AmqpArray() {
            for (AmqpArrayItr_t i=_value.begin(); i!=_value.end(); ++i) {
                delete *i;
            }
            _value.clear();
        }
        std::string AmqpArray::valueStr(bool colorFlag) const {
            std::ostringstream oss;
            for (AmqpArrayCitr_t i=_value.cbegin(); i!=_value.cend(); ++i) {
                if (i != _value.cbegin()) oss << ", ";
                oss << (*i)->typeValueStr(colorFlag);
            }
            return oss.str();
        }
        void AmqpArray::appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin, bool colorFlag) const {
            std::string n(name());
            std::string t(typeStr());
            std::size_t l(margin + n.length() + t.length() + 4);
            std::string m(l, ' ');
            oss << n << ":" << t << ": [";
            for (AmqpArrayCitr_t i=_value.cbegin(); i!=_value.cend(); ++i) {
                if (i!=_value.cbegin()) oss << "\n" << m;
                stringAppendHandler(oss, *i, l, false, colorFlag);
            }
            oss << "]";
        }


        AmqpRole::AmqpRole(AmqpRole_t v): AmqpBoolean((bool)v) {}
        AmqpRole::AmqpRole(AmqpRole_t v, const char* name): AmqpBoolean((bool)v, name) {}
        AmqpRole:: ~AmqpRole() {}
        std::string AmqpRole::valueStr(bool colorFlag) const {
            return Color::color(DisplayColorType_t::AMQP_RESTRICTED_LIST, s_choiceNames.at((AmqpRole_t)_value));
        }
        // static
        std::map<AmqpRole_t, const char*> AmqpRole::s_choiceNames = {
            {AmqpRole_t::SENDER, "sender"},
            {AmqpRole_t::RECEIVER, "receiver"}
        };


        AmqpSenderSettleMode::AmqpSenderSettleMode(AmqpSenderSettleMode_t v): AmqpUbyte((uint8_t)v) {}
        AmqpSenderSettleMode::AmqpSenderSettleMode(AmqpSenderSettleMode_t v, const char* name): AmqpUbyte((uint8_t)v, name) {}
        AmqpSenderSettleMode::~AmqpSenderSettleMode() {}
        std::string AmqpSenderSettleMode::valueStr(bool colorFlag) const {
            return Color::color(DisplayColorType_t::AMQP_RESTRICTED_LIST, s_choiceNames.at((AmqpSenderSettleMode_t)_value));
        }
        // static
        std::map<AmqpSenderSettleMode_t, const char*> AmqpSenderSettleMode::s_choiceNames = {
            {AmqpSenderSettleMode_t::UNSETTLED, "unsettled"},
            {AmqpSenderSettleMode_t::SETTLED, "settled"},
            {AmqpSenderSettleMode_t::MIXED, "mixed"}
        };


        AmqpReceiverSettleMode::AmqpReceiverSettleMode(AmqpReceiverSettleMode_t v): AmqpUbyte((uint8_t)v) {}
        AmqpReceiverSettleMode::AmqpReceiverSettleMode(AmqpReceiverSettleMode_t v, const char* name): AmqpUbyte((uint8_t)v, name) {}
        AmqpReceiverSettleMode::~AmqpReceiverSettleMode() {}
        std::string AmqpReceiverSettleMode::valueStr(bool colorFlag) const {
            return Color::color(DisplayColorType_t::AMQP_RESTRICTED_LIST,s_choiceNames.at((AmqpReceiverSettleMode_t)_value));
        }
        // static
        std::map<AmqpReceiverSettleMode_t, const char*> AmqpReceiverSettleMode::s_choiceNames = {
            {AmqpReceiverSettleMode_t::FIRST, "first"},
            {AmqpReceiverSettleMode_t::SECOND, "second"}
        };


        AmqpHandle::AmqpHandle(uint32_t v): AmqpUint(v) {}
        AmqpHandle::AmqpHandle(uint32_t v, const char* name): AmqpUint(v, name) {}
        AmqpHandle::~AmqpHandle() {}


        AmqpSeconds::AmqpSeconds(uint32_t v): AmqpUint(v) {}
        AmqpSeconds::AmqpSeconds(uint32_t v, const char* name): AmqpUint(v, name) {}
        AmqpSeconds::~AmqpSeconds() {}


        AmqpMilliseconds::AmqpMilliseconds(uint32_t v): AmqpUint(v) {}
        AmqpMilliseconds::AmqpMilliseconds(uint32_t v, const char* name): AmqpUint(v, name) {}
        AmqpMilliseconds::~AmqpMilliseconds() {}


        AmqpDeliveryTag::AmqpDeliveryTag(): AmqpBinary() {}
        AmqpDeliveryTag::AmqpDeliveryTag(const char* name): AmqpBinary(name) {}
        AmqpDeliveryTag::~AmqpDeliveryTag() {}


        AmqpSequenceNum::AmqpSequenceNum(uint32_t v): AmqpUint(v) {}
        AmqpSequenceNum::AmqpSequenceNum(uint32_t v, const char* name): AmqpUint(v, name) {}
        AmqpSequenceNum::~AmqpSequenceNum() {}


        AmqpDeliveryNum::AmqpDeliveryNum(uint32_t v): AmqpSequenceNum(v) {}
        AmqpDeliveryNum::AmqpDeliveryNum(uint32_t v, const char* name): AmqpSequenceNum(v, name) {}
        AmqpDeliveryNum::~AmqpDeliveryNum() {}


        AmqpTransferNum::AmqpTransferNum(uint32_t v): AmqpSequenceNum(v) {}
        AmqpTransferNum::AmqpTransferNum(uint32_t v, const char* name): AmqpSequenceNum(v, name) {}
        AmqpTransferNum::~AmqpTransferNum() {}


        AmqpMessageFormat::AmqpMessageFormat(uint32_t v): AmqpUint(v) {}
        AmqpMessageFormat::AmqpMessageFormat(uint32_t v, const char* name): AmqpUint(v, name) {}
        AmqpMessageFormat::~AmqpMessageFormat() {}
        std::string AmqpMessageFormat::valueStr(bool colorFlag) const {
            std::ostringstream oss;
            oss << typeStr() << "fmt=0x" << std::hex << std::setfill('0') << std::setw(6) << getFormat()
                << " ver=0x" << std::setw(2) << (int)getVersion() << ")";
            return Color::color(DisplayColorType_t::AMQP_RESTRICTED_LIST, oss.str());
        }


        AmqpLanguageTag::AmqpLanguageTag(): AmqpSymbol() {}
        AmqpLanguageTag::AmqpLanguageTag(const char* name): AmqpSymbol(name) {}
        AmqpLanguageTag::~AmqpLanguageTag() {}


        AmqpFields::AmqpFields(): AmqpMap() {}
        AmqpFields::AmqpFields(const char* name): AmqpMap(name) {}
        AmqpFields::~AmqpFields() {}

        AmqpError::AmqpError(const std::string& v): AmqpSymbol() {
            checkAssignValue(v);
        }
        AmqpError::AmqpError(const std::string& v, const char* name): AmqpSymbol(name) {
            checkAssignValue(v);
        }
        AmqpError::~AmqpError() {}
        void AmqpError::checkAssignValue(const std::string& v) {
            bool found = false;
            for (AmqpErrorListCitr_t i=s_amqpErrors.cbegin(); i!=s_amqpErrors.cend(); ++i) {
                if (v.compare(*i) == 0) {
                    found = true;
                    break;
                }
            }
            if (!found) throw amqpAnalyze::Error(MSG("Illegal value for AmqpError: \"" << v << "\""));
            _value.assign(v);
        }
        // static
        AmqpProvidesRequiresList_t AmqpError::s_providesList = {
            AmqpRequiresProvides_t::ERROR_CONDITION
        };
        // static
        AmqpErrorList_t AmqpError::s_amqpErrors = {
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


        AmqpConnectionError::AmqpConnectionError(const std::string& v): AmqpSymbol() {
            checkAssignValue(v);
        }
        AmqpConnectionError::AmqpConnectionError(const std::string& v, const char* name): AmqpSymbol(name) {
            checkAssignValue(v);
        }
        AmqpConnectionError::~AmqpConnectionError() {}
        void AmqpConnectionError::checkAssignValue(const std::string& v) {
            bool found = false;
            for (AmqpConnectionErrorListCitr_t i=s_amqpConnectionErrors.cbegin(); i!=s_amqpConnectionErrors.cend(); ++i) {
                if (v.compare(*i) == 0) {
                    found = true;
                    break;
                }
            }
            if (!found) throw amqpAnalyze::Error(MSG("Illegal value for AmqpConnectionError: \"" << v << "\""));
            _value.assign(v);
        }
        // static
        AmqpProvidesRequiresList_t AmqpConnectionError::s_providesList = {
            AmqpRequiresProvides_t::ERROR_CONDITION
        };
        // static
        AmqpConnectionErrorList_t AmqpConnectionError::s_amqpConnectionErrors = {
            "amqp:connection:forced",
            "amqp:connection:framing-error",
            "amqp:connection:redirect"
        };


        AmqpSessionError::AmqpSessionError(const std::string& v): AmqpSymbol() {
            checkAssignValue(v);
        }
        AmqpSessionError::AmqpSessionError(const std::string& v, const char* name): AmqpSymbol(name) {
            checkAssignValue(v);
        }
        AmqpSessionError::~AmqpSessionError() {}
        void AmqpSessionError::checkAssignValue(const std::string& v) {
            bool found = false;
            for (AmqpSessionErrorListCitr_t i=s_amqpSessionErrors.cbegin(); i!=s_amqpSessionErrors.cend(); ++i) {
                if (v.compare(*i) == 0) {
                    found = true;
                    break;
                }
            }
            if (!found) throw amqpAnalyze::Error(MSG("Illegal value for AmqpSessionError: \"" << v << "\""));
            _value.assign(v);
        }
        // static
        AmqpProvidesRequiresList_t AmqpSessionError::s_providesList = {
            AmqpRequiresProvides_t::ERROR_CONDITION
        };
        // static
        AmqpSessionErrorList_t AmqpSessionError::s_amqpSessionErrors = {
            "amqp:session:window-violation",
            "amqp:session:errant-link",
            "amqp:session:handle-in-use",
            "amqp:session:unattached-handle"
        };


        AmqpLinkError::AmqpLinkError(const std::string& v): AmqpSymbol() {
            checkAssignValue(v);
        }
        AmqpLinkError::AmqpLinkError(const std::string& v, const char* name): AmqpSymbol(name) {
            checkAssignValue(v);
        }
        AmqpLinkError::~AmqpLinkError() {}
        void AmqpLinkError::checkAssignValue(const std::string& v) {
            bool found = false;
            for (AmqpLinkErrorListCitr_t i=s_amqpLinkErrors.cbegin(); i!=s_amqpLinkErrors.cend(); ++i) {
                if (v.compare(*i) == 0) {
                    found = true;
                    break;
                }
            }
            if (!found) throw amqpAnalyze::Error(MSG("Illegal value for AmqpLinkError: \"" << v << "\""));
            _value.assign(v);
        }
        // static
        AmqpProvidesRequiresList_t AmqpLinkError::s_providesList = {
            AmqpRequiresProvides_t::ERROR_CONDITION
        };
        // static
        AmqpLinkErrorList_t AmqpLinkError::s_amqpLinkErrors = {
            "amqp:link:detach-forced",
            "amqp:link:transfer-limit-exceeded",
            "amqp:link:message-size-exceeded",
            "amqp:link:redirect",
            "amqp:link:stolen"
        };


        AmqpAnnotations::AmqpAnnotations(): AmqpMap() {}
        AmqpAnnotations::AmqpAnnotations(const char* name): AmqpMap(name) {}
        AmqpAnnotations::~AmqpAnnotations() {}


        AmqpMessageIdUlong::AmqpMessageIdUlong(uint64_t v): AmqpUlong((uint64_t)v) {}
        AmqpMessageIdUlong::AmqpMessageIdUlong(uint64_t v, const char* name): AmqpUlong((uint64_t)v, name) {}
        AmqpMessageIdUlong::~AmqpMessageIdUlong() {}
        // static
        AmqpProvidesRequiresList_t AmqpMessageIdUlong::s_providesList = {
            AmqpRequiresProvides_t::MESSAGE_ID
        };


        AmqpMessageIdUuid::AmqpMessageIdUuid(): AmqpUuid() {}
        AmqpMessageIdUuid::AmqpMessageIdUuid(const char* name): AmqpUuid(name) {}
        AmqpMessageIdUuid::~AmqpMessageIdUuid() {}
        // static
        AmqpProvidesRequiresList_t AmqpMessageIdUuid::s_providesList = {
            AmqpRequiresProvides_t::MESSAGE_ID
        };


        AmqpMessageIdBinary::AmqpMessageIdBinary(): AmqpBinary() {}
        AmqpMessageIdBinary::AmqpMessageIdBinary(const char* name): AmqpBinary(name) {}
        AmqpMessageIdBinary::~AmqpMessageIdBinary() {}
        // static
        AmqpProvidesRequiresList_t AmqpMessageIdBinary::s_providesList = {
            AmqpRequiresProvides_t::MESSAGE_ID
        };


        AmqpMessageIdString::AmqpMessageIdString(): AmqpString() {}
        AmqpMessageIdString::AmqpMessageIdString(const char* name): AmqpString(name) {}
        AmqpMessageIdString::~AmqpMessageIdString() {}
        // static
        AmqpProvidesRequiresList_t AmqpMessageIdString::s_providesList = {
            AmqpRequiresProvides_t::MESSAGE_ID
        };


        AmqpAddress::AmqpAddress(): AmqpString() {}
        AmqpAddress::AmqpAddress(const char* name): AmqpString(name) {}
        AmqpAddress::~AmqpAddress() {}
        // static
        AmqpProvidesRequiresList_t AmqpAddress::s_providesList = {
            AmqpRequiresProvides_t::ADDRESS
        };


        AmqpTerminusDurability::AmqpTerminusDurability(AmqpTerminusDurability_t v): AmqpUint((uint32_t)v) {}
        AmqpTerminusDurability::AmqpTerminusDurability(AmqpTerminusDurability_t v, const char* name): AmqpUint((uint32_t)v, name) {}
        AmqpTerminusDurability::~AmqpTerminusDurability() {}
        std::string AmqpTerminusDurability::valueStr(bool colorFlag) const {
            return Color::color(DisplayColorType_t::AMQP_RESTRICTED_LIST, s_choiceNames.at((AmqpTerminusDurability_t)_value));
        }
        // static
        std::map<AmqpTerminusDurability_t, const char*> AmqpTerminusDurability::s_choiceNames = {
            {AmqpTerminusDurability_t::NONE, "none"},
            {AmqpTerminusDurability_t::CONFIGURATION, "configuration"},
            {AmqpTerminusDurability_t::UNSETTLED_STATE, "unsettled-state"}
        };


        AmqpTerminusExpiryPolicy::AmqpTerminusExpiryPolicy(const std::string& v): AmqpSymbol() {
            checkAssignValue(v);
        }
        AmqpTerminusExpiryPolicy::AmqpTerminusExpiryPolicy(const std::string& v, const char* name): AmqpSymbol(name) {
            checkAssignValue(v);
        }
        AmqpTerminusExpiryPolicy::~AmqpTerminusExpiryPolicy() {}
        void AmqpTerminusExpiryPolicy::checkAssignValue(const std::string& v) {
            bool found = false;
            for (AmqpTerminusExpiryPolicyListCitr_t i=s_terminusExpiryPolicyList.cbegin(); i!=s_terminusExpiryPolicyList.cend(); ++i) {
                if (v.compare(*i) == 0) {
                    found = true;
                    break;
                }
            }
            if (!found) throw amqpAnalyze::Error(MSG("Illegal value for AmqpTerminusExpiryPolicy: \"" << v << "\""));
            _value.assign(v);
        }
        // static
        AmqpTerminusExpiryPolicyList_t AmqpTerminusExpiryPolicy::s_terminusExpiryPolicyList = {
             "link-detach",
             "session-end",
             "connection-close",
             "never"
        };


        AmqpStandardDistributionMode::AmqpStandardDistributionMode(const std::string& v): AmqpSymbol() {
            checkAssignValue(v);
        }
        AmqpStandardDistributionMode::AmqpStandardDistributionMode(const std::string& v, const char* name): AmqpSymbol(name) {
            checkAssignValue(v);
        }
        AmqpStandardDistributionMode::~AmqpStandardDistributionMode() {}
        void AmqpStandardDistributionMode::checkAssignValue(const std::string& v) {
            bool found = false;
            for (AmqpStdDistModeListCitr_t i=s_StandardDistributionModeList.cbegin(); i!=s_StandardDistributionModeList.cend(); ++i) {
                if (v.compare(*i) == 0) {
                    found = true;
                    break;
                }
            }
            if (!found) throw amqpAnalyze::Error(MSG("Illegal value for AmqpStandardDistributionMode: \"" << v << "\""));
            _value.assign(v);
        }
        // static
        AmqpStdDistModeList_t AmqpStandardDistributionMode::s_StandardDistributionModeList {
            "move",
            "copy"
        };
        // static
        AmqpProvidesRequiresList_t AmqpStandardDistributionMode::s_providesList= {
            AmqpRequiresProvides_t::DISTRIBUTION_MODE
        };


        AmqpFilterSet::AmqpFilterSet(): AmqpMap() {}
        AmqpFilterSet::AmqpFilterSet(const char* name): AmqpMap(name) {}
        AmqpFilterSet::~AmqpFilterSet() {}


        AmqpNodeProperties::AmqpNodeProperties(): AmqpFields() {}
        AmqpNodeProperties::AmqpNodeProperties(const char* name): AmqpFields(name) {}
        AmqpNodeProperties::~AmqpNodeProperties() {}


        AmqpTransactionId::AmqpTransactionId(): AmqpBinary() {}
        AmqpTransactionId::AmqpTransactionId(const char* name): AmqpBinary(name) {}
        AmqpTransactionId::~AmqpTransactionId() {}
        // static
        AmqpProvidesRequiresList_t AmqpTransactionId::s_providesList = {
        	AmqpRequiresProvides_t::TXN_ID
        };

        AmqpTransactionalCapability::AmqpTransactionalCapability(const std::string& v): AmqpSymbol() {
            checkAssignValue(v);
        }
        AmqpTransactionalCapability::AmqpTransactionalCapability(const std::string& v, const char* name): AmqpSymbol(name) {
            checkAssignValue(v);
        }
        AmqpTransactionalCapability::~AmqpTransactionalCapability() {}
        void AmqpTransactionalCapability::checkAssignValue(const std::string& v) {
            bool found = false;
            for (AmqpTransactionCapabilityListCitr_t i=s_transactionCapabilityList.cbegin(); i!=s_transactionCapabilityList.cend(); ++i) {
                if (v.compare(*i) == 0) {
                    found = true;
                    break;
                }
            }
            if (!found) throw amqpAnalyze::Error(MSG("Illegal value for TransactionalCapability: \"" << v << "\""));
            _value.assign(v);
        }
        // static
        AmqpTransactionCapabilityList_t AmqpTransactionalCapability::s_transactionCapabilityList = {
            "local-transactions",
            "distributed-transactions",
            "promotable-transactions",
            "multi-txns-per-ssn",
            "multi-ssns-per-txn"
        };
        // static
        AmqpProvidesRequiresList_t AmqpTransactionalCapability::s_providesList = {
            AmqpRequiresProvides_t::TXN_CAPABILITY
        };


        AmqpTransactionError::AmqpTransactionError(const std::string& v): AmqpSymbol() {
            bool found = false;
            for (AmqpTransactionErrorListCitr_t i=s_transactionErrorList.cbegin(); i!=s_transactionErrorList.cend(); ++i) {
                if (v.compare(*i) == 0) {
                    found = true;
                    break;
                }
            }
            if (!found) throw amqpAnalyze::Error(MSG("Illegal value for TransactionalCapability: \"" << v << "\""));
            _value.assign(v);
        }
        AmqpTransactionError::AmqpTransactionError(const std::string& v, const char* name): AmqpSymbol(name) {
            bool found = false;
            for (AmqpTransactionErrorListCitr_t i=s_transactionErrorList.cbegin(); i!=s_transactionErrorList.cend(); ++i) {
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
        AmqpTransactionErrorList_t AmqpTransactionError::s_transactionErrorList = {
            "unknown-id",
            "transaction-rollback",
            "transaction-timeout"
        };
        // static
        AmqpProvidesRequiresList_t AmqpTransactionError::s_providesList = {
            AmqpRequiresProvides_t::ERROR_CONDITION
        };


        AmqpSaslCode::AmqpSaslCode(AmqpSaslCode_t v): AmqpUbyte((uint8_t)v) {}
        AmqpSaslCode::AmqpSaslCode(AmqpSaslCode_t v, const char* name): AmqpUbyte((uint8_t)v, name) {}
        AmqpSaslCode::~AmqpSaslCode() {}
        std::string AmqpSaslCode::valueStr(bool colorFlag) const {
            return Color::color(DisplayColorType_t::AMQP_RESTRICTED_LIST, s_choiceNames.at((AmqpSaslCode_t)_value));
        }
        // static
        std::map<AmqpSaslCode_t, const char*> AmqpSaslCode::s_choiceNames = {
            {AmqpSaslCode_t::OK, "ok"},
            {AmqpSaslCode_t::AUTH, "auth"},
            {AmqpSaslCode_t::SYS, "sys"},
            {AmqpSaslCode_t::SYS_PERM, "sys-perm"},
            {AmqpSaslCode_t::SYS_TEMP, "sys-temp"}
        };



        CompositeType::CompositeType(AmqpList* fieldListPtr): Type(), _fieldListPtr(fieldListPtr) {}
        CompositeType::CompositeType(AmqpList* fieldListPtr, const char* name): Type(name), _fieldListPtr(fieldListPtr) {}
        CompositeType::~CompositeType() {
            if (_fieldListPtr != nullptr) {
                delete _fieldListPtr;
                _fieldListPtr = nullptr;
            }
        }
        std::ostringstream& CompositeType::appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin, bool colorFlag) const {
            std::string n(name());
            std::string t(typeStr());
            std::size_t l(margin + n.length() + t.length() + 1);
            if (!ignoreFirstMargin) oss << "\n" << std::string(l, ' ');
            oss << n << ":" << t;
            if (_fieldListPtr != nullptr) {
                _fieldListPtr->appendString(oss, l, true, colorFlag);
            }
            return oss;
        }
        void CompositeType::validate(addErrorFp errorHandler, AmqpBlock* errorHandlerInstance) {
            if (_fieldListPtr != nullptr) {
                _fieldListPtr->validate(fieldTypeList(), errorHandler, errorHandlerInstance);
            }
        }
        std::string CompositeType::valueStr(bool colorFlag) const {
            return _fieldListPtr->valueStr(colorFlag);
        }
        // static
        std::map<AmqpCompositeType_t, const char*> CompositeType::s_amqpCompositeTypeNames = {
                        {AmqpCompositeType_t::ERROR, "error"},
                        {AmqpCompositeType_t::RECEIVED, "received"},
                        {AmqpCompositeType_t::ACCEPTED, "accepted"},
                        {AmqpCompositeType_t::REJECTED, "rejected"},
                        {AmqpCompositeType_t::RELEASED, "released"},
                        {AmqpCompositeType_t::MODIFIED, "modified"},
                        {AmqpCompositeType_t::SOURCE, "source"},
                        {AmqpCompositeType_t::TARGET, "target"},
                        {AmqpCompositeType_t::DELETE_ON_CLOSE, "delete-on-close"},
                        {AmqpCompositeType_t::DELETE_ON_NO_LINKS, "delete-on-no-links"},
                        {AmqpCompositeType_t::DELETE_ON_NO_MESSAGES, "delete-on-no-messages"},
                        {AmqpCompositeType_t::DELETE_ON_NO_LINKS_OR_MESSAGES, "delete-on-no-links-or-messages"},
                        {AmqpCompositeType_t::COORDINATOR, "coordinator"},
                        {AmqpCompositeType_t::DECLARE, "declare"},
                        {AmqpCompositeType_t::DISCHARGE, "discharge"},
                        {AmqpCompositeType_t::DECLARED, "declared"},
                        {AmqpCompositeType_t::TRANSACTIONAL_STATE, "transactional-state"},
                        {AmqpCompositeType_t::SASL_MECHANISMS, "sasl-mechanisms"},
                        {AmqpCompositeType_t::SASL_INIT, "sasl-init"},
                        {AmqpCompositeType_t::SASL_CHALLENGE, "sasl-challenge"},
                        {AmqpCompositeType_t::SASL_RESPONSE, "sasl-response"},
                        {AmqpCompositeType_t::SASL_OUTCOME, "sasl-outcome"}
        };
        // static
        AmqpProvidesRequiresList_t CompositeType::s_providesList = {};


        AmqpErrorRecord::AmqpErrorRecord(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpErrorRecord::AmqpErrorRecord(AmqpList* fieldList, const char* name): CompositeType(fieldList, name) {}
        AmqpErrorRecord::~AmqpErrorRecord() {}
        // static
        const FieldTypeList_t AmqpErrorRecord::s_fieldTypeList = {
            FieldType("condition", AmqpPrimitiveType_t::SYMBOL_TYPE, true, false, {AmqpRequiresProvides_t::ERROR_CONDITION}),
            FieldType("description", AmqpPrimitiveType_t::STRING_TYPE, false, false),
            FieldType("info", AmqpPrimitiveType_t::FIELDS_TYPE, false, false)
        };


        AmqpReceived::AmqpReceived(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpReceived::AmqpReceived(AmqpList* fieldList, const char* name): CompositeType(fieldList, name) {}
        AmqpReceived::~AmqpReceived() {}
        // static
        const FieldTypeList_t AmqpReceived::s_fieldTypeList = {
            FieldType("section-number", AmqpPrimitiveType_t::UINT_TYPE, true, false),
            FieldType("section-offset", AmqpPrimitiveType_t::ULONG_TYPE, true, false)
        };
        // static
        AmqpProvidesRequiresList_t AmqpReceived::s_providesList = {
        	AmqpRequiresProvides_t::DELIVERY_STATE
        };

        AmqpAccepted::AmqpAccepted(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpAccepted::AmqpAccepted(AmqpList* fieldList, const char* name): CompositeType(fieldList, name) {}
        AmqpAccepted::~AmqpAccepted() {}
        // static
        const FieldTypeList_t AmqpAccepted::s_fieldTypeList = {};
        // static
        AmqpProvidesRequiresList_t AmqpAccepted::s_providesList = {
        	AmqpRequiresProvides_t::DELIVERY_STATE,
        	AmqpRequiresProvides_t::OUTCOME
        };


        AmqpRejected::AmqpRejected(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpRejected::AmqpRejected(AmqpList* fieldList, const char* name): CompositeType(fieldList, name) {}
        AmqpRejected::~AmqpRejected() {}
        // static
        const FieldTypeList_t AmqpRejected::s_fieldTypeList = {
            FieldType("error", AmqpPrimitiveType_t::AMQP_ERROR_TYPE, false, false)
        };
        // static
        AmqpProvidesRequiresList_t AmqpRejected::s_providesList = {
        	AmqpRequiresProvides_t::DELIVERY_STATE,
        	AmqpRequiresProvides_t::OUTCOME
        };


        AmqpReleased::AmqpReleased(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpReleased::AmqpReleased(AmqpList* fieldList, const char* name): CompositeType(fieldList, name) {}
        AmqpReleased::~AmqpReleased() {}
        // static
        const FieldTypeList_t AmqpReleased::s_fieldTypeList = {};
        // static
        AmqpProvidesRequiresList_t AmqpReleased::s_providesList = {
        	AmqpRequiresProvides_t::DELIVERY_STATE,
        	AmqpRequiresProvides_t::OUTCOME
        };


        AmqpModified::AmqpModified(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpModified::AmqpModified(AmqpList* fieldList, const char* name): CompositeType(fieldList, name) {}
        AmqpModified::~AmqpModified() {}
        // static
        const FieldTypeList_t AmqpModified::s_fieldTypeList = {
            FieldType("delivery-failed", AmqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType("undeliverable-here", AmqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType("message-annotations", AmqpPrimitiveType_t::FIELDS_TYPE, false, false)
        };
        // static
        AmqpProvidesRequiresList_t AmqpModified::s_providesList = {
        	AmqpRequiresProvides_t::DELIVERY_STATE,
        	AmqpRequiresProvides_t::OUTCOME
        };


        AmqpSource::AmqpSource(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpSource::AmqpSource(AmqpList* fieldList, const char* name): CompositeType(fieldList, name) {}
        AmqpSource::~AmqpSource() {}
        // static
        const FieldTypeList_t AmqpSource::s_fieldTypeList = {
            FieldType("address", "*", false, false, {AmqpRequiresProvides_t::ADDRESS}),
            FieldType("durable", AmqpPrimitiveType_t::TERMINUS_DURABILITY_TYPE, false, false),
            FieldType("expiry-policy", AmqpPrimitiveType_t::TERMINUS_EXPIRY_POLICY_TYPE, false, false),
            FieldType("timeout", AmqpPrimitiveType_t::SECONDS_TYPE, false, false),
            FieldType("dynamic", AmqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType("dynamic-node-properties", AmqpPrimitiveType_t::NODE_PROPERTIES_TYPE, false, false),
            FieldType("distribution-mode", AmqpPrimitiveType_t::SYMBOL_TYPE, false, false, {AmqpRequiresProvides_t::DISTRIBUTION_MODE}),
            FieldType("filter", AmqpPrimitiveType_t::FILTER_SET_TYPE, false, false),
            FieldType("default-outcome", "*", false, false, {AmqpRequiresProvides_t::OUTCOME}),
            FieldType("outcomes", AmqpPrimitiveType_t::SYMBOL_TYPE, false, true),
            FieldType("capabilities", AmqpPrimitiveType_t::SYMBOL_TYPE, false, true)
        };
        // static
        AmqpProvidesRequiresList_t AmqpSource::s_providesList = {
        	AmqpRequiresProvides_t::SOURCE
        };


        AmqpTarget::AmqpTarget(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpTarget::AmqpTarget(AmqpList* fieldList, const char* name): CompositeType(fieldList, name) {}
        AmqpTarget::~AmqpTarget() {}
        // static
        const FieldTypeList_t AmqpTarget::s_fieldTypeList = {
            FieldType("address", "*", false, false, {AmqpRequiresProvides_t::ADDRESS}),
            FieldType("durable", AmqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType("expiry-policy", AmqpPrimitiveType_t::TERMINUS_EXPIRY_POLICY_TYPE, false, false),
            FieldType("timeout", AmqpPrimitiveType_t::SECONDS_TYPE, false, false),
            FieldType("dynamic", AmqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType("dynamic-node-properties", AmqpPrimitiveType_t::NODE_PROPERTIES_TYPE, false, false),
            FieldType("capabilities", AmqpPrimitiveType_t::SYMBOL_TYPE, false, true)
        };
        // static
        AmqpProvidesRequiresList_t AmqpTarget::s_providesList = {
        	AmqpRequiresProvides_t::TARGET
        };

        AmqpDeleteOnClose::AmqpDeleteOnClose(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpDeleteOnClose::AmqpDeleteOnClose(AmqpList* fieldList, const char* name): CompositeType(fieldList, name) {}
        AmqpDeleteOnClose::~AmqpDeleteOnClose() {}
        // static
        const FieldTypeList_t AmqpDeleteOnClose::s_fieldTypeList = {};


        AmqpDeleteOnNoLinks::AmqpDeleteOnNoLinks(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpDeleteOnNoLinks::AmqpDeleteOnNoLinks(AmqpList* fieldList, const char* name): CompositeType(fieldList, name) {}
        AmqpDeleteOnNoLinks::~AmqpDeleteOnNoLinks() {}
        // static
        const FieldTypeList_t AmqpDeleteOnNoLinks::s_fieldTypeList = {};


        AmqpDeleteOnNoMessages::AmqpDeleteOnNoMessages(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpDeleteOnNoMessages::AmqpDeleteOnNoMessages(AmqpList* fieldList, const char* name): CompositeType(fieldList, name) {}
        AmqpDeleteOnNoMessages::~AmqpDeleteOnNoMessages() {}
        // static
        const FieldTypeList_t AmqpDeleteOnNoMessages::s_fieldTypeList = {};


        AmqpDeleteOnNoLinksOrMessages::AmqpDeleteOnNoLinksOrMessages(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpDeleteOnNoLinksOrMessages::AmqpDeleteOnNoLinksOrMessages(AmqpList* fieldList, const char* name): CompositeType(fieldList, name) {}
        AmqpDeleteOnNoLinksOrMessages::~AmqpDeleteOnNoLinksOrMessages() {}
        // static
        const FieldTypeList_t AmqpDeleteOnNoLinksOrMessages::s_fieldTypeList = {};


        AmqpCoordinator::AmqpCoordinator(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpCoordinator::AmqpCoordinator(AmqpList* fieldList, const char* name): CompositeType(fieldList, name) {}
        AmqpCoordinator::~AmqpCoordinator() {}
        // static
        const FieldTypeList_t AmqpCoordinator::s_fieldTypeList = {
            FieldType("capabilities", AmqpPrimitiveType_t::SYMBOL_TYPE, false, true, {AmqpRequiresProvides_t::TXN_CAPABILITY})
        };
        // static
        AmqpProvidesRequiresList_t AmqpCoordinator::s_providesList = {
        	AmqpRequiresProvides_t::TARGET
        };


        AmqpDeclare::AmqpDeclare(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpDeclare::AmqpDeclare(AmqpList* fieldList, const char* name): CompositeType(fieldList, name) {}
        AmqpDeclare::~AmqpDeclare() {}
        // static
        const FieldTypeList_t AmqpDeclare::s_fieldTypeList = {
            FieldType("global-id", "*", false, false, {AmqpRequiresProvides_t::GLOBAL_TX_ID}) // NOTE: GLOBAL_TX_ID is not yet defined by AMQP 1.0 spec
        };


        AmqpDischarge::AmqpDischarge(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpDischarge::AmqpDischarge(AmqpList* fieldList, const char* name): CompositeType(fieldList, name) {}
        AmqpDischarge::~AmqpDischarge() {}
        // static
        const FieldTypeList_t AmqpDischarge::s_fieldTypeList = {
            FieldType("txn-id", "*", true, false, {AmqpRequiresProvides_t::TXN_ID}),
            FieldType("fail", AmqpPrimitiveType_t::BOOLEAN_TYPE, false, false)
        };


        AmqpDeclared::AmqpDeclared(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpDeclared::AmqpDeclared(AmqpList* fieldList, const char* name): CompositeType(fieldList, name) {}
        AmqpDeclared::~AmqpDeclared() {}
        // static
        const FieldTypeList_t AmqpDeclared::s_fieldTypeList = {
            FieldType("txn-id", "*", true, false, {AmqpRequiresProvides_t::TXN_ID})
        };
        // static
        AmqpProvidesRequiresList_t AmqpDeclared::s_providesList = {
        	AmqpRequiresProvides_t::DELIVERY_STATE,
        	AmqpRequiresProvides_t::OUTCOME
        };


        AmqpTransactionalState::AmqpTransactionalState(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpTransactionalState::AmqpTransactionalState(AmqpList* fieldList, const char* name): CompositeType(fieldList, name) {}
        AmqpTransactionalState::~AmqpTransactionalState() {}
        // static
        const FieldTypeList_t AmqpTransactionalState::s_fieldTypeList = {
            FieldType("txn-id", "*", true, false, {AmqpRequiresProvides_t::TXN_ID}),
            FieldType("outcome", "*", false, false, {AmqpRequiresProvides_t::OUTCOME})
        };
        // static
        AmqpProvidesRequiresList_t AmqpTransactionalState::s_providesList = {
        	AmqpRequiresProvides_t::DELIVERY_STATE
        };


        AmqpSaslMechanisms::AmqpSaslMechanisms(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpSaslMechanisms::AmqpSaslMechanisms(AmqpList* fieldList, const char* name): CompositeType(fieldList, name) {}
        AmqpSaslMechanisms::~AmqpSaslMechanisms() {}
        // static
        const FieldTypeList_t AmqpSaslMechanisms::s_fieldTypeList = {
            FieldType("sasl-server-mechanisms", AmqpPrimitiveType_t::SYMBOL_TYPE, true, true)
        };


        AmqpSaslInit::AmqpSaslInit(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpSaslInit::AmqpSaslInit(AmqpList* fieldList, const char* name): CompositeType(fieldList, name) {}
        AmqpSaslInit::~AmqpSaslInit() {}
        // static
        const FieldTypeList_t AmqpSaslInit::s_fieldTypeList = {
            FieldType("mechanism", AmqpPrimitiveType_t::SYMBOL_TYPE, true, false),
            FieldType("initial-response", AmqpPrimitiveType_t::BINARY_TYPE, false, false),
            FieldType("hostname", AmqpPrimitiveType_t::STRING_TYPE, false, false)
        };


        AmqpSaslChallenge::AmqpSaslChallenge(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpSaslChallenge::AmqpSaslChallenge(AmqpList* fieldList, const char* name): CompositeType(fieldList, name) {}
        AmqpSaslChallenge::~AmqpSaslChallenge() {}
        // static
        const FieldTypeList_t AmqpSaslChallenge::s_fieldTypeList = {
            FieldType("challenge", AmqpPrimitiveType_t::BINARY_TYPE, true, false)
        };


        AmqpSaslResponse::AmqpSaslResponse(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpSaslResponse::AmqpSaslResponse(AmqpList* fieldList, const char* name): CompositeType(fieldList, name) {}
        AmqpSaslResponse::~AmqpSaslResponse() {}
        // static
        const FieldTypeList_t AmqpSaslResponse::s_fieldTypeList = {
            FieldType("response", AmqpPrimitiveType_t::BINARY_TYPE, true, false)
        };


        AmqpSaslOutcome::AmqpSaslOutcome(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpSaslOutcome::AmqpSaslOutcome(AmqpList* fieldList, const char* name): CompositeType(fieldList, name) {}
        AmqpSaslOutcome::~AmqpSaslOutcome() {}
        // static
        const FieldTypeList_t AmqpSaslOutcome::s_fieldTypeList = {
            FieldType("code", AmqpPrimitiveType_t::SASL_CODE_TYPE, true, false),
            FieldType("additional-data", AmqpPrimitiveType_t::BINARY_TYPE, false, false)
        };


    } /* namespace amqp10 */
} /* namespace amqpAnalyze */
