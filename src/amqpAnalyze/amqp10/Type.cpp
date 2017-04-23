/*
 * AmqpTypes.cpp
 *
 *  Created on: Apr 5, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/amqp10/Type.hpp>

#include <amqpAnalyze/amqp10/FieldType.hpp>
#include <amqpAnalyze/amqp10/FrameBuffer.hpp>
#include <amqpAnalyze/amqp10/ProvidesRequires.hpp>
#include <amqpAnalyze/Error.hpp>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <std/AnsiTermColors.hpp>

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
                oss << providesRequiresNames[*i];
            }
            oss << "]";
            return oss.str();
        }


        PrimitiveType::PrimitiveType(): Type() {}
        PrimitiveType::PrimitiveType(const char* name): Type(name) {}
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
            {amqpPrimitiveType_t::RECEIVER_SETTLE_MODE_TYPE, "receiver-settle-mode"},
            {amqpPrimitiveType_t::HANDLE_TYPE, "handle"},
            {amqpPrimitiveType_t::SECONDS_TYPE, "seconds"},
            {amqpPrimitiveType_t::MILLISECONDS_TYPE, "milliseconds"},
            {amqpPrimitiveType_t::DELIVERY_TAG_TYPE, "delivery-tag"},
            {amqpPrimitiveType_t::DELIVERY_NUMBER_TYPE, "delivery-number"},
            {amqpPrimitiveType_t::TRANSFER_NUMBER_TYPE, "transfer-number"},
            {amqpPrimitiveType_t::SEQUENCE_NUMBER_TYPE, "sequence-number"},
            {amqpPrimitiveType_t::MESSAGE_FORMAT_TYPE, "message-format"},
            {amqpPrimitiveType_t::LANGUAGE_TAG_TYPE, "language-tag"},
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
            return COLOR(FGND_BWHT, "null", colorFlag);
        }


        AmqpBoolean::AmqpBoolean(bool v): PrimitiveType(), _value(v) {}
        AmqpBoolean::AmqpBoolean(bool v, const char* name): PrimitiveType(name), _value(v) {}
        AmqpBoolean::~AmqpBoolean() {}
        std::string AmqpBoolean::valueStr(bool colorFlag) const {
            return COLOR(FGND_BWHT, (_value ? "true" : "false"), colorFlag);
        }


        AmqpUbyte::AmqpUbyte(uint8_t v): PrimitiveType(), _value(v) {}
        AmqpUbyte::AmqpUbyte(uint8_t v, const char* name): PrimitiveType(name), _value(v) {}
        AmqpUbyte::~AmqpUbyte() {}
        std::string AmqpUbyte::valueStr(bool colorFlag) const {
            std::stringstream oss;
            oss << "0x" << std::hex << (int)_value;
            return COLOR(FGND_BWHT, oss.str(), colorFlag);
        }


        AmqpUshort::AmqpUshort(uint16_t v): PrimitiveType(), _value(v) {}
        AmqpUshort::AmqpUshort(uint16_t v, const char* name): PrimitiveType(name), _value(v) {}
        AmqpUshort::~AmqpUshort() {}
        std::string AmqpUshort::valueStr(bool colorFlag) const {
            std::stringstream oss;
            oss << "0x" << std::hex << _value;
            return COLOR(FGND_BWHT, oss.str(), colorFlag);
        }


        AmqpUint::AmqpUint(uint32_t v): PrimitiveType(), _value(v) {}
        AmqpUint::AmqpUint(uint32_t v, const char* name): PrimitiveType(name), _value(v) {}
        AmqpUint::~AmqpUint() {}
        std::string AmqpUint::valueStr(bool colorFlag) const {
            std::ostringstream oss;
            oss<< "0x" << std::hex << _value;
            return COLOR(FGND_BWHT, oss.str(), colorFlag);
        }


        AmqpUlong::AmqpUlong(uint64_t v): PrimitiveType(), _value(v) {}
        AmqpUlong::AmqpUlong(uint64_t v, const char* name): PrimitiveType(name), _value(v) {}
        AmqpUlong::~AmqpUlong() {}
        std::string AmqpUlong::valueStr(bool colorFlag) const {
            std::ostringstream oss;
            oss << "0x" << std::hex << _value;
            return COLOR(FGND_BWHT, oss.str(), colorFlag);
        }


        AmqpByte::AmqpByte(int8_t v): PrimitiveType(), _value(v) {}
        AmqpByte::AmqpByte(int8_t v, const char* name): PrimitiveType(name), _value(v) {}
        AmqpByte::~AmqpByte() {}
        std::string AmqpByte::valueStr(bool colorFlag) const {
            std::ostringstream oss;
            oss <<  "0x" << std::hex << (int)(uint8_t)_value;
            return COLOR(FGND_BWHT, oss.str(), colorFlag);
        }


        AmqpShort::AmqpShort(int16_t v): PrimitiveType(), _value(v) {}
        AmqpShort::AmqpShort(int16_t v, const char* name): PrimitiveType(name), _value(v) {}
        AmqpShort::~AmqpShort() {}
        std::string AmqpShort::valueStr(bool colorFlag) const {
            std::ostringstream oss;
            oss << "0x" << std::hex << _value;
            return COLOR(FGND_BWHT, oss.str(), colorFlag);
        }


        AmqpInt::AmqpInt(int32_t v): PrimitiveType(), _value(v) {}
        AmqpInt::AmqpInt(int32_t v, const char* name): PrimitiveType(name), _value(v) {}
        AmqpInt::~AmqpInt() {}
        std::string AmqpInt::valueStr(bool colorFlag) const {
            std::ostringstream oss;
            oss << "0x" << std::hex << _value;
            return COLOR(FGND_BWHT, oss.str(), colorFlag);
        }


        AmqpLong::AmqpLong(int64_t v): PrimitiveType(), _value(v) {}
        AmqpLong::AmqpLong(int64_t v, const char* name): PrimitiveType(name), _value(v) {}
        AmqpLong::~AmqpLong() {}
        std::string AmqpLong::valueStr(bool colorFlag) const {
            std::ostringstream oss;
            oss << "0x" << std::hex << _value;
            return COLOR(FGND_BWHT, oss.str(), colorFlag);
        }


        AmqpFloat::AmqpFloat(float v): PrimitiveType(), _value(v) {}
        AmqpFloat::AmqpFloat(float v, const char* name): PrimitiveType(name), _value(v) {}
        AmqpFloat::~AmqpFloat() {}
        std::string AmqpFloat::valueStr(bool colorFlag) const {
            std::ostringstream oss;
            oss << _value;
            return COLOR(FGND_BWHT, oss.str(), colorFlag);
        }


        AmqpDouble::AmqpDouble(double v): PrimitiveType(), _value(v) {}
        AmqpDouble::AmqpDouble(double v, const char* name): PrimitiveType(name), _value(v) {}
        AmqpDouble::~AmqpDouble() {}
        std::string AmqpDouble::valueStr(bool colorFlag) const {
            std::ostringstream oss;
            oss << _value;
            return COLOR(FGND_BWHT, oss.str(), colorFlag);
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
            return COLOR(FGND_BWHT, oss.str(), colorFlag);
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
            return COLOR(FGND_BWHT, oss.str(), colorFlag);
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
            return COLOR(FGND_BWHT, oss.str(), colorFlag);
        }


        AmqpChar::AmqpChar(char32_t v): PrimitiveType(), _value(v) {}
        AmqpChar::AmqpChar(char32_t v, const char* name): PrimitiveType(name), _value(v) {}
        AmqpChar::~AmqpChar() {}
        std::string AmqpChar::valueStr(bool colorFlag) const {
            std::ostringstream oss;
            oss << "0x" << std::hex << (int)_value << "='" << _value << "'";
            return COLOR(FGND_BWHT, oss.str(), colorFlag);
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
            oss << "0x" << std::hex << _value;
            if (_value > 0) oss << "=" << buf << "." << std::setfill('0') << std::setw(3) << (_value%1000) << " " << tzbuf;
            return COLOR(FGND_BWHT, oss.str(), colorFlag);
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
            return COLOR(FGND_BWHT, oss.str(), colorFlag);
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
            return COLOR(FGND_BWHT, oss.str(), colorFlag);
        }


        AmqpString::AmqpString(): PrimitiveType(), _value() {}
        AmqpString::AmqpString(const char* name): PrimitiveType(name), _value() {}
        AmqpString::~AmqpString() {}
        std::string AmqpString::valueStr(bool colorFlag) const {
            std::stringstream oss;
            oss << "\"" << _value << "\"";
            return COLOR(FGND_BWHT, oss.str(), colorFlag);
        }


        AmqpSymbol::AmqpSymbol(): PrimitiveType(), _value() {}
        AmqpSymbol::AmqpSymbol(const char* name): PrimitiveType(name), _value() {}
        AmqpSymbol::~AmqpSymbol() {}
        std::string AmqpSymbol::valueStr(bool colorFlag) const {
            std::stringstream oss;
            oss << "\"" << _value << "\"";
            return COLOR(FGND_BWHT, oss.str(), colorFlag);
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
            for (amqp_list_itr_t i=_value.begin(); i!=_value.end(); ++i) {
                delete *i;
            }
            _value.clear();
        }
        std::string AmqpList::valueStr(bool colorFlag) const {
            std::ostringstream oss;
            for (amqp_list_citr_t i=_value.cbegin(); i!=_value.cend(); ++i) {
                if (i!=_value.cbegin()) oss << ", ";
                oss << (*i)->typeValueStr(colorFlag);
            }
            return oss.str();
        }
        void AmqpList::appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin, bool colorFlag) const {
            std::string m(margin + 3, ' ');
            if (!ignoreFirstMargin) oss << "\n" << m;
            oss << ": [";
            for (amqp_list_citr_t i=_value.cbegin(); i<_value.cend(); ++i) {
                if (i!=_value.cbegin()) oss << "\n" << m;
                stringAppendHandler(oss, *i, margin + 3, true, colorFlag);
            }
            oss << "]";
        }
        void AmqpList::validate(const fieldTypeList_t& fieldTypeList, addErrorFp errorHandler, AmqpBlock* errorHandlerInstance) {
            // 1. Check field list not larger than fieldTypeList
            if (_value.size() > fieldTypeList.size()) {
                (errorHandlerInstance->*errorHandler)(new AmqpValidationError(error_severity_t::ERROR,
                                errorHandlerInstance->packetNum(),
                                errorHandlerInstance->dataOffset(),
                                MSG("FieldTypeList size mismatch: FieldTypeList size=" << fieldTypeList.size() << ", fieldList size=" << _value.size())));
            }

            // 2. Iterate through fields
            int index = 0;
            for (amqp_list_citr_t i=_value.cbegin(); i!=_value.cend(); ++i) {
                const FieldType& fieldType = fieldTypeList[index++];

                // 3. Check that null not present for required field
                if (fieldType._mandatoryFlag && (*i)->isNull()) {
                    (errorHandlerInstance->*errorHandler)(new AmqpValidationError(errorHandlerInstance->packetNum(),
                                    errorHandlerInstance->dataOffset(),
                                    MSG("Mandatory field \"" << (*i)->name() << "\" is null")));
                }

                // 4. Check that fieldTypeList requires is met in value (if not null)
                if (!(*i)->isNull()) {
                    for (amqp_provides_requires_list_citr_t j=fieldType._requiresList.cbegin(); j!=fieldType._requiresList.cend(); ++j) {
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
                amqp_map_itr_t i = _value.begin();
                Type* k = i->first;
                Type* v = i->second;
                _value.erase(i);
                delete k;
                delete v;
            }
        }
        std::string AmqpMap::valueStr(bool colorFlag) const {
            std::ostringstream oss;
            for (amqp_map_citr_t i=_value.cbegin(); i!=_value.cend(); ++i) {
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
            for (amqp_map_citr_t i=_value.cbegin(); i!=_value.cend(); ++i) {
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
            for (amqp_array_itr_t i=_value.begin(); i!=_value.end(); ++i) {
                delete *i;
            }
            _value.clear();
        }
        std::string AmqpArray::valueStr(bool colorFlag) const {
            std::ostringstream oss;
            for (amqp_array_citr_t i=_value.cbegin(); i!=_value.cend(); ++i) {
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
            for (amqp_array_citr_t i=_value.cbegin(); i!=_value.cend(); ++i) {
                if (i!=_value.cbegin()) oss << "\n" << m;
                stringAppendHandler(oss, *i, l, false, colorFlag);
            }
            oss << "]";
        }


        AmqpRole::AmqpRole(amqp_role_t v): AmqpBoolean((amqp_boolean_t)v) {}
        AmqpRole::AmqpRole(amqp_role_t v, const char* name): AmqpBoolean((amqp_boolean_t)v, name) {}
        AmqpRole:: ~AmqpRole() {}
        std::string AmqpRole::valueStr(bool colorFlag) const {
            return COLOR(FGND_BWHT, s_choiceNames.at((amqp_role_t)_value), colorFlag);
        }
        // static
        std::map<amqp_role_t, const char*> AmqpRole::s_choiceNames = {
            {amqp_role_t::SENDER, "sender"},
            {amqp_role_t::RECEIVER, "receiver"}
        };


        AmqpSenderSettleMode::AmqpSenderSettleMode(amqp_sender_settle_mode_t v): AmqpUbyte((amqp_ubyte_t)v) {}
        AmqpSenderSettleMode::AmqpSenderSettleMode(amqp_sender_settle_mode_t v, const char* name): AmqpUbyte((amqp_ubyte_t)v, name) {}
        AmqpSenderSettleMode::~AmqpSenderSettleMode() {}
        std::string AmqpSenderSettleMode::valueStr(bool colorFlag) const {
            return COLOR(FGND_BWHT, s_choiceNames.at((amqp_sender_settle_mode_t)_value), colorFlag);
        }
        // static
        std::map<amqp_sender_settle_mode_t, const char*> AmqpSenderSettleMode::s_choiceNames = {
            {amqp_sender_settle_mode_t::UNSETTLED, "unsettled"},
            {amqp_sender_settle_mode_t::SETTLED, "settled"},
            {amqp_sender_settle_mode_t::MIXED, "mixed"}
        };


        AmqpReceiverSettleMode::AmqpReceiverSettleMode(amqp_receiver_settle_mode_t v): AmqpUbyte((amqp_ubyte_t)v) {}
        AmqpReceiverSettleMode::AmqpReceiverSettleMode(amqp_receiver_settle_mode_t v, const char* name): AmqpUbyte((amqp_ubyte_t)v, name) {}
        AmqpReceiverSettleMode::~AmqpReceiverSettleMode() {}
        std::string AmqpReceiverSettleMode::valueStr(bool colorFlag) const {
            return COLOR(FGND_BWHT, s_choiceNames.at((amqp_receiver_settle_mode_t)_value), colorFlag);
        }
        // static
        std::map<amqp_receiver_settle_mode_t, const char*> AmqpReceiverSettleMode::s_choiceNames = {
            {amqp_receiver_settle_mode_t::FIRST, "first"},
            {amqp_receiver_settle_mode_t::SECOND, "second"}
        };


        AmqpHandle::AmqpHandle(amqp_handle_t v): AmqpUint(v) {}
        AmqpHandle::AmqpHandle(amqp_handle_t v, const char* name): AmqpUint(v, name) {}
        AmqpHandle::~AmqpHandle() {}


        AmqpSeconds::AmqpSeconds(amqp_seconds_t v): AmqpUint(v) {}
        AmqpSeconds::AmqpSeconds(amqp_seconds_t v, const char* name): AmqpUint(v, name) {}
        AmqpSeconds::~AmqpSeconds() {}


        AmqpMilliseconds::AmqpMilliseconds(amqp_milliseconds_t v): AmqpUint(v) {}
        AmqpMilliseconds::AmqpMilliseconds(amqp_milliseconds_t v, const char* name): AmqpUint(v, name) {}
        AmqpMilliseconds::~AmqpMilliseconds() {}


        AmqpDeliveryTag::AmqpDeliveryTag(): AmqpBinary() {}
        AmqpDeliveryTag::AmqpDeliveryTag(const char* name): AmqpBinary(name) {}
        AmqpDeliveryTag::~AmqpDeliveryTag() {}


        AmqpSequenceNum::AmqpSequenceNum(amqp_sequence_num_t v): AmqpUint(v) {}
        AmqpSequenceNum::AmqpSequenceNum(amqp_sequence_num_t v, const char* name): AmqpUint(v, name) {}
        AmqpSequenceNum::~AmqpSequenceNum() {}


        AmqpDeliveryNum::AmqpDeliveryNum(amqp_delivery_num_t v): AmqpSequenceNum(v) {}
        AmqpDeliveryNum::AmqpDeliveryNum(amqp_delivery_num_t v, const char* name): AmqpSequenceNum(v, name) {}
        AmqpDeliveryNum::~AmqpDeliveryNum() {}


        AmqpTransferNum::AmqpTransferNum(amqp_transfer_num_t v): AmqpSequenceNum(v) {}
        AmqpTransferNum::AmqpTransferNum(amqp_transfer_num_t v, const char* name): AmqpSequenceNum(v, name) {}
        AmqpTransferNum::~AmqpTransferNum() {}


        AmqpMessageFormat::AmqpMessageFormat(amqp_message_format_t v): AmqpUint(v) {}
        AmqpMessageFormat::AmqpMessageFormat(amqp_message_format_t v, const char* name): AmqpUint(v, name) {}
        AmqpMessageFormat::~AmqpMessageFormat() {}
        std::string AmqpMessageFormat::valueStr(bool colorFlag) const {
            std::ostringstream oss;
            oss << typeStr() << "fmt=0x" << std::hex << std::setfill('0') << std::setw(6) << getFormat()
                << " ver=0x" << std::setw(2) << (int)getVersion() << ")";
            return COLOR(FGND_BWHT, oss.str(), colorFlag);
        }


        AmqpLanguageTag::AmqpLanguageTag(): AmqpSymbol() {}
        AmqpLanguageTag::AmqpLanguageTag(const char* name): AmqpSymbol(name) {}
        AmqpLanguageTag::~AmqpLanguageTag() {}


        AmqpFields::AmqpFields(): AmqpMap() {}
        AmqpFields::AmqpFields(const char* name): AmqpMap(name) {}
        AmqpFields::~AmqpFields() {}

        AmqpError::AmqpError(const amqp_error_t& v): AmqpSymbol() {
            checkAssignValue(v);
        }
        AmqpError::AmqpError(const amqp_error_t& v, const char* name): AmqpSymbol(name) {
            checkAssignValue(v);
        }
        AmqpError::~AmqpError() {}
        void AmqpError::checkAssignValue(const amqp_error_t& v) {
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
        // static
        amqp_provides_requires_list_t AmqpError::s_providesList = {
            amqpRequiresProvides_t::ERROR_CONDITION
        };
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


        AmqpConnectionError::AmqpConnectionError(const amqp_connection_error_t& v): AmqpSymbol() {
            checkAssignValue(v);
        }
        AmqpConnectionError::AmqpConnectionError(const amqp_connection_error_t& v, const char* name): AmqpSymbol(name) {
            checkAssignValue(v);
        }
        AmqpConnectionError::~AmqpConnectionError() {}
        void AmqpConnectionError::checkAssignValue(const amqp_connection_error_t& v) {
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
        // static
        amqp_provides_requires_list_t AmqpConnectionError::s_providesList = {
            amqpRequiresProvides_t::ERROR_CONDITION
        };
        // static
        amqp_connection_error_list_t AmqpConnectionError::s_amqpConnectionErrors = {
            "amqp:connection:forced",
            "amqp:connection:framing-error",
            "amqp:connection:redirect"
        };


        AmqpSessionError::AmqpSessionError(const amqp_session_error_t& v): AmqpSymbol() {
            checkAssignValue(v);
        }
        AmqpSessionError::AmqpSessionError(const amqp_session_error_t& v, const char* name): AmqpSymbol(name) {
            checkAssignValue(v);
        }
        AmqpSessionError::~AmqpSessionError() {}
        void AmqpSessionError::checkAssignValue(const amqp_session_error_t& v) {
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
        // static
        amqp_provides_requires_list_t AmqpSessionError::s_providesList = {
            amqpRequiresProvides_t::ERROR_CONDITION
        };
        // static
        amqp_session_error_list_t AmqpSessionError::s_amqpSessionErrors = {
            "amqp:session:window-violation",
            "amqp:session:errant-link",
            "amqp:session:handle-in-use",
            "amqp:session:unattached-handle"
        };


        AmqpLinkError::AmqpLinkError(const amqp_link_error_t& v): AmqpSymbol() {
            checkAssignValue(v);
        }
        AmqpLinkError::AmqpLinkError(const amqp_link_error_t& v, const char* name): AmqpSymbol(name) {
            checkAssignValue(v);
        }
        AmqpLinkError::~AmqpLinkError() {}
        void AmqpLinkError::checkAssignValue(const amqp_link_error_t& v) {
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
        // static
        amqp_provides_requires_list_t AmqpLinkError::s_providesList = {
            amqpRequiresProvides_t::ERROR_CONDITION
        };
        // static
        amqp_link_error_list_t AmqpLinkError::s_amqpLinkErrors = {
            "amqp:link:detach-forced",
            "amqp:link:transfer-limit-exceeded",
            "amqp:link:message-size-exceeded",
            "amqp:link:redirect",
            "amqp:link:stolen"
        };


        AmqpAnnotations::AmqpAnnotations(): AmqpMap() {}
        AmqpAnnotations::AmqpAnnotations(const char* name): AmqpMap(name) {}
        AmqpAnnotations::~AmqpAnnotations() {}


        AmqpMessageIdUlong::AmqpMessageIdUlong(amqp_msg_id_ulong_t v): AmqpUlong((amqp_ulong_t)v) {}
        AmqpMessageIdUlong::AmqpMessageIdUlong(amqp_msg_id_ulong_t v, const char* name): AmqpUlong((amqp_ulong_t)v, name) {}
        AmqpMessageIdUlong::~AmqpMessageIdUlong() {}
        // static
        amqp_provides_requires_list_t AmqpMessageIdUlong::s_providesList = {
            amqpRequiresProvides_t::MESSAGE_ID
        };


        AmqpMessageIdUuid::AmqpMessageIdUuid(): AmqpUuid() {}
        AmqpMessageIdUuid::AmqpMessageIdUuid(const char* name): AmqpUuid(name) {}
        AmqpMessageIdUuid::~AmqpMessageIdUuid() {}
        // static
        amqp_provides_requires_list_t AmqpMessageIdUuid::s_providesList = {
            amqpRequiresProvides_t::MESSAGE_ID
        };


        AmqpMessageIdBinary::AmqpMessageIdBinary(): AmqpBinary() {}
        AmqpMessageIdBinary::AmqpMessageIdBinary(const char* name): AmqpBinary(name) {}
        AmqpMessageIdBinary::~AmqpMessageIdBinary() {}
        // static
        amqp_provides_requires_list_t AmqpMessageIdBinary::s_providesList = {
            amqpRequiresProvides_t::MESSAGE_ID
        };


        AmqpMessageIdString::AmqpMessageIdString(): AmqpString() {}
        AmqpMessageIdString::AmqpMessageIdString(const char* name): AmqpString(name) {}
        AmqpMessageIdString::~AmqpMessageIdString() {}
        // static
        amqp_provides_requires_list_t AmqpMessageIdString::s_providesList = {
            amqpRequiresProvides_t::MESSAGE_ID
        };


        AmqpAddress::AmqpAddress(): AmqpString() {}
        AmqpAddress::AmqpAddress(const char* name): AmqpString(name) {}
        AmqpAddress::~AmqpAddress() {}
        // static
        amqp_provides_requires_list_t AmqpAddress::s_providesList = {
            amqpRequiresProvides_t::ADDRESS
        };


        AmqpTerminusDurability::AmqpTerminusDurability(amqp_terminus_durability_t v): AmqpUint((amqp_uint_t)v) {}
        AmqpTerminusDurability::AmqpTerminusDurability(amqp_terminus_durability_t v, const char* name): AmqpUint((amqp_uint_t)v, name) {}
        AmqpTerminusDurability::~AmqpTerminusDurability() {}
        std::string AmqpTerminusDurability::valueStr(bool colorFlag) const {
            return COLOR(FGND_BWHT, s_choiceNames.at((amqp_terminus_durability_t)_value), colorFlag);
        }
        // static
        std::map<amqp_terminus_durability_t, const char*> AmqpTerminusDurability::s_choiceNames = {
            {amqp_terminus_durability_t::NONE, "none"},
            {amqp_terminus_durability_t::CONFIGURATION, "configuration"},
            {amqp_terminus_durability_t::UNSETTLED_STATE, "unsettled-state"}
        };


        AmqpTerminusExpiryPolicy::AmqpTerminusExpiryPolicy(const amqp_terminus_expiry_policy_t& v): AmqpSymbol() {
            checkAssignValue(v);
        }
        AmqpTerminusExpiryPolicy::AmqpTerminusExpiryPolicy(const amqp_terminus_expiry_policy_t& v, const char* name): AmqpSymbol(name) {
            checkAssignValue(v);
        }
        AmqpTerminusExpiryPolicy::~AmqpTerminusExpiryPolicy() {}
        void AmqpTerminusExpiryPolicy::checkAssignValue(const amqp_terminus_expiry_policy_t& v) {
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
        // static
        amqp_terminus_expiry_policy_list_t AmqpTerminusExpiryPolicy::s_terminusExpiryPolicyList = {
             "link-detach",
             "session-end",
             "connection-close",
             "never"
        };


        AmqpStandardDistributionMode::AmqpStandardDistributionMode(const amqp_std_dist_mode_t& v): AmqpSymbol() {
            checkAssignValue(v);
        }
        AmqpStandardDistributionMode::AmqpStandardDistributionMode(const amqp_std_dist_mode_t& v, const char* name): AmqpSymbol(name) {
            checkAssignValue(v);
        }
        AmqpStandardDistributionMode::~AmqpStandardDistributionMode() {}
        void AmqpStandardDistributionMode::checkAssignValue(const amqp_std_dist_mode_t& v) {
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
        // static
        amqp_std_dist_mode_list_t AmqpStandardDistributionMode::s_StandardDistributionModeList {
            "move",
            "copy"
        };
        // static
        amqp_provides_requires_list_t AmqpStandardDistributionMode::s_providesList= {
            amqpRequiresProvides_t::DISTRIBUTION_MODE
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
        amqp_provides_requires_list_t AmqpTransactionId::s_providesList = {
        	amqpRequiresProvides_t::TXN_ID
        };

        AmqpTransactionalCapability::AmqpTransactionalCapability(const amqp_transaction_capability_t& v): AmqpSymbol() {
            checkAssignValue(v);
        }
        AmqpTransactionalCapability::AmqpTransactionalCapability(const amqp_transaction_capability_t& v, const char* name): AmqpSymbol(name) {
            checkAssignValue(v);
        }
        AmqpTransactionalCapability::~AmqpTransactionalCapability() {}
        void AmqpTransactionalCapability::checkAssignValue(const amqp_transaction_capability_t& v) {
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
        AmqpTransactionError::AmqpTransactionError(const amqp_transaction_error_t& v, const char* name): AmqpSymbol(name) {
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
        amqp_transaction_error_list_t AmqpTransactionError::s_transactionErrorList = {
            "unknown-id",
            "transaction-rollback",
            "transaction-timeout"
        };
        // static
        amqp_provides_requires_list_t AmqpTransactionError::s_providesList = {
            amqpRequiresProvides_t::ERROR_CONDITION
        };


        AmqpSaslCode::AmqpSaslCode(amqp_sasl_code_t v): AmqpUbyte((amqp_ubyte_t)v) {}
        AmqpSaslCode::AmqpSaslCode(amqp_sasl_code_t v, const char* name): AmqpUbyte((amqp_ubyte_t)v, name) {}
        AmqpSaslCode::~AmqpSaslCode() {}
        std::string AmqpSaslCode::valueStr(bool colorFlag) const {
            return COLOR(FGND_BWHT, s_choiceNames.at((amqp_sasl_code_t)_value), colorFlag);
        }
        // static
        std::map<amqp_sasl_code_t, const char*> AmqpSaslCode::s_choiceNames = {
            {amqp_sasl_code_t::OK, "ok"},
            {amqp_sasl_code_t::AUTH, "auth"},
            {amqp_sasl_code_t::SYS, "sys"},
            {amqp_sasl_code_t::SYS_PERM, "sys-perm"},
            {amqp_sasl_code_t::SYS_TEMP, "sys-temp"}
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
        // static
        amqp_provides_requires_list_t CompositeType::s_providesList = {};


        AmqpErrorRecord::AmqpErrorRecord(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpErrorRecord::AmqpErrorRecord(AmqpList* fieldList, const char* name): CompositeType(fieldList, name) {}
        AmqpErrorRecord::~AmqpErrorRecord() {}
        // static
        const fieldTypeList_t AmqpErrorRecord::s_fieldTypeList = {
            FieldType("condition", amqpPrimitiveType_t::SYMBOL_TYPE, true, false, {amqpRequiresProvides_t::ERROR_CONDITION}),
            FieldType("description", amqpPrimitiveType_t::STRING_TYPE, false, false),
            FieldType("info", amqpPrimitiveType_t::FIELDS_TYPE, false, false)
        };


        AmqpReceived::AmqpReceived(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpReceived::AmqpReceived(AmqpList* fieldList, const char* name): CompositeType(fieldList, name) {}
        AmqpReceived::~AmqpReceived() {}
        // static
        const fieldTypeList_t AmqpReceived::s_fieldTypeList = {
            FieldType("section-number", amqpPrimitiveType_t::UINT_TYPE, true, false),
            FieldType("section-offset", amqpPrimitiveType_t::ULONG_TYPE, true, false)
        };
        // static
        amqp_provides_requires_list_t AmqpReceived::s_providesList = {
        	amqpRequiresProvides_t::DELIVERY_STATE
        };

        AmqpAccepted::AmqpAccepted(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpAccepted::AmqpAccepted(AmqpList* fieldList, const char* name): CompositeType(fieldList, name) {}
        AmqpAccepted::~AmqpAccepted() {}
        // static
        const fieldTypeList_t AmqpAccepted::s_fieldTypeList = {};
        // static
        amqp_provides_requires_list_t AmqpAccepted::s_providesList = {
        	amqpRequiresProvides_t::DELIVERY_STATE,
        	amqpRequiresProvides_t::OUTCOME
        };


        AmqpRejected::AmqpRejected(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpRejected::AmqpRejected(AmqpList* fieldList, const char* name): CompositeType(fieldList, name) {}
        AmqpRejected::~AmqpRejected() {}
        // static
        const fieldTypeList_t AmqpRejected::s_fieldTypeList = {
            FieldType("error", amqpPrimitiveType_t::AMQP_ERROR_TYPE, false, false)
        };
        // static
        amqp_provides_requires_list_t AmqpRejected::s_providesList = {
        	amqpRequiresProvides_t::DELIVERY_STATE,
        	amqpRequiresProvides_t::OUTCOME
        };


        AmqpReleased::AmqpReleased(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpReleased::AmqpReleased(AmqpList* fieldList, const char* name): CompositeType(fieldList, name) {}
        AmqpReleased::~AmqpReleased() {}
        // static
        const fieldTypeList_t AmqpReleased::s_fieldTypeList = {};
        // static
        amqp_provides_requires_list_t AmqpReleased::s_providesList = {
        	amqpRequiresProvides_t::DELIVERY_STATE,
        	amqpRequiresProvides_t::OUTCOME
        };


        AmqpModified::AmqpModified(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpModified::AmqpModified(AmqpList* fieldList, const char* name): CompositeType(fieldList, name) {}
        AmqpModified::~AmqpModified() {}
        // static
        const fieldTypeList_t AmqpModified::s_fieldTypeList = {
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
        AmqpSource::AmqpSource(AmqpList* fieldList, const char* name): CompositeType(fieldList, name) {}
        AmqpSource::~AmqpSource() {}
        // static
        const fieldTypeList_t AmqpSource::s_fieldTypeList = {
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
        AmqpTarget::AmqpTarget(AmqpList* fieldList, const char* name): CompositeType(fieldList, name) {}
        AmqpTarget::~AmqpTarget() {}
        // static
        const fieldTypeList_t AmqpTarget::s_fieldTypeList = {
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
        AmqpDeleteOnClose::AmqpDeleteOnClose(AmqpList* fieldList, const char* name): CompositeType(fieldList, name) {}
        AmqpDeleteOnClose::~AmqpDeleteOnClose() {}
        // static
        const fieldTypeList_t AmqpDeleteOnClose::s_fieldTypeList = {};


        AmqpDeleteOnNoLinks::AmqpDeleteOnNoLinks(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpDeleteOnNoLinks::AmqpDeleteOnNoLinks(AmqpList* fieldList, const char* name): CompositeType(fieldList, name) {}
        AmqpDeleteOnNoLinks::~AmqpDeleteOnNoLinks() {}
        // static
        const fieldTypeList_t AmqpDeleteOnNoLinks::s_fieldTypeList = {};


        AmqpDeleteOnNoMessages::AmqpDeleteOnNoMessages(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpDeleteOnNoMessages::AmqpDeleteOnNoMessages(AmqpList* fieldList, const char* name): CompositeType(fieldList, name) {}
        AmqpDeleteOnNoMessages::~AmqpDeleteOnNoMessages() {}
        // static
        const fieldTypeList_t AmqpDeleteOnNoMessages::s_fieldTypeList = {};


        AmqpDeleteOnNoLinksOrMessages::AmqpDeleteOnNoLinksOrMessages(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpDeleteOnNoLinksOrMessages::AmqpDeleteOnNoLinksOrMessages(AmqpList* fieldList, const char* name): CompositeType(fieldList, name) {}
        AmqpDeleteOnNoLinksOrMessages::~AmqpDeleteOnNoLinksOrMessages() {}
        // static
        const fieldTypeList_t AmqpDeleteOnNoLinksOrMessages::s_fieldTypeList = {};


        AmqpCoordinator::AmqpCoordinator(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpCoordinator::AmqpCoordinator(AmqpList* fieldList, const char* name): CompositeType(fieldList, name) {}
        AmqpCoordinator::~AmqpCoordinator() {}
        // static
        const fieldTypeList_t AmqpCoordinator::s_fieldTypeList = {
            FieldType("capabilities", amqpPrimitiveType_t::SYMBOL_TYPE, false, true, {amqpRequiresProvides_t::TXN_CAPABILITY})
        };
        // static
        amqp_provides_requires_list_t AmqpCoordinator::s_providesList = {
        	amqpRequiresProvides_t::TARGET
        };


        AmqpDeclare::AmqpDeclare(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpDeclare::AmqpDeclare(AmqpList* fieldList, const char* name): CompositeType(fieldList, name) {}
        AmqpDeclare::~AmqpDeclare() {}
        // static
        const fieldTypeList_t AmqpDeclare::s_fieldTypeList = {
            FieldType("global-id", "*", false, false, {amqpRequiresProvides_t::GLOBAL_TX_ID}) // NOTE: GLOBAL_TX_ID is not yet defined by AMQP 1.0 spec
        };


        AmqpDischarge::AmqpDischarge(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpDischarge::AmqpDischarge(AmqpList* fieldList, const char* name): CompositeType(fieldList, name) {}
        AmqpDischarge::~AmqpDischarge() {}
        // static
        const fieldTypeList_t AmqpDischarge::s_fieldTypeList = {
            FieldType("txn-id", "*", true, false, {amqpRequiresProvides_t::TXN_ID}),
            FieldType("fail", amqpPrimitiveType_t::BOOLEAN_TYPE, false, false)
        };


        AmqpDeclared::AmqpDeclared(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpDeclared::AmqpDeclared(AmqpList* fieldList, const char* name): CompositeType(fieldList, name) {}
        AmqpDeclared::~AmqpDeclared() {}
        // static
        const fieldTypeList_t AmqpDeclared::s_fieldTypeList = {
            FieldType("txn-id", "*", true, false, {amqpRequiresProvides_t::TXN_ID})
        };
        // static
        amqp_provides_requires_list_t AmqpDeclared::s_providesList = {
        	amqpRequiresProvides_t::DELIVERY_STATE,
        	amqpRequiresProvides_t::OUTCOME
        };


        AmqpTransactionalState::AmqpTransactionalState(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpTransactionalState::AmqpTransactionalState(AmqpList* fieldList, const char* name): CompositeType(fieldList, name) {}
        AmqpTransactionalState::~AmqpTransactionalState() {}
        // static
        const fieldTypeList_t AmqpTransactionalState::s_fieldTypeList = {
            FieldType("txn-id", "*", true, false, {amqpRequiresProvides_t::TXN_ID}),
            FieldType("outcome", "*", false, false, {amqpRequiresProvides_t::OUTCOME})
        };
        // static
        amqp_provides_requires_list_t AmqpTransactionalState::s_providesList = {
        	amqpRequiresProvides_t::DELIVERY_STATE
        };


        AmqpSaslMechanisms::AmqpSaslMechanisms(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpSaslMechanisms::AmqpSaslMechanisms(AmqpList* fieldList, const char* name): CompositeType(fieldList, name) {}
        AmqpSaslMechanisms::~AmqpSaslMechanisms() {}
        // static
        const fieldTypeList_t AmqpSaslMechanisms::s_fieldTypeList = {
            FieldType("sasl-server-mechanisms", amqpPrimitiveType_t::SYMBOL_TYPE, true, true)
        };


        AmqpSaslInit::AmqpSaslInit(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpSaslInit::AmqpSaslInit(AmqpList* fieldList, const char* name): CompositeType(fieldList, name) {}
        AmqpSaslInit::~AmqpSaslInit() {}
        // static
        const fieldTypeList_t AmqpSaslInit::s_fieldTypeList = {
            FieldType("mechanism", amqpPrimitiveType_t::SYMBOL_TYPE, true, false),
            FieldType("initial-response", amqpPrimitiveType_t::BINARY_TYPE, false, false),
            FieldType("hostname", amqpPrimitiveType_t::STRING_TYPE, false, false)
        };


        AmqpSaslChallenge::AmqpSaslChallenge(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpSaslChallenge::AmqpSaslChallenge(AmqpList* fieldList, const char* name): CompositeType(fieldList, name) {}
        AmqpSaslChallenge::~AmqpSaslChallenge() {}
        // static
        const fieldTypeList_t AmqpSaslChallenge::s_fieldTypeList = {
            FieldType("challenge", amqpPrimitiveType_t::BINARY_TYPE, true, false)
        };


        AmqpSaslResponse::AmqpSaslResponse(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpSaslResponse::AmqpSaslResponse(AmqpList* fieldList, const char* name): CompositeType(fieldList, name) {}
        AmqpSaslResponse::~AmqpSaslResponse() {}
        // static
        const fieldTypeList_t AmqpSaslResponse::s_fieldTypeList = {
            FieldType("response", amqpPrimitiveType_t::BINARY_TYPE, true, false)
        };


        AmqpSaslOutcome::AmqpSaslOutcome(AmqpList* fieldList): CompositeType(fieldList) {}
        AmqpSaslOutcome::AmqpSaslOutcome(AmqpList* fieldList, const char* name): CompositeType(fieldList, name) {}
        AmqpSaslOutcome::~AmqpSaslOutcome() {}
        // static
        const fieldTypeList_t AmqpSaslOutcome::s_fieldTypeList = {
            FieldType("code", amqpPrimitiveType_t::SASL_CODE_TYPE, true, false),
            FieldType("additional-data", amqpPrimitiveType_t::BINARY_TYPE, false, false)
        };


    } /* namespace amqp10 */
} /* namespace amqpAnalyze */
