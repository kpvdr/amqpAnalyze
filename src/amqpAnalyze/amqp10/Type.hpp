/*
 * AmqpTypes.hpp
 *
 *  Created on: Apr 5, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_AMQP10_TYPE_HPP_
#define SRC_AMQPANALYZE_AMQP10_TYPE_HPP_

#include <amqpAnalyze/amqp10/ProvidesRequires.hpp>

#include <amqpAnalyze/amqp10/AmqpBlock.hpp>
#include <array>
#include <cstdint>
#include <initializer_list>
#include <map>
#include <string>
#include <vector>

namespace amqpAnalyze
{
    namespace amqp10
    {

        class CompositeType;
        class FieldType;
        typedef std::vector<FieldType> fieldTypeList_t;
        class PrimitiveType;
        class FrameBuffer;

        class Type {
        public:
            Type();
            Type(const char* name);
            virtual ~Type();

            bool hasName() const;
            virtual inline bool isNull() const { return false; }
            const char* name() const;
            virtual std::string nameValueStr(bool colorFlag, const char* valueDelim = "()") const; // formatted string name(value) without margins
            virtual std::string nameTypeValueStr(bool colorFlag, const char* valueDelim = "()") const; // formatted string name:type(value) without margins
            virtual const amqp_provides_requires_list_t& providesList() const = 0;
            virtual const char* typeStr() const = 0; // type as string
            virtual std::string typeValueStr(bool colorFlag, const char* valueDelim = "()") const; // formatted string type(value) without margins
            virtual inline void validate(addErrorFp errorHandler, AmqpBlock* errorHandlerInstance) {}
            virtual std::string valueStr(bool colorFlag) const = 0; // value only as string

            static bool provides(amqpRequiresProvides_t requires, const amqp_provides_requires_list_t& prList);
            static std::string providesListAsString(const amqp_provides_requires_list_t& prList);

        protected:
            const char* _name;
        };


        //=========================
        //--- AMQP simple types ---
        //=========================

        enum class amqpPrimitiveType_t: uint8_t {
            // Primitive types
            NULL_TYPE,
            BOOLEAN_TYPE,
            UBYTE_TYPE,
            USHORT_TYPE,
            UINT_TYPE,
            ULONG_TYPE,
            BYTE_TYPE,
            SHORT_TYPE,
            INT_TYPE,
            LONG_TYPE,
            FLOAT_TYPE,
            DOUBLE_TYPE,
            DECIMAL32_TYPE,
            DECIMAL64_TYPE,
            DECIMAL128_TYPE,
            CHAR_TYPE,
            TIMESTAMP_TYPE,
            UUID_TYPE,
            BINARY_TYPE,
            STRING_TYPE,
            SYMBOL_TYPE,
            LIST_TYPE,
            MAP_TYPE,
            ARRAY_TYPE,
            // Restricted types
            ROLE_TYPE,
            SENDER_SETTLE_MODE_TYPE,
            RECEIVER_SETTLE_MODE_TYPE,
            HANDLE_TYPE,
            SECONDS_TYPE,
            MILLISECONDS_TYPE,
            DELIVERY_TAG_TYPE,
            DELIVERY_NUMBER_TYPE,
            TRANSFER_NUMBER_TYPE,
            SEQUENCE_NUMBER_TYPE,
            MESSAGE_FORMAT_TYPE,
            LANGUAGE_TAG_TYPE,
            FIELDS_TYPE,
            AMQP_ERROR_TYPE,
            CONNECTION_ERROR_TYPE,
            SESSION_ERROR_TYPE,
            LINK_ERROR_TYPE,
            ANNOTATIONS_TYPE,
            MESSAGE_ID_ULONG_TYPE,
            MESSAGE_ID_UUID_TYPE,
            MESSAGE_ID_BINARY_TYPE,
            MESSAGE_ID_STRING_TYPE,
            ADDRESS_TYPE,
            TERMINUS_DURABILITY_TYPE,
            TERMINUS_EXPIRY_POLICY_TYPE,
            STD_DIST_MODE_TYPE,
            FILTER_SET_TYPE,
            NODE_PROPERTIES_TYPE,
            TRANSACTION_ID_TYPE,
            TRANSACTION_CAPABILITY_TYPE,
            TRANSACTION_ERROR_TYPE,
            SASL_CODE_TYPE
        };
        class PrimitiveType: public Type {
        public:
            PrimitiveType();
            PrimitiveType(const char* name);
            virtual ~PrimitiveType();

            const inline amqp_provides_requires_list_t& providesList() const override { return s_providesList; }
            virtual amqpPrimitiveType_t type() const = 0;
            inline const char* typeStr() const override { return s_amqpSimpleTypeNames.at(type()); }
            static std::map<amqpPrimitiveType_t, const char*> s_amqpSimpleTypeNames;

        protected:
            static amqp_provides_requires_list_t s_providesList;
        };
        typedef PrimitiveType amqp_primitive_t;
        

        class AmqpNull: public PrimitiveType {
        public:
            AmqpNull();
            AmqpNull(const char* name);
            virtual ~AmqpNull();

            inline bool isNull() const { return true; }
            std::string nameValueStr(bool colorFlag, const char* valueDelim = "()") const override;
            std::string nameTypeValueStr(bool colorFlag, const char* valueDelim = "()") const override;
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::NULL_TYPE; }
            std::string typeValueStr(bool colorFlag, const char* valueDelim = "()") const override;
            std::string valueStr(bool colorFlag) const override;
        };


        typedef bool amqp_boolean_t;
        class AmqpBoolean: public PrimitiveType {
        public:
            AmqpBoolean(amqp_boolean_t v);
            AmqpBoolean(amqp_boolean_t v, const char* name);
            virtual ~AmqpBoolean();

            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::BOOLEAN_TYPE; }
            inline amqp_boolean_t value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            amqp_boolean_t _value;
        };


        typedef uint8_t amqp_ubyte_t;
        class AmqpUbyte: public PrimitiveType {
        public:
            AmqpUbyte(amqp_ubyte_t v);
            AmqpUbyte(amqp_ubyte_t v, const char* name);
            virtual ~AmqpUbyte();

            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::UBYTE_TYPE; }
            inline amqp_ubyte_t value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            amqp_ubyte_t _value;
        };


        typedef uint16_t amqp_ushort_t;
        class AmqpUshort: public PrimitiveType {
        public:
            AmqpUshort(amqp_ushort_t v);
            AmqpUshort(amqp_ushort_t v, const char* name);
            virtual ~AmqpUshort();

            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::USHORT_TYPE; }
            inline amqp_ushort_t value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            amqp_ushort_t _value;
        };


        typedef uint32_t amqp_uint_t;
        class AmqpUint: public PrimitiveType {
        public:
            AmqpUint(amqp_uint_t v);
            AmqpUint(amqp_uint_t v, const char* name);
            virtual ~AmqpUint();

            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::UINT_TYPE; }
            inline amqp_uint_t value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            amqp_uint_t _value;
        };


        typedef uint64_t amqp_ulong_t;
        class AmqpUlong: public PrimitiveType {
        public:
            AmqpUlong(amqp_ulong_t v);
            AmqpUlong(amqp_ulong_t v, const char* name);
            virtual ~AmqpUlong();

            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::ULONG_TYPE; }
            inline amqp_ulong_t value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            amqp_ulong_t _value;
        };


        typedef int8_t amqp_byte_t;
        class AmqpByte: public PrimitiveType {
        public:
            AmqpByte(amqp_byte_t v);
            AmqpByte(amqp_byte_t v, const char* name);
            virtual ~AmqpByte();

            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::BYTE_TYPE; }
            inline amqp_byte_t value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            amqp_byte_t _value;
        };


        typedef int16_t amqp_short_t;
        class AmqpShort: public PrimitiveType {
        public:
            AmqpShort(amqp_short_t v);
            AmqpShort(amqp_short_t v, const char* name);
            virtual ~AmqpShort();

            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::SHORT_TYPE; }
            inline amqp_short_t value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            amqp_short_t _value;
        };


        typedef int32_t amqp_int_t;
        class AmqpInt: public PrimitiveType {
        public:
            AmqpInt(amqp_int_t v);
            AmqpInt(amqp_int_t v, const char* name);
            virtual ~AmqpInt();

            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::INT_TYPE; }
            inline amqp_int_t value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            amqp_int_t _value;
        };


        typedef int64_t amqp_long_t;
        class AmqpLong: public PrimitiveType {
        public:
            AmqpLong(amqp_long_t v);
            AmqpLong(amqp_long_t v, const char* name);
            virtual ~AmqpLong();

            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::LONG_TYPE; }
            inline amqp_long_t value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            amqp_long_t _value;
        };


        typedef float amqp_float_t;
        class AmqpFloat: public PrimitiveType {
        public:
            AmqpFloat(amqp_float_t v);
            AmqpFloat(amqp_float_t v, const char* name);
            virtual ~AmqpFloat();

            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::FLOAT_TYPE; }
            inline amqp_float_t value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            amqp_float_t _value;
        };


        typedef double amqp_double_t;
        class AmqpDouble: public PrimitiveType {
        public:
            AmqpDouble(amqp_double_t v);
            AmqpDouble(amqp_double_t v, const char* name);
            virtual ~AmqpDouble();

            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::DOUBLE_TYPE; }
            inline amqp_double_t value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            amqp_double_t _value;
        };


        typedef std::array<uint8_t, 4> amqp_decimal32_t;
        class AmqpDecimal32: public PrimitiveType {
        public:
            AmqpDecimal32();
            AmqpDecimal32(const char* name);
            virtual ~AmqpDecimal32();

            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::DECIMAL32_TYPE; }
            inline std::string typeValueStr(bool colorFlag, const char* valueDelim = "()") const { return Type::typeValueStr("[]"); }
            inline amqp_decimal32_t& value() { return _value; }
            inline const amqp_decimal32_t& value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            amqp_decimal32_t _value;
        };


        typedef std::array<uint8_t, 8> amqp_decimal64_t;
        class AmqpDecimal64: public PrimitiveType {
        public:
            AmqpDecimal64();
            AmqpDecimal64(const char* name);
            virtual ~AmqpDecimal64();

            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::DECIMAL64_TYPE; }
            inline std::string typeValueStr(bool colorFlag, const char* valueDelim = "()") const { return Type::typeValueStr("[]"); }
            inline amqp_decimal64_t& value() { return _value; }
            inline const amqp_decimal64_t& value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            amqp_decimal64_t _value;
        };


        typedef std::array<uint8_t, 16> amqp_decimal128_t;
        class AmqpDecimal128: public PrimitiveType {
        public:
            AmqpDecimal128();
            AmqpDecimal128(const char* name);
            virtual ~AmqpDecimal128();

            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::DECIMAL128_TYPE; }
            inline std::string typeValueStr(bool colorFlag, const char* valueDelim = "()") const { return Type::typeValueStr("[]"); }
            inline amqp_decimal128_t& value() { return _value; }
            inline const amqp_decimal128_t& value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            amqp_decimal128_t _value;
        };


        typedef char32_t amqp_char_t;
        class AmqpChar: public PrimitiveType {
        public:
            AmqpChar(amqp_char_t v);
            AmqpChar(amqp_char_t v, const char* name);
            virtual ~AmqpChar();

            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::CHAR_TYPE; }
            inline amqp_char_t value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            amqp_char_t _value;
        };


        typedef uint64_t amqp_timestamp_t;
        class AmqpTimestamp: public PrimitiveType {
        public:
            AmqpTimestamp(amqp_timestamp_t v);
            AmqpTimestamp(amqp_timestamp_t v, const char* name);
            virtual ~AmqpTimestamp();

            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::TIMESTAMP_TYPE; }
            inline amqp_timestamp_t value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            amqp_timestamp_t _value;
        };


        typedef std::array<uint8_t, 16> amqp_uuid_t;
        class AmqpUuid: public PrimitiveType {
        public:
            AmqpUuid();
            AmqpUuid(const char* name);
            virtual ~AmqpUuid();

            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::UUID_TYPE; }
            inline amqp_uuid_t& value() { return _value; }
            inline const amqp_uuid_t& value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            amqp_uuid_t _value;
        };


        typedef std::vector<uint8_t> amqp_binary_t;
        typedef amqp_binary_t::iterator amqp_binary_itr_t;
        typedef amqp_binary_t::const_iterator amqp_binary_citr_t;
        class AmqpBinary: public PrimitiveType {
        public:
            AmqpBinary();
            AmqpBinary(const char* name);
            virtual ~AmqpBinary();

            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::BINARY_TYPE; }
            inline amqp_binary_t& value() { return _value; }
            inline const amqp_binary_t& value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            amqp_binary_t _value;
        };


        typedef std::string amqp_string_t;
        class AmqpString: public PrimitiveType {
        public:
            AmqpString();
            AmqpString(const char* name);
            virtual ~AmqpString();

            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::STRING_TYPE; }
            inline amqp_string_t& value() { return _value; }
            inline const amqp_string_t& value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            amqp_string_t _value;
        };


        typedef std::string amqp_symbol_t;
        class AmqpSymbol: public PrimitiveType {
        public:
            AmqpSymbol();
            AmqpSymbol(const char* name);
            virtual ~AmqpSymbol();

            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::SYMBOL_TYPE; }
            inline amqp_symbol_t& value() { return _value; }
            inline const amqp_symbol_t& value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            amqp_symbol_t _value;
        };


        //======================
        //--- Compound types ---
        //======================

        class CompoundType: public PrimitiveType {
        public:
            CompoundType();
            CompoundType(const char* name);
            virtual ~CompoundType();

            virtual void appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin, bool colorFlag) const = 0;

        protected:
            static void stringAppendHandler(std::ostringstream& oss, Type* ptr, std::size_t margin, bool nameFlag, bool colorFlag);
        };


        typedef std::vector<Type*> amqp_list_t;
        typedef amqp_list_t::iterator amqp_list_itr_t;
        typedef amqp_list_t::const_iterator amqp_list_citr_t;
        class AmqpList: public CompoundType {
        public:
            AmqpList();
            AmqpList(const char* name);
            virtual ~AmqpList();

            void appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin, bool colorFlag) const override;
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::LIST_TYPE; }
            inline std::string typeValueStr(bool colorFlag) const { return Type::typeValueStr(colorFlag, "[]"); }
            void validate(const fieldTypeList_t& fieldList, addErrorFp errorHandler, AmqpBlock* errorHandlerInstance);
            inline amqp_list_t& value() { return _value; }
            inline const amqp_list_t& value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            amqp_list_t _value;
        };


        typedef std::map<PrimitiveType*, PrimitiveType*> amqp_map_t;
        typedef amqp_map_t::iterator amqp_map_itr_t;
        typedef amqp_map_t::const_iterator amqp_map_citr_t;
        class AmqpMap: public CompoundType {
        public:
            AmqpMap();
            AmqpMap(const char* name);
            virtual ~AmqpMap();

            void appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin, bool colorFlag) const override;
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::MAP_TYPE; }
            inline std::string typeValueStr(bool colorFlag) const { return Type::typeValueStr(colorFlag, "{}"); }
            inline amqp_map_t& value() { return _value; }
            inline const amqp_map_t& value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            amqp_map_t _value;
        };


        typedef std::vector<PrimitiveType*> amqp_array_t;
        typedef amqp_array_t::iterator amqp_array_itr_t;
        typedef amqp_array_t::const_iterator amqp_array_citr_t;
        class AmqpArray: public CompoundType {
        public:
            AmqpArray();
            AmqpArray(const char* name);
            virtual ~AmqpArray();

            void appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin, bool colorFlag) const override;
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::ARRAY_TYPE; }
            inline std::string typeValueStr(bool colorFlag) const { return Type::typeValueStr(colorFlag, "[]"); }
            inline amqp_array_t& value() { return _value; }
            inline const amqp_array_t& value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            amqp_array_t _value;
        };


        //=============================
        //--- AMQP restricted types ---
        //=============================


        enum class amqp_role_t: amqp_boolean_t {
            SENDER=false,
            RECEIVER=true
        };
        class AmqpRole: public AmqpBoolean {
        public:
            AmqpRole(amqp_role_t v);
            AmqpRole(amqp_role_t v, const char* name);
            virtual ~AmqpRole();

            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::ROLE_TYPE; }
            inline amqp_role_t value() const { return (amqp_role_t)_value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            static std::map<amqp_role_t, const char*> s_choiceNames;
        };


        enum class amqp_sender_settle_mode_t:amqp_ubyte_t {
            UNSETTLED=0,
            SETTLED,
            MIXED
        };
        class AmqpSenderSettleMode: public AmqpUbyte {
        public:
            AmqpSenderSettleMode(amqp_sender_settle_mode_t v);
            AmqpSenderSettleMode(amqp_sender_settle_mode_t v, const char* name);
            virtual ~AmqpSenderSettleMode();

            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::SENDER_SETTLE_MODE_TYPE; }
            inline amqp_sender_settle_mode_t value() const { return (amqp_sender_settle_mode_t)_value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            static std::map<amqp_sender_settle_mode_t, const char*> s_choiceNames;
        };


        enum class amqp_receiver_settle_mode_t:amqp_ubyte_t {
            FIRST=0,
            SECOND
        };
        class AmqpReceiverSettleMode: public AmqpUbyte {
        public:
            AmqpReceiverSettleMode(amqp_receiver_settle_mode_t v);
            AmqpReceiverSettleMode(amqp_receiver_settle_mode_t v, const char* name);
            virtual ~AmqpReceiverSettleMode();

            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::RECEIVER_SETTLE_MODE_TYPE; }
            inline amqp_receiver_settle_mode_t value() const { return (amqp_receiver_settle_mode_t)_value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            static std::map<amqp_receiver_settle_mode_t, const char*> s_choiceNames;
        };


        typedef amqp_uint_t amqp_handle_t;
        class AmqpHandle: public AmqpUint {
        public:
            AmqpHandle(amqp_handle_t v);
            AmqpHandle(amqp_handle_t v, const char* name);
            virtual ~AmqpHandle();

            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::HANDLE_TYPE; }
            inline amqp_handle_t value() const { return _value; }
        };


        typedef amqp_uint_t amqp_seconds_t;
        class AmqpSeconds: public AmqpUint {
        public:
            AmqpSeconds(amqp_seconds_t v);
            AmqpSeconds(amqp_seconds_t v, const char* name);
            virtual ~AmqpSeconds();

            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::SECONDS_TYPE; }
            inline amqp_seconds_t value() const { return _value; }
        };


        typedef amqp_uint_t amqp_milliseconds_t;
        class AmqpMilliseconds: public AmqpUint {
        public:
            AmqpMilliseconds(amqp_milliseconds_t v);
            AmqpMilliseconds(amqp_milliseconds_t v, const char* name);
            virtual ~AmqpMilliseconds();

            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::MILLISECONDS_TYPE; }
            inline amqp_milliseconds_t value() const { return _value; }
        };


        typedef amqp_binary_t amqp_delivery_tag_t;
        typedef amqp_delivery_tag_t::iterator amqp_delivery_tag_itr_t;
        typedef amqp_delivery_tag_t::const_iterator amqp_delivery_tag_citr_t;
        class AmqpDeliveryTag: public AmqpBinary {
        public:
            AmqpDeliveryTag();
            AmqpDeliveryTag(const char* name);
            virtual ~AmqpDeliveryTag();

            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::DELIVERY_TAG_TYPE; }
            inline amqp_delivery_tag_t& value() { return _value; }
            inline const amqp_delivery_tag_t& value() const { return _value; }
        };


        typedef amqp_uint_t amqp_sequence_num_t;
        class AmqpSequenceNum: public AmqpUint {
        public:
            AmqpSequenceNum(amqp_sequence_num_t v);
            AmqpSequenceNum(amqp_sequence_num_t v, const char* name);
            virtual ~AmqpSequenceNum();

            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::DELIVERY_NUMBER_TYPE; }
            inline amqp_sequence_num_t value() const { return _value; }
        };


        typedef amqp_sequence_num_t amqp_delivery_num_t;
        class AmqpDeliveryNum: public AmqpSequenceNum {
        public:
            AmqpDeliveryNum(amqp_delivery_num_t v);
            AmqpDeliveryNum(amqp_delivery_num_t v, const char* name);
            virtual ~AmqpDeliveryNum();

            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::TRANSFER_NUMBER_TYPE; }
            inline amqp_delivery_num_t value() const { return _value; }
        };


        typedef amqp_sequence_num_t amqp_transfer_num_t;
        class AmqpTransferNum: public AmqpSequenceNum {
        public:
            AmqpTransferNum(amqp_transfer_num_t v);
            AmqpTransferNum(amqp_transfer_num_t v, const char* name);
            virtual ~AmqpTransferNum();

            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::SEQUENCE_NUMBER_TYPE; }
            inline amqp_transfer_num_t value() const { return _value; }
        };


        typedef amqp_uint_t amqp_message_format_t;
        class AmqpMessageFormat: public AmqpUint {
        public:
            AmqpMessageFormat(amqp_message_format_t v);
            AmqpMessageFormat(amqp_message_format_t v, const char* name);
            virtual ~AmqpMessageFormat();

            inline uint32_t getFormat() const { return (uint32_t)(_value >> 8); }
            inline uint8_t getVersion() const { return (uint8_t)(_value & uint32_t(0xff)); }
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::MESSAGE_FORMAT_TYPE; }
            inline amqp_message_format_t value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;
        };


        typedef amqp_symbol_t amqp_language_tag_t;
        class AmqpLanguageTag: public AmqpSymbol {
        public:
            AmqpLanguageTag();
            AmqpLanguageTag(const char* name);
            virtual ~AmqpLanguageTag();

            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::LANGUAGE_TAG_TYPE; }
            inline amqp_language_tag_t& value() { return _value; }
            inline const amqp_language_tag_t& value() const { return _value; }
        };


        typedef amqp_map_t amqp_fields_t;
        typedef amqp_fields_t::iterator amqp_fields_itr;
        typedef amqp_fields_t::const_iterator amqp_fields_citr;
        class AmqpFields: public AmqpMap {
        public:
            AmqpFields();
            AmqpFields(const char* name);
            virtual ~AmqpFields();

            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::FIELDS_TYPE; }
            inline amqp_fields_t& value() { return _value; }
            inline const amqp_fields_t& value() const { return _value; }
        };


        typedef amqp_symbol_t amqp_error_t;
        typedef std::vector<amqp_error_t> amqp_error_list_t;
        typedef amqp_error_list_t::const_iterator amqp_error_list_citr_t;
        class AmqpError: public AmqpSymbol {
        public:
            AmqpError(const amqp_error_t& v);
            AmqpError(const amqp_error_t& v, const char* name);
            virtual ~AmqpError();

            inline const amqp_provides_requires_list_t& providesList() const override { return s_providesList; }
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::AMQP_ERROR_TYPE; }
            inline amqp_error_t& value() { return _value; }
            inline const amqp_error_t& value() const { return _value; }

        protected:
            void checkAssignValue(const amqp_error_t& v);
            static amqp_provides_requires_list_t s_providesList;
            static amqp_error_list_t s_amqpErrors;
        };


        typedef amqp_symbol_t amqp_connection_error_t;
        typedef std::vector<amqp_error_t> amqp_connection_error_list_t;
        typedef amqp_connection_error_list_t::const_iterator amqp_connection_error_list_citr_t;
        class AmqpConnectionError: public AmqpSymbol {
        public:
            AmqpConnectionError(const amqp_connection_error_t& v);
            AmqpConnectionError(const amqp_connection_error_t& v, const char* name);
            virtual ~AmqpConnectionError();

            inline const amqp_provides_requires_list_t& providesList() const override { return s_providesList; }
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::CONNECTION_ERROR_TYPE; }
            inline amqp_connection_error_t& value() { return _value; }
            inline const amqp_connection_error_t& value() const { return _value; }

        protected:
            void checkAssignValue(const amqp_connection_error_t& v);
            static amqp_provides_requires_list_t s_providesList;
            static amqp_connection_error_list_t s_amqpConnectionErrors;
        };


        typedef amqp_symbol_t amqp_session_error_t;
        typedef std::vector<amqp_error_t> amqp_session_error_list_t;
        typedef amqp_session_error_list_t::const_iterator amqp_session_error_list_citr_t;
        class AmqpSessionError: public AmqpSymbol {
        public:
            AmqpSessionError(const amqp_session_error_t& v);
            AmqpSessionError(const amqp_session_error_t& v, const char* name);
            virtual ~AmqpSessionError();

            inline const amqp_provides_requires_list_t& providesList() const override { return s_providesList; }
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::SESSION_ERROR_TYPE; }
            inline amqp_session_error_t& value() { return _value; }
            inline const amqp_session_error_t& value() const { return _value; }

        protected:
            void checkAssignValue(const amqp_session_error_t& v);
            static amqp_provides_requires_list_t s_providesList;
            static amqp_session_error_list_t s_amqpSessionErrors;
        };


        typedef amqp_symbol_t amqp_link_error_t;
        typedef std::vector<amqp_error_t> amqp_link_error_list_t;
        typedef amqp_link_error_list_t::const_iterator amqp_link_error_list_citr_t;
        class AmqpLinkError: public AmqpSymbol {
        public:
            AmqpLinkError(const amqp_link_error_t& v);
            AmqpLinkError(const amqp_link_error_t& v, const char* name);
            virtual ~AmqpLinkError();

            inline const amqp_provides_requires_list_t& providesList() const override { return s_providesList; }
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::LINK_ERROR_TYPE; }
            inline amqp_link_error_t& value() { return _value; }
            inline const amqp_link_error_t& value() const { return _value; }

        protected:
            void checkAssignValue(const amqp_link_error_t& v);
            static amqp_provides_requires_list_t s_providesList;
            static amqp_link_error_list_t s_amqpLinkErrors;
        };


        typedef amqp_map_t amqp_annotations_t;
        typedef amqp_annotations_t::iterator amqp_annotations_itr_t;
        typedef amqp_annotations_t::const_iterator amqp_annotations_citr_t;
        class AmqpAnnotations: public AmqpMap {
        public:
            AmqpAnnotations();
            AmqpAnnotations(const char* name);
            virtual ~AmqpAnnotations();

            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::ANNOTATIONS_TYPE; }
            inline amqp_annotations_t& value() { return _value; }
            inline const amqp_annotations_t& value() const { return _value; }
        };




        typedef amqp_ulong_t amqp_msg_id_ulong_t;
        class AmqpMessageIdUlong: public AmqpUlong {
        public:
            AmqpMessageIdUlong(amqp_msg_id_ulong_t v);
            AmqpMessageIdUlong(amqp_msg_id_ulong_t v, const char* name);
            virtual ~AmqpMessageIdUlong();

            inline const amqp_provides_requires_list_t& providesList() const override { return s_providesList; }
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::MESSAGE_ID_ULONG_TYPE; }
            inline amqp_msg_id_ulong_t value() const { return _value; }

        protected:
            static amqp_provides_requires_list_t s_providesList;
        };


        typedef amqp_uuid_t amqp_msg_id_uuid_t;
        class AmqpMessageIdUuid: public AmqpUuid {
        public:
            AmqpMessageIdUuid();
            AmqpMessageIdUuid(const char* name);
            virtual ~AmqpMessageIdUuid();

            inline const amqp_provides_requires_list_t& providesList() const override { return s_providesList; }
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::MESSAGE_ID_UUID_TYPE; }
            inline amqp_msg_id_uuid_t& value() { return _value; }
            inline const amqp_msg_id_uuid_t& value() const { return _value; }

        protected:
            static amqp_provides_requires_list_t s_providesList;
        };


        typedef amqp_binary_t amqp_msg_id_binary_t;
        class AmqpMessageIdBinary: public AmqpBinary {
        public:
            AmqpMessageIdBinary();
            AmqpMessageIdBinary(const char* name);
            virtual ~AmqpMessageIdBinary();

            inline const amqp_provides_requires_list_t& providesList() const override { return s_providesList; }
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::MESSAGE_ID_BINARY_TYPE; }
            inline amqp_msg_id_binary_t& value() { return _value; }
            inline const amqp_msg_id_binary_t& value() const { return _value; }

        protected:
            static amqp_provides_requires_list_t s_providesList;
        };


        typedef amqp_string_t amqp_msg_id_string_t;
        class AmqpMessageIdString: public AmqpString {
        public:
            AmqpMessageIdString();
            AmqpMessageIdString(const char* name);
            virtual ~AmqpMessageIdString();

            inline const amqp_provides_requires_list_t& providesList() const override { return s_providesList; }
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::MESSAGE_ID_STRING_TYPE; }
            inline amqp_msg_id_string_t& value() { return _value; }
            inline const amqp_msg_id_string_t& value() const { return _value; }

        protected:
            static amqp_provides_requires_list_t s_providesList;
       };


        typedef amqp_string_t amqp_address_t;
        class AmqpAddress: public AmqpString {
        public:
            AmqpAddress();
            AmqpAddress(const char* name);
            virtual ~AmqpAddress();

            const inline amqp_provides_requires_list_t& providesList() const override { return s_providesList; }
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::ADDRESS_TYPE; }
            inline amqp_address_t& value() { return _value; }
            inline const amqp_address_t& value() const { return _value; }

        protected:
            static amqp_provides_requires_list_t s_providesList;
        };

        enum class amqp_terminus_durability_t:amqp_uint_t {
            NONE=0,
            CONFIGURATION,
            UNSETTLED_STATE
        };
        class AmqpTerminusDurability: public AmqpUint {
        public:
            AmqpTerminusDurability(amqp_terminus_durability_t v);
            AmqpTerminusDurability(amqp_terminus_durability_t v, const char* name);
            virtual ~AmqpTerminusDurability();

            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::TERMINUS_DURABILITY_TYPE; }
            inline amqp_terminus_durability_t value() const { return (amqp_terminus_durability_t)_value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            static std::map<amqp_terminus_durability_t, const char*> s_choiceNames;
        };


        typedef amqp_symbol_t amqp_terminus_expiry_policy_t;
        typedef std::vector<amqp_terminus_expiry_policy_t> amqp_terminus_expiry_policy_list_t;
        typedef amqp_terminus_expiry_policy_list_t::const_iterator amqp_terminus_expiry_policy_list_citr_t;
        class AmqpTerminusExpiryPolicy: public AmqpSymbol {
        public:
            AmqpTerminusExpiryPolicy(const amqp_terminus_expiry_policy_t& v);
            AmqpTerminusExpiryPolicy(const amqp_terminus_expiry_policy_t& v, const char* name);
            virtual ~AmqpTerminusExpiryPolicy();

            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::TERMINUS_EXPIRY_POLICY_TYPE; }
            inline amqp_terminus_expiry_policy_t& value() { return _value; }
            inline const amqp_terminus_expiry_policy_t& value() const { return _value; }

        protected:
            void checkAssignValue(const amqp_terminus_expiry_policy_t& v);
            static amqp_terminus_expiry_policy_list_t s_terminusExpiryPolicyList;
        };


        typedef amqp_symbol_t amqp_std_dist_mode_t;
        typedef std::vector<amqp_std_dist_mode_t> amqp_std_dist_mode_list_t;
        typedef amqp_std_dist_mode_list_t::const_iterator amqp_std_dist_mode_list_citr_tt;
        class AmqpStandardDistributionMode: public  AmqpSymbol {
        public:
            AmqpStandardDistributionMode(const amqp_std_dist_mode_t& v);
            AmqpStandardDistributionMode(const amqp_std_dist_mode_t& v, const char* name);
            virtual ~AmqpStandardDistributionMode();

            inline const amqp_provides_requires_list_t& providesList() const override { return s_providesList; }
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::STD_DIST_MODE_TYPE; }
            inline amqp_std_dist_mode_t& value() { return _value; }
            inline const amqp_std_dist_mode_t& value() const { return _value; }

        protected:
            void checkAssignValue(const amqp_std_dist_mode_t& v);
            static amqp_std_dist_mode_list_t s_StandardDistributionModeList;
            static amqp_provides_requires_list_t s_providesList;
        };


        typedef amqp_map_t amqp_filter_set_t;
        typedef amqp_filter_set_t::iterator amqp_filter_set_itr_t;
        typedef amqp_filter_set_t::const_iterator amqp_filter_set_citr_t;
        class AmqpFilterSet: public AmqpMap {
        public:
            AmqpFilterSet();
            AmqpFilterSet(const char* name);
            virtual ~AmqpFilterSet();

            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::FILTER_SET_TYPE; }
            inline amqp_filter_set_t& value() { return _value; }
            inline const amqp_filter_set_t& value() const { return _value; }
        };


        typedef amqp_fields_t amqp_node_properties_t;
        typedef amqp_node_properties_t::iterator amqp_node_properties_itr_t;
        typedef amqp_node_properties_t::const_iterator amqp_node_properties_citr_t;
        class AmqpNodeProperties: public AmqpFields {
        public:
            AmqpNodeProperties();
            AmqpNodeProperties(const char* name);
            virtual ~AmqpNodeProperties();

            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::NODE_PROPERTIES_TYPE; }
            inline amqp_node_properties_t& value() { return _value; }
            inline const amqp_node_properties_t& value() const { return _value; }
        };


        typedef amqp_binary_t amqp_transaction_id_t;
        typedef amqp_transaction_id_t::iterator amqp_transaction_id_itr_t;
        typedef amqp_transaction_id_t::const_iterator amqp_transaction_id_citr_t;
        class AmqpTransactionId: public AmqpBinary {
        public:
            AmqpTransactionId();
            AmqpTransactionId(const char* name);
            virtual ~AmqpTransactionId();

            inline const amqp_provides_requires_list_t& providesList() const override { return s_providesList; }
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::TRANSACTION_ID_TYPE; }
            inline amqp_transaction_id_t& value() { return _value; }
            inline const amqp_transaction_id_t& value() const { return _value; }

        protected:
        	static amqp_provides_requires_list_t s_providesList;
        };


        typedef amqp_symbol_t amqp_transaction_capability_t;
        typedef std::vector<amqp_transaction_capability_t> amqp_transaction_capability_list_t;
        typedef amqp_transaction_capability_list_t::const_iterator amqp_transaction_capability_list_citr_t;
        class AmqpTransactionalCapability: public AmqpSymbol {
        public:
            AmqpTransactionalCapability(const amqp_transaction_capability_t& v);
            AmqpTransactionalCapability(const amqp_transaction_capability_t& v, const char* name);
            virtual ~AmqpTransactionalCapability();

            inline const amqp_provides_requires_list_t& providesList() const override { return s_providesList; }
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::TRANSACTION_CAPABILITY_TYPE; }
            inline amqp_transaction_capability_t& value() { return _value; }
            inline const amqp_transaction_capability_t& value() const { return _value; }

        protected:
            void checkAssignValue(const amqp_transaction_capability_t& v);
            static amqp_transaction_capability_list_t s_transactionCapabilityList;
            static amqp_provides_requires_list_t s_providesList;
        };


        typedef amqp_symbol_t amqp_transaction_error_t;
        typedef std::vector<amqp_transaction_error_t> amqp_transaction_error_list_t;
        typedef amqp_transaction_error_list_t::const_iterator amqp_transaction_error_list_citr_t;
        class AmqpTransactionError: public AmqpSymbol {
        public:
            AmqpTransactionError(const amqp_transaction_error_t& v);
            AmqpTransactionError(const amqp_transaction_error_t& v, const char* name);
            virtual ~AmqpTransactionError();

            inline const amqp_provides_requires_list_t& providesList() const override { return s_providesList; }
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::TRANSACTION_ERROR_TYPE; }
            inline amqp_transaction_error_t& value() { return _value; }
            inline const amqp_transaction_error_t& value() const { return _value; }

        protected:
            static amqp_transaction_error_list_t s_transactionErrorList;
            static amqp_provides_requires_list_t s_providesList;
        };


        enum class amqp_sasl_code_t:amqp_ubyte_t {
            OK=0,
            AUTH,
            SYS,
            SYS_PERM,
            SYS_TEMP
        };
        class AmqpSaslCode: public AmqpUbyte {
        public:
            AmqpSaslCode(amqp_sasl_code_t v);
            AmqpSaslCode(amqp_sasl_code_t v, const char* name);
            virtual ~AmqpSaslCode();

            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::SASL_CODE_TYPE; }
            inline amqp_sasl_code_t value() const { return (amqp_sasl_code_t)_value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            static std::map<amqp_sasl_code_t, const char*> s_choiceNames;
        };




        //=========================================
        //--- AMQP composite types and sections ---
        //=========================================


        enum class amqpCompositeType_t:uint64_t {
            ERROR = 0x1d,
            RECEIVED = 0x23,
            ACCEPTED,
            REJECTED,
            RELEASED,
            MODIFIED,
            SOURCE,
            TARGET,
            DELETE_ON_CLOSE = 0x2b,
            DELETE_ON_NO_LINKS,
            DELETE_ON_NO_MESSAGES,
            DELETE_ON_NO_LINKS_OR_MESSAGES,
            COORDINATOR = 0x30,
            DECLARE,
            DISCHARGE,
            DECLARED,
            TRANSACTIONAL_STATE,
            SASL_MECHANISMS = 0x40,
            SASL_INIT,
            SASL_CHALLENGE,
            SASL_RESPONSE,
            SASL_OUTCOME
        };


        class CompositeType: public Type {
        public:
            CompositeType(AmqpList* fieldListPtr);
            CompositeType(AmqpList* fieldListPtr, const char* name);
            virtual ~CompositeType();

            std::ostringstream& appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin, bool colorFlag) const;
            inline const AmqpList* fieldList() const { return _fieldListPtr; }
            virtual const fieldTypeList_t& fieldTypeList() const = 0;
            inline const amqp_provides_requires_list_t& providesList() const { return s_providesList; }
            virtual amqpCompositeType_t type() const = 0;
            inline const char* typeStr() const override { return s_amqpCompositeTypeNames.at(type()); }
            virtual void validate(addErrorFp errorHandler, AmqpBlock* errorHandlerInstance) override;
            std::string valueStr(bool colorFlag) const override;
            static std::map<amqpCompositeType_t, const char*> s_amqpCompositeTypeNames;

        protected:
            AmqpList* _fieldListPtr;
            static amqp_provides_requires_list_t s_providesList;
       };


        class AmqpErrorRecord: public CompositeType {
        public:
            AmqpErrorRecord(AmqpList* fieldList);
            AmqpErrorRecord(AmqpList* fieldList, const char* name);
            virtual ~AmqpErrorRecord();

            inline const fieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; }
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::ERROR; }
            static const fieldTypeList_t s_fieldTypeList;
        };


        class AmqpReceived: public CompositeType {
        public:
            AmqpReceived(AmqpList* fieldList);
            AmqpReceived(AmqpList* fieldList, const char* name);
            virtual ~AmqpReceived();

            inline const fieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; }
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::RECEIVED; }
            const inline amqp_provides_requires_list_t& providesList() const override { return s_providesList; }
            static const fieldTypeList_t s_fieldTypeList;

        protected:
        	static amqp_provides_requires_list_t s_providesList;
        };


        class AmqpAccepted: public CompositeType {
        public:
            AmqpAccepted(AmqpList* fieldList);
            AmqpAccepted(AmqpList* fieldList, const char* name);
            virtual ~AmqpAccepted();

            inline const fieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; }
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::ACCEPTED; }
            const inline amqp_provides_requires_list_t& providesList() const override { return s_providesList; }
            static const fieldTypeList_t s_fieldTypeList;

        protected:
        	static amqp_provides_requires_list_t s_providesList;
        };


        class AmqpRejected: public CompositeType {
        public:
            AmqpRejected(AmqpList* fieldList);
            AmqpRejected(AmqpList* fieldList, const char* name);
            virtual ~AmqpRejected();

            inline const fieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; }
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::REJECTED; }
            const inline amqp_provides_requires_list_t& providesList() const override { return s_providesList; }
            static const fieldTypeList_t s_fieldTypeList;

        protected:
        	static amqp_provides_requires_list_t s_providesList;
        };


        class AmqpReleased: public CompositeType {
        public:
            AmqpReleased(AmqpList* fieldList);
            AmqpReleased(AmqpList* fieldList, const char* name);
            virtual ~AmqpReleased();

            inline const fieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; }
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::RELEASED; }
            const inline amqp_provides_requires_list_t& providesList() const override { return s_providesList; }
            static const fieldTypeList_t s_fieldTypeList;

        protected:
        	static amqp_provides_requires_list_t s_providesList;
        };


        class AmqpModified: public CompositeType {
        public:
            AmqpModified(AmqpList* fieldList);
            AmqpModified(AmqpList* fieldList, const char* name);
            virtual ~AmqpModified();

            inline const fieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; }
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::MODIFIED; }
            const inline amqp_provides_requires_list_t& providesList() const override { return s_providesList; }
            static const fieldTypeList_t s_fieldTypeList;

        protected:
        	static amqp_provides_requires_list_t s_providesList;
        };


        class AmqpSource: public CompositeType {
        public:
            AmqpSource(AmqpList* fieldList);
            AmqpSource(AmqpList* fieldList, const char* name);
            virtual ~AmqpSource();

            inline const fieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; }
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::SOURCE; }
            const inline amqp_provides_requires_list_t& providesList() const override { return s_providesList; }
            static const fieldTypeList_t s_fieldTypeList;

        protected:
        	static amqp_provides_requires_list_t s_providesList;
        };


        class AmqpTarget: public CompositeType {
        public:
            AmqpTarget(AmqpList* fieldList);
            AmqpTarget(AmqpList* fieldList, const char* name);
            virtual ~AmqpTarget();

            inline const fieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; }
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::TARGET; }
            const inline amqp_provides_requires_list_t& providesList() const override { return s_providesList; }
            static const fieldTypeList_t s_fieldTypeList;

        protected:
        	static amqp_provides_requires_list_t s_providesList;
        };

        class AmqpDeleteOnClose: public CompositeType {
        public:
            AmqpDeleteOnClose(AmqpList* fieldList);
            AmqpDeleteOnClose(AmqpList* fieldList, const char* name);
            virtual ~AmqpDeleteOnClose();

            inline const fieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; }
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::DELETE_ON_CLOSE; }
            static const fieldTypeList_t s_fieldTypeList;
        };


        class AmqpDeleteOnNoLinks: public CompositeType {
        public:
            AmqpDeleteOnNoLinks(AmqpList* fieldList);
            AmqpDeleteOnNoLinks(AmqpList* fieldList, const char* name);
            virtual ~AmqpDeleteOnNoLinks();

            inline const fieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; }
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::DELETE_ON_NO_LINKS; }
            static const fieldTypeList_t s_fieldTypeList;
        };


        class AmqpDeleteOnNoMessages: public CompositeType {
        public:
            AmqpDeleteOnNoMessages(AmqpList* fieldList);
            AmqpDeleteOnNoMessages(AmqpList* fieldList, const char* name);
            virtual ~AmqpDeleteOnNoMessages();

            inline const fieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; }
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::DELETE_ON_NO_MESSAGES; }
            static const fieldTypeList_t s_fieldTypeList;
        };


        class AmqpDeleteOnNoLinksOrMessages: public CompositeType {
        public:
            AmqpDeleteOnNoLinksOrMessages(AmqpList* fieldList);
            AmqpDeleteOnNoLinksOrMessages(AmqpList* fieldList, const char* name);
            virtual ~AmqpDeleteOnNoLinksOrMessages();

            inline const fieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; }
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::DELETE_ON_NO_LINKS_OR_MESSAGES; }
            static const fieldTypeList_t s_fieldTypeList;
        };


        class AmqpCoordinator: public CompositeType {
        public:
            AmqpCoordinator(AmqpList* fieldList);
            AmqpCoordinator(AmqpList* fieldList, const char* name);
            virtual ~AmqpCoordinator();

            inline const fieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; }
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::COORDINATOR; }
            const inline amqp_provides_requires_list_t& providesList() const override { return s_providesList; }
            static const fieldTypeList_t s_fieldTypeList;

        protected:
        	static amqp_provides_requires_list_t s_providesList;
        };


        class AmqpDeclare: public CompositeType {
        public:
            AmqpDeclare(AmqpList* fieldList);
            AmqpDeclare(AmqpList* fieldList, const char* name);
            virtual ~AmqpDeclare();

            inline const fieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; }
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::DECLARE; }
            static const fieldTypeList_t s_fieldTypeList;
        };


        class AmqpDischarge: public CompositeType {
        public:
            AmqpDischarge(AmqpList* fieldList);
            AmqpDischarge(AmqpList* fieldList, const char* name);
            virtual ~AmqpDischarge();

            inline const fieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; }
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::DISCHARGE; }
            static const fieldTypeList_t s_fieldTypeList;
        };


        class AmqpDeclared: public CompositeType {
        public:
            AmqpDeclared(AmqpList* fieldList);
            AmqpDeclared(AmqpList* fieldList, const char* name);
            virtual ~AmqpDeclared();

            inline const fieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; }
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::DECLARED; }
            const inline amqp_provides_requires_list_t& providesList() const override { return s_providesList; }
            static const fieldTypeList_t s_fieldTypeList;

        protected:
        	static amqp_provides_requires_list_t s_providesList;
        };


        class AmqpTransactionalState: public CompositeType {
        public:
            AmqpTransactionalState(AmqpList* fieldList);
            AmqpTransactionalState(AmqpList* fieldList, const char* name);
            virtual ~AmqpTransactionalState();

            inline const fieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; }
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::TRANSACTIONAL_STATE; }
            const inline amqp_provides_requires_list_t& providesList() const override { return s_providesList; }
            static const fieldTypeList_t s_fieldTypeList;

        protected:
        	static amqp_provides_requires_list_t s_providesList;
        };


        class AmqpSaslMechanisms: public CompositeType {
        public:
            AmqpSaslMechanisms(AmqpList* fieldList);
            AmqpSaslMechanisms(AmqpList* fieldList, const char* name);
            virtual ~AmqpSaslMechanisms();

            inline const fieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; }
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::SASL_MECHANISMS; }
            static const fieldTypeList_t s_fieldTypeList;
        };


        class AmqpSaslInit: public CompositeType {
        public:
            AmqpSaslInit(AmqpList* fieldList);
            AmqpSaslInit(AmqpList* fieldList, const char* name);
            virtual ~AmqpSaslInit();

            inline const fieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; }
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::SASL_INIT; }
            static const fieldTypeList_t s_fieldTypeList;
        };


        class AmqpSaslChallenge: public CompositeType {
        public:
            AmqpSaslChallenge(AmqpList* fieldList);
            AmqpSaslChallenge(AmqpList* fieldList, const char* name);
            virtual ~AmqpSaslChallenge();

            inline const fieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; }
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::SASL_CHALLENGE; }
            static const fieldTypeList_t s_fieldTypeList;
        };


        class AmqpSaslResponse: public CompositeType {
        public:
            AmqpSaslResponse(AmqpList* fieldList);
            AmqpSaslResponse(AmqpList* fieldList, const char* name);
            virtual ~AmqpSaslResponse();

            inline const fieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; }
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::SASL_RESPONSE; }
            static const fieldTypeList_t s_fieldTypeList;
        };


        class AmqpSaslOutcome: public CompositeType {
        public:
            AmqpSaslOutcome(AmqpList* fieldList);
            AmqpSaslOutcome(AmqpList* fieldList, const char* name);
            virtual ~AmqpSaslOutcome();

            inline const fieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; }
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::SASL_OUTCOME; }
            static const fieldTypeList_t s_fieldTypeList;
        };


    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQP10_TYPE_HPP_ */
