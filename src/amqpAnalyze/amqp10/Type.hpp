/*
 * AmqpTypes.hpp
 *
 *  Created on: Apr 5, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_AMQP10_TYPE_HPP_
#define SRC_AMQPANALYZE_AMQP10_TYPE_HPP_

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
        class PrimitiveType;
        class Buffer;

        // List of requires and provides values
        enum class amqpRequiresProvides_t {
            SOURCE,
            TARGET,
            DELIVERY_STATE,
            ERROR_CONDITION,
            MESSAGE_ID,
            ADDRESS,
            DISTRIBUTION_MODE,
            OUTCOME,
            TXN_CAPABILITY,
			TXN_ID,
            GLOBAL_TX_ID
        };
        typedef std::vector<amqpRequiresProvides_t> amqp_provides_requires_list_t;
        typedef amqp_provides_requires_list_t::const_iterator amqp_provides_requires_list_citr_t;


        class Type {
        protected:
        	static amqp_provides_requires_list_t s_providesList;
        public:
            Type();
            virtual ~Type();
            virtual bool isComposite() const = 0;
            virtual const char* typeStr() const = 0; // type as string
            std::string typeValueStr(const char* valueDelim = "()") const; // formatted string type(value) without margins
            virtual std::string valueStr() const = 0; // value only as string

            static Type* decode(Buffer& b);
            static PrimitiveType* decodePrimitive(uint8_t code, Buffer& b);
            static CompositeType* decodeComposite(Buffer& b);

        //protected:
            //virtual std::string toString(const char* name, std::size_t margin) const = 0;
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
            DELIVERY_ANNOTATIONS_TYPE,
            MESSAGE_ANNOTATIONS_TYPE,
            APPLICATION_PROPERTIES_TYPE,
            RAW_DATA_TYPE,
            AMQP_SEQUENCE_TYPE,
            // AMQP_VALUE_TYPE, // TODO
            FOOTER_TYPE,
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
            virtual ~PrimitiveType();
            inline bool isComposite() const override { return false; };
            virtual amqpPrimitiveType_t type() const = 0;
            inline const char* typeStr() const override { return s_amqpSimpleTypeNames.at(type()); }

            static std::map<amqpPrimitiveType_t, const char*> s_amqpSimpleTypeNames;
        protected:
            //std::string toString(const char* name, std::size_t margin) const override;
        };
        typedef PrimitiveType amqp_primitive_t;
        

        class AmqpNull: public PrimitiveType {
        public:
            AmqpNull();
            virtual ~AmqpNull();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::NULL_TYPE; }
            std::string valueStr() const override;
        };


        typedef bool amqp_boolean_t;
        class AmqpBoolean: public PrimitiveType {
        protected:
            amqp_boolean_t _value;
        public:
            AmqpBoolean(amqp_boolean_t v);
            virtual ~AmqpBoolean();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::BOOLEAN_TYPE; }
            inline amqp_boolean_t value() const { return _value; }
            std::string valueStr() const override;
        };


        typedef uint8_t amqp_ubyte_t;
        class AmqpUbyte: public PrimitiveType {
        protected:
            amqp_ubyte_t _value;
        public:
            AmqpUbyte(amqp_ubyte_t v);
            virtual ~AmqpUbyte();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::UBYTE_TYPE; }
            inline amqp_ubyte_t value() const { return _value; }
            std::string valueStr() const override;
        };


        typedef uint16_t amqp_ushort_t;
        class AmqpUshort: public PrimitiveType {
        protected:
            amqp_ushort_t _value;
        public:
            AmqpUshort(amqp_ushort_t v);
            virtual ~AmqpUshort();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::USHORT_TYPE; }
            inline amqp_ushort_t value() const { return _value; }
            std::string valueStr() const override;
        };


        typedef uint32_t amqp_uint_t;
        class AmqpUint: public PrimitiveType {
        protected:
            amqp_uint_t _value;
        public:
            AmqpUint(amqp_uint_t v);
            virtual ~AmqpUint();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::UINT_TYPE; }
            inline amqp_uint_t value() const { return _value; }
            std::string valueStr() const override;
        };


        typedef uint64_t amqp_ulong_t;
        class AmqpUlong: public PrimitiveType {
        protected:
            amqp_ulong_t _value;
        public:
            AmqpUlong(amqp_ulong_t v);
            virtual ~AmqpUlong();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::ULONG_TYPE; }
            inline amqp_ulong_t value() const { return _value; }
            std::string valueStr() const override;
        };


        typedef int8_t amqp_byte_t;
        class AmqpByte: public PrimitiveType {
        protected:
            amqp_byte_t _value;
        public:
            AmqpByte(amqp_byte_t v);
            virtual ~AmqpByte();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::BYTE_TYPE; }
            inline amqp_byte_t value() const { return _value; }
            std::string valueStr() const override;
        };


        typedef int16_t amqp_short_t;
        class AmqpShort: public PrimitiveType {
        protected:
            amqp_short_t _value;
        public:
            AmqpShort(amqp_short_t v);
            virtual ~AmqpShort();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::SHORT_TYPE; }
            inline amqp_short_t value() const { return _value; }
            std::string valueStr() const override;
        };


        typedef int32_t amqp_int_t;
        class AmqpInt: public PrimitiveType {
        protected:
            amqp_int_t _value;
        public:
            AmqpInt(amqp_int_t v);
            virtual ~AmqpInt();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::INT_TYPE; }
            inline amqp_int_t value() const { return _value; }
            std::string valueStr() const override;
        };


        typedef int64_t amqp_long_t;
        class AmqpLong: public PrimitiveType {
        protected:
            amqp_long_t _value;
        public:
            AmqpLong(amqp_long_t v);
            virtual ~AmqpLong();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::LONG_TYPE; }
            inline amqp_long_t value() const { return _value; }
            std::string valueStr() const override;
        };


        typedef float amqp_float_t;
        class AmqpFloat: public PrimitiveType {
        protected:
            amqp_float_t _value;
        public:
            AmqpFloat(amqp_float_t v);
            virtual ~AmqpFloat();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::FLOAT_TYPE; }
            inline amqp_float_t value() const { return _value; }
            std::string valueStr() const override;
        };


        typedef double amqp_double_t;
        class AmqpDouble: public PrimitiveType {
        protected:
            amqp_double_t _value;
        public:
            AmqpDouble(amqp_double_t v);
            virtual ~AmqpDouble();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::DOUBLE_TYPE; }
            inline amqp_double_t value() const { return _value; }
            std::string valueStr() const override;
        };


        typedef std::array<uint8_t, 4> amqp_decimal32_t;
        class AmqpDecimal32: public PrimitiveType {
        protected:
            amqp_decimal32_t _value;
        public:
            AmqpDecimal32();
            virtual ~AmqpDecimal32();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::DECIMAL32_TYPE; }
            inline std::string typeValueStr(const char* valueDelim = "()") const { return Type::typeValueStr("[]"); }
            inline amqp_decimal32_t& value() { return _value; }
            inline const amqp_decimal32_t& value() const { return _value; }
            std::string valueStr() const override;
        };


        typedef std::array<uint8_t, 8> amqp_decimal64_t;
        class AmqpDecimal64: public PrimitiveType {
        protected:
            amqp_decimal64_t _value;
        public:
            AmqpDecimal64();
            virtual ~AmqpDecimal64();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::DECIMAL64_TYPE; }
            inline std::string typeValueStr(const char* valueDelim = "()") const { return Type::typeValueStr("[]"); }
            inline amqp_decimal64_t& value() { return _value; }
            inline const amqp_decimal64_t& value() const { return _value; }
            std::string valueStr() const override;
        };


        typedef std::array<uint8_t, 16> amqp_decimal128_t;
        class AmqpDecimal128: public PrimitiveType {
        protected:
            amqp_decimal128_t _value;
        public:
            AmqpDecimal128();
            virtual ~AmqpDecimal128();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::DECIMAL128_TYPE; }
            inline std::string typeValueStr(const char* valueDelim = "()") const { return Type::typeValueStr("[]"); }
            inline amqp_decimal128_t& value() { return _value; }
            inline const amqp_decimal128_t& value() const { return _value; }
            std::string valueStr() const override;
        };


        typedef char32_t amqp_char_t;
        class AmqpChar: public PrimitiveType {
        protected:
            amqp_char_t _value;
        public:
            AmqpChar(amqp_char_t v);
            virtual ~AmqpChar();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::CHAR_TYPE; }
            inline amqp_char_t value() const { return _value; }
            std::string valueStr() const override;
        };


        typedef uint64_t amqp_timestamp_t;
        class AmqpTimestamp: public PrimitiveType {
        protected:
            amqp_timestamp_t _value;
        public:
            AmqpTimestamp(amqp_timestamp_t v);
            virtual ~AmqpTimestamp();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::TIMESTAMP_TYPE; }
            inline amqp_timestamp_t value() const { return _value; }
            std::string valueStr() const override;
        };


        typedef std::array<uint8_t, 16> amqp_uuid_t;
        class AmqpUuid: public PrimitiveType {
        protected:
            amqp_uuid_t _value;
        public:
            AmqpUuid();
            virtual ~AmqpUuid();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::UUID_TYPE; }
            inline amqp_uuid_t& value() { return _value; }
            inline const amqp_uuid_t& value() const { return _value; }
            std::string valueStr() const override;
        };


        typedef std::vector<uint8_t> amqp_binary_t;
        typedef amqp_binary_t::iterator amqp_binary_itr_t;
        typedef amqp_binary_t::const_iterator amqp_binary_citr_t;
        class AmqpBinary: public PrimitiveType {
        protected:
            amqp_binary_t _value;
        public:
            AmqpBinary();
            virtual ~AmqpBinary();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::BINARY_TYPE; }
            inline amqp_binary_t& value() { return _value; }
            inline const amqp_binary_t& value() const { return _value; }
            std::string valueStr() const override;
        };


        typedef std::string amqp_string_t;
        class AmqpString: public PrimitiveType {
        protected:
            amqp_string_t _value;
        public:
            AmqpString();
            virtual ~AmqpString();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::STRING_TYPE; }
            inline amqp_string_t& value() { return _value; }
            inline const amqp_string_t& value() const { return _value; }
            std::string valueStr() const override;
        };


        typedef std::string amqp_symbol_t;
        class AmqpSymbol: public PrimitiveType {
        protected:
            amqp_symbol_t _value;
        public:
            AmqpSymbol();
            virtual ~AmqpSymbol();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::SYMBOL_TYPE; }
            inline amqp_symbol_t& value() { return _value; }
            inline const amqp_symbol_t& value() const { return _value; }
            std::string valueStr() const override;
        };


        //======================
        //--- Compound types ---
        //======================

        class CompoundType: public PrimitiveType {
        public:
            CompoundType();
            virtual ~CompoundType();
            virtual std::string toString(std::size_t margin) const = 0; // formatted one element-per-line output
        protected:
            static std::size_t appendString(std::ostringstream& oss, Type* ptr, std::size_t margin);
        };


        typedef std::vector<PrimitiveType*> amqp_list_t;
        typedef amqp_list_t::iterator amqp_list_itr_t;
        typedef amqp_list_t::const_iterator amqp_list_citr_t;
        class AmqpList: public CompoundType {
        protected:
            amqp_list_t _value;
        public:
            AmqpList();
            virtual ~AmqpList();
            std::string toString(std::size_t margin) const override;
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::LIST_TYPE; }
            inline std::string typeValueStr() const { return Type::typeValueStr("[]"); }
            inline amqp_list_t& value() { return _value; }
            inline const amqp_list_t& value() const { return _value; }
            std::string valueStr() const override;
        };


        typedef std::map<PrimitiveType*, PrimitiveType*> amqp_map_t;
        typedef amqp_map_t::iterator amqp_map_itr_t;
        typedef amqp_map_t::const_iterator amqp_map_citr_t;
        class AmqpMap: public CompoundType {
        protected:
            amqp_map_t _value;
        public:
            AmqpMap();
            virtual ~AmqpMap();
            std::string toString(std::size_t margin) const override;
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::MAP_TYPE; }
            inline std::string typeValueStr() const { return Type::typeValueStr("{}"); }
            inline amqp_map_t& value() { return _value; }
            inline const amqp_map_t& value() const { return _value; }
            std::string valueStr() const override;
        };


        typedef std::vector<PrimitiveType*> amqp_array_t;
        typedef amqp_array_t::iterator amqp_array_itr_t;
        typedef amqp_array_t::const_iterator amqp_array_citr_t;
        class AmqpArray: public CompoundType {
        protected:
            amqp_array_t _value;
        public:
            AmqpArray();
            virtual ~AmqpArray();
            std::string toString(std::size_t margin) const override;
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::ARRAY_TYPE; }
            inline std::string typeValueStr() const { return Type::typeValueStr("[]"); }
            inline amqp_array_t& value() { return _value; }
            inline const amqp_array_t& value() const { return _value; }
            std::string valueStr() const override;
        };


        //=============================
        //--- AMQP restricted types ---
        //=============================


        enum class amqp_role_t: amqp_boolean_t {
            SENDER=false,
            RECEIVER=true
        };
        class AmqpRole: public AmqpBoolean {
        protected:
            static std::map<amqp_role_t, const char*> s_choiceNames;
        public:
            AmqpRole(amqp_role_t v);
            virtual ~AmqpRole();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::ROLE_TYPE; }
            inline amqp_role_t value() const { return (amqp_role_t)_value; }
            std::string valueStr() const override;
        };


        enum class amqp_sender_settle_mode_t:amqp_ubyte_t {
            UNSETTLED=0,
            SETTLED,
            MIXED
        };
        class AmqpSenderSettleMode: public AmqpUbyte {
        protected:
            static std::map<amqp_sender_settle_mode_t, const char*> s_choiceNames;
        public:
            AmqpSenderSettleMode(amqp_sender_settle_mode_t v);
            virtual ~AmqpSenderSettleMode();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::SENDER_SETTLE_MODE_TYPE; }
            inline amqp_sender_settle_mode_t value() const { return (amqp_sender_settle_mode_t)_value; }
            std::string valueStr() const override;
        };


        enum class amqp_receiver_settle_mode_t:amqp_ubyte_t {
            FIRST=0,
            SECOND
        };
        class AmqpReceiverSettleMode: public AmqpUbyte {
        protected:
            static std::map<amqp_receiver_settle_mode_t, const char*> s_choiceNames;
        public:
            AmqpReceiverSettleMode(amqp_receiver_settle_mode_t v);
            virtual ~AmqpReceiverSettleMode();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::RECEIVER_SETTLE_MODE_TYPE; }
            inline amqp_receiver_settle_mode_t value() const { return (amqp_receiver_settle_mode_t)_value; }
            std::string valueStr() const override;
        };


        typedef amqp_uint_t amqp_handle_t;
        class AmqpHandle: public AmqpUint {
        public:
            AmqpHandle(amqp_handle_t v);
            virtual ~AmqpHandle();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::HANDLE_TYPE; }
            inline amqp_handle_t value() const { return _value; }
        };


        typedef amqp_uint_t amqp_seconds_t;
        class AmqpSeconds: public AmqpUint {
        public:
            AmqpSeconds(amqp_seconds_t v);
            virtual ~AmqpSeconds();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::SECONDS_TYPE; }
            inline amqp_seconds_t value() const { return _value; }
        };


        typedef amqp_uint_t amqp_milliseconds_t;
        class AmqpMilliseconds: public AmqpUint {
        public:
            AmqpMilliseconds(amqp_milliseconds_t v);
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
            virtual ~AmqpDeliveryTag();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::DELIVERY_TAG_TYPE; }
            inline amqp_delivery_tag_t& value() { return _value; }
            inline const amqp_delivery_tag_t& value() const { return _value; }
        };


        typedef amqp_uint_t amqp_sequence_num_t;
        class AmqpSequenceNum: public AmqpUint {
        public:
            AmqpSequenceNum(amqp_sequence_num_t v);
            virtual ~AmqpSequenceNum();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::DELIVERY_NUMBER_TYPE; }
            inline amqp_sequence_num_t value() const { return _value; }
        };


        typedef amqp_sequence_num_t amqp_delivery_num_t;
        class AmqpDeliveryNum: public AmqpUint {
        public:
            AmqpDeliveryNum(amqp_delivery_num_t v);
            virtual ~AmqpDeliveryNum();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::TRANSFER_NUMBER_TYPE; }
            inline amqp_delivery_num_t value() const { return _value; }
        };


        typedef amqp_sequence_num_t amqp_transfer_num_t;
        class AmqpTransferNum: public AmqpUint {
        public:
            AmqpTransferNum(amqp_transfer_num_t v);
            virtual ~AmqpTransferNum();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::SEQUENCE_NUMBER_TYPE; }
            inline amqp_transfer_num_t value() const { return _value; }
        };


        typedef amqp_uint_t amqp_message_format_t;
        class AmqpMessageFormat: public AmqpUint {
        public:
            AmqpMessageFormat(amqp_message_format_t v);
            virtual ~AmqpMessageFormat();
            inline uint32_t getFormat() const { return (uint32_t)(_value >> 8); }
            inline uint8_t getVersion() const { return (uint8_t)(_value & uint32_t(0xff)); }
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::MESSAGE_FORMAT_TYPE; }
            inline amqp_message_format_t value() const { return _value; }
            std::string valueStr() const override;
        };


        typedef amqp_symbol_t amqp_language_tag_t;
        class AmqpLanguageTag: public AmqpSymbol {
        public:
            AmqpLanguageTag();
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
            virtual ~AmqpFields();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::FIELDS_TYPE; }
            inline amqp_fields_t& value() { return _value; }
            inline const amqp_fields_t& value() const { return _value; }
        };


        typedef amqp_symbol_t amqp_error_t;
        typedef std::vector<amqp_error_t> amqp_error_list_t;
        typedef amqp_error_list_t::const_iterator amqp_error_list_citr_t;
        class AmqpError: public AmqpSymbol {
        protected:
        	static amqp_provides_requires_list_t s_providesList;
            static amqp_error_list_t s_amqpErrors;
        public:
            AmqpError(const amqp_error_t& v);
            virtual ~AmqpError();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::AMQP_ERROR_TYPE; }
            inline amqp_error_t& value() { return _value; }
            inline const amqp_error_t& value() const { return _value; }
        };


        typedef amqp_symbol_t amqp_connection_error_t;
        typedef std::vector<amqp_error_t> amqp_connection_error_list_t;
        typedef amqp_connection_error_list_t::const_iterator amqp_connection_error_list_citr_t;
        class AmqpConnectionError: public AmqpSymbol {
        protected:
        	static amqp_provides_requires_list_t s_providesList;
            static amqp_connection_error_list_t s_amqpConnectionErrors;
        public:
            AmqpConnectionError(const amqp_connection_error_t& v);
            virtual ~AmqpConnectionError();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::CONNECTION_ERROR_TYPE; }
            inline amqp_connection_error_t& value() { return _value; }
            inline const amqp_connection_error_t& value() const { return _value; }
        };


        typedef amqp_symbol_t amqp_session_error_t;
        typedef std::vector<amqp_error_t> amqp_session_error_list_t;
        typedef amqp_session_error_list_t::const_iterator amqp_session_error_list_citr_t;
        class AmqpSessionError: public AmqpSymbol {
        protected:
        	static amqp_provides_requires_list_t s_providesList;
            static amqp_session_error_list_t s_amqpSessionErrors;
        public:
            AmqpSessionError(const amqp_session_error_t& v);
            virtual ~AmqpSessionError();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::SESSION_ERROR_TYPE; }
            inline amqp_session_error_t& value() { return _value; }
            inline const amqp_session_error_t& value() const { return _value; }
        };


        typedef amqp_symbol_t amqp_link_error_t;
        typedef std::vector<amqp_error_t> amqp_link_error_list_t;
        typedef amqp_link_error_list_t::const_iterator amqp_link_error_list_citr_t;
        class AmqpLinkError: public AmqpSymbol {
        protected:
        	static amqp_provides_requires_list_t s_providesList;
            static amqp_link_error_list_t s_amqpLinkErrors;
        public:
            AmqpLinkError(const amqp_link_error_t& v);
            virtual ~AmqpLinkError();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::LINK_ERROR_TYPE; }
            inline amqp_link_error_t& value() { return _value; }
            inline const amqp_link_error_t& value() const { return _value; }
        };


        typedef amqp_map_t amqp_annotations_t;
        typedef amqp_annotations_t::iterator amqp_annotations_itr_t;
        typedef amqp_annotations_t::const_iterator amqp_annotations_citr_t;
        class AmqpAnnotations: public AmqpMap {
        public:
            AmqpAnnotations();
            virtual ~AmqpAnnotations();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::ANNOTATIONS_TYPE; }
            inline amqp_annotations_t& value() { return _value; }
            inline const amqp_annotations_t& value() const { return _value; }
        };


        typedef amqp_annotations_t amqp_delivery_annotations_t;
        typedef amqp_delivery_annotations_t::iterator amqp_delivery_annotations_itr_t;
        typedef amqp_delivery_annotations_t::const_iterator amqp_delivery_annotations_citr_t;
        class AmqpDeliveryAnnotations: public AmqpAnnotations {
        public:
            AmqpDeliveryAnnotations();
            virtual ~AmqpDeliveryAnnotations();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::DELIVERY_ANNOTATIONS_TYPE; }
            inline amqp_delivery_annotations_t& value() { return _value; }
            inline const amqp_delivery_annotations_t& value() const { return _value; }
        };


        typedef amqp_annotations_t amqp_message_annotations_t;
        typedef amqp_message_annotations_t::iterator amqp_message_annotations_itr_t;
        typedef amqp_message_annotations_t::const_iterator amqp_message_annotations_citr_t;
        class AmqpMessageAnnotations: public AmqpAnnotations {
        public:
            AmqpMessageAnnotations();
            virtual ~AmqpMessageAnnotations();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::MESSAGE_ANNOTATIONS_TYPE; }
            inline amqp_message_annotations_t& value() { return _value; }
            inline const amqp_message_annotations_t& value() const { return _value; }
        };


        typedef amqp_map_t amqp_application_properties_t;
        typedef amqp_application_properties_t::iterator amqp_application_properties_itr_t;
        typedef amqp_application_properties_t::const_iterator amqp_application_properties_citr_t;
        class AmqpApplicationProperties: public AmqpMap {
        public:
            AmqpApplicationProperties();
            virtual ~AmqpApplicationProperties();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::APPLICATION_PROPERTIES_TYPE; }
            inline amqp_application_properties_t& value() { return _value; }
            inline const amqp_application_properties_t& value() const { return _value; }
        };


        typedef amqp_binary_t amqp_raw_data_t;
        typedef amqp_raw_data_t::iterator amqp_raw_data_itr_t;
        typedef amqp_raw_data_t::const_iterator amqp_raw_data_citr_t;
        class AmqpRawData: public AmqpBinary {
        public:
            AmqpRawData();
            virtual ~AmqpRawData();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::RAW_DATA_TYPE; }
            inline amqp_raw_data_t& value() { return _value; }
            inline const amqp_raw_data_t& value() const { return _value; }
        };


        typedef amqp_list_t amqp_sequence_t;
        typedef amqp_list_t::iterator amqp_sequence_itr_t;
        typedef amqp_list_t::const_iterator amqp_sequence_citr_t;
        class AmqpSequence: public AmqpList {
        public:
            AmqpSequence();
            virtual ~AmqpSequence();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::AMQP_SEQUENCE_TYPE; }
            inline amqp_sequence_t& value() { return _value; }
            inline const amqp_sequence_t& value() const { return _value; }
        };


        typedef amqp_primitive_t amqp_value_t;


        typedef amqp_annotations_t amqp_footer_t;
        typedef amqp_footer_t::iterator amqp_footer_itr_t;
        typedef amqp_footer_t::const_iterator amqp_footer_citr_t;
        class AmqpFooter: public AmqpAnnotations {
        public:
            AmqpFooter();
            virtual ~AmqpFooter();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::FOOTER_TYPE; }
            inline amqp_footer_t& value() { return _value; }
            inline const amqp_footer_t& value() const { return _value; }
        };


        typedef amqp_ulong_t amqp_msg_id_ulong_t;
        class AmqpMessageIdUlong: public AmqpUlong {
        protected:
        	static amqp_provides_requires_list_t s_providesList;
        public:
            AmqpMessageIdUlong(amqp_msg_id_ulong_t v);
            virtual ~AmqpMessageIdUlong();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::MESSAGE_ID_ULONG_TYPE; }
            inline amqp_msg_id_ulong_t value() const { return _value; }
        };


        typedef amqp_uuid_t amqp_msg_id_uuid_t;
        class AmqpMessageIdUuid: public AmqpUuid {
        protected:
        	static amqp_provides_requires_list_t s_providesList;
        public:
            AmqpMessageIdUuid();
            virtual ~AmqpMessageIdUuid();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::MESSAGE_ID_UUID_TYPE; }
            inline amqp_msg_id_uuid_t& value() { return _value; }
            inline const amqp_msg_id_uuid_t& value() const { return _value; }
        };


        typedef amqp_binary_t amqp_msg_id_binary_t;
        class AmqpMessageIdBinary: public AmqpBinary {
        protected:
        	static amqp_provides_requires_list_t s_providesList;
        public:
            AmqpMessageIdBinary();
            virtual ~AmqpMessageIdBinary();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::MESSAGE_ID_BINARY_TYPE; }
            inline amqp_msg_id_binary_t& value() { return _value; }
            inline const amqp_msg_id_binary_t& value() const { return _value; }
        };


        typedef amqp_string_t amqp_msg_id_string_t;
        class AmqpMessageIdString: public AmqpString {
        protected:
        	static amqp_provides_requires_list_t s_providesList;
        public:
            AmqpMessageIdString();
            virtual ~AmqpMessageIdString();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::MESSAGE_ID_STRING_TYPE; }
            inline amqp_msg_id_string_t& value() { return _value; }
            inline const amqp_msg_id_string_t& value() const { return _value; }
       };


        typedef amqp_string_t amqp_address_t;
        class AmqpAddress: public AmqpString {
        protected:
        	static amqp_provides_requires_list_t s_providesList;
        public:
            AmqpAddress();
            virtual ~AmqpAddress();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::ADDRESS_TYPE; }
            inline amqp_address_t& value() { return _value; }
            inline const amqp_address_t& value() const { return _value; }
        };

        enum class amqp_terminus_durability_t:amqp_uint_t {
            NONE=0,
            CONFIGURATION,
            UNSETTLED_STATE
        };
        class AmqpTerminusDurability: public AmqpUint {
        protected:
            static std::map<amqp_terminus_durability_t, const char*> s_choiceNames;
        public:
            AmqpTerminusDurability(amqp_terminus_durability_t v);
            virtual ~AmqpTerminusDurability();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::TERMINUS_DURABILITY_TYPE; }
            inline amqp_terminus_durability_t value() const { return (amqp_terminus_durability_t)_value; }
            std::string valueStr() const override;
        };


        typedef amqp_symbol_t amqp_terminus_expiry_policy_t;
        typedef std::vector<amqp_terminus_expiry_policy_t> amqp_terminus_expiry_policy_list_t;
        typedef amqp_terminus_expiry_policy_list_t::const_iterator amqp_terminus_expiry_policy_list_citr_t;
        class AmqpTerminusExpiryPolicy: public AmqpSymbol {
        protected:
            static amqp_terminus_expiry_policy_list_t s_terminusExpiryPolicyList;
        public:
            AmqpTerminusExpiryPolicy(const amqp_terminus_expiry_policy_t& v);
            virtual ~AmqpTerminusExpiryPolicy();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::TERMINUS_EXPIRY_POLICY_TYPE; }
            inline amqp_terminus_expiry_policy_t& value() { return _value; }
            inline const amqp_terminus_expiry_policy_t& value() const { return _value; }
        };


        typedef amqp_symbol_t amqp_std_dist_mode_t;
        typedef std::vector<amqp_std_dist_mode_t> amqp_std_dist_mode_list_t;
        typedef amqp_std_dist_mode_list_t::const_iterator amqp_std_dist_mode_list_citr_tt;
        class AmqpStandardDistributionMode: public  AmqpSymbol {
        protected:
            static amqp_std_dist_mode_list_t s_StandardDistributionModeList;
            static amqp_provides_requires_list_t s_providesList;
        public:
            AmqpStandardDistributionMode(const amqp_std_dist_mode_t& v);
            virtual ~AmqpStandardDistributionMode();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::STD_DIST_MODE_TYPE; }
            inline amqp_std_dist_mode_t& value() { return _value; }
            inline const amqp_std_dist_mode_t& value() const { return _value; }
        };


        typedef amqp_map_t amqp_filter_set_t;
        typedef amqp_filter_set_t::iterator amqp_filter_set_itr_t;
        typedef amqp_filter_set_t::const_iterator amqp_filter_set_citr_t;
        class AmqpFilterSet: public AmqpMap {
        public:
            AmqpFilterSet();
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
            virtual ~AmqpTransactionId();
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
        protected:
            static amqp_transaction_capability_list_t s_transactionCapabilityList;
        	static amqp_provides_requires_list_t s_providesList;
        public:
            AmqpTransactionalCapability(const amqp_transaction_capability_t& v);
            virtual ~AmqpTransactionalCapability();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::TRANSACTION_CAPABILITY_TYPE; }
            inline amqp_transaction_capability_t& value() { return _value; }
            inline const amqp_transaction_capability_t& value() const { return _value; }
        };


        typedef amqp_symbol_t amqp_transaction_error_t;
        typedef std::vector<amqp_transaction_error_t> amqp_transaction_error_list_t;
        typedef amqp_transaction_error_list_t::const_iterator amqp_transaction_error_list_citr_t;
        class AmqpTransactionError: public AmqpSymbol {
        protected:
            static amqp_transaction_error_list_t s_transactionErrorList;
        	static amqp_provides_requires_list_t s_providesList;
        public:
            AmqpTransactionError(const amqp_transaction_error_t& v);
            virtual ~AmqpTransactionError();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::TRANSACTION_ERROR_TYPE; }
            inline amqp_transaction_error_t& value() { return _value; }
            inline const amqp_transaction_error_t& value() const { return _value; }
        };


        enum class amqp_sasl_code_t:amqp_ubyte_t {
            OK=0,
            AUTH,
            SYS,
            SYS_PERM,
            SYS_TEMP
        };
        class AmqpSaslCode: public AmqpUbyte {
        protected:
            static std::map<amqp_sasl_code_t, const char*> s_choiceNames;
        public:
            AmqpSaslCode(amqp_sasl_code_t v);
            virtual ~AmqpSaslCode();
            inline amqpPrimitiveType_t type() const override { return amqpPrimitiveType_t::SASL_CODE_TYPE; }
            inline amqp_sasl_code_t value() const { return (amqp_sasl_code_t)_value; }
            std::string valueStr() const override;
        };




        //=========================================
        //--- AMQP composite types and sections ---
        //=========================================

        enum class amqpCompositeType_t:uint64_t {
            OPEN=0x10,
            BEGIN,
            ATTACH,
            FLOW,
            TRANSFER,
            DISPOSITION,
            DETACH,
            END,
            CLOSE,
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
            SASL_OUTCOME,
            HEADER = 0x70,
            //DELIVERY_ANNOTATIONS,
            //MESSAGE_ANNOTATIONS,
            PROPERTIES = 0x73 //,
            //APPLICATION_PROPERTIES,
            //MESSAGE_DATA_RAW,
            //MESSAGE_DATA_AMQP_SEQUENCE,
            //MESSAGE_DATA_AMQP_VALUE,
            //FOOTER
        };

        struct FieldType {
            const char* _fieldName;
            union ctype {
                const amqpPrimitiveType_t _primitiveType;
                const amqpCompositeType_t _compositeType;
                const char _wildcard;
                ctype(amqpPrimitiveType_t primitiveType): _primitiveType(primitiveType) {}
                ctype(amqpCompositeType_t compositeType): _compositeType(compositeType) {}
                ctype(char wildcard): _wildcard(wildcard) {}
            } _types;
            const bool _mandatoryFlag;
            const bool _multipleFlag;
            std::vector<amqpRequiresProvides_t> _requiresList;
            FieldType(const char* fieldName, amqpPrimitiveType_t primitiveType, bool mandatoryFlag, bool multipleFlag, std::initializer_list<amqpRequiresProvides_t> _requiresInit = {});
            FieldType(const char* fieldName, amqpCompositeType_t compositeType, bool mandatoryFlag, bool multipleFlag, std::initializer_list<amqpRequiresProvides_t> _requiresInit = {});
            FieldType(const char* fieldName, char wildcard, bool mandatoryFlag, bool multipleFlag, std::initializer_list<amqpRequiresProvides_t> _requiresInit);
        };
        typedef std::vector<FieldType> fieldTypeList_t;
        typedef fieldTypeList_t::const_iterator fieldTypeList_citr_t;


        class CompositeType: public Type {
        protected:
            AmqpList* _fieldListPtr;
        public:
            CompositeType(AmqpList* fieldListPtr);
            virtual ~CompositeType();
            inline const AmqpList* fieldList() const { return _fieldListPtr; }
            inline bool isComposite() const override { return true; };
            std::string toString(std::size_t margin) const;
            //std::string toString(std::size_t margin, const fieldTypeList_t& fieldTypeList) const;
            virtual amqpCompositeType_t type() const = 0;
            inline const char* typeStr() const override { return s_amqpCompositeTypeNames.at(type()); }
            std::string valueStr() const override;
            static std::map<amqpCompositeType_t, const char*> s_amqpCompositeTypeNames;
        protected:
            //std::string toString(const char* name) const override;
        };


        class AmqpOpen: public CompositeType {
        public:
            AmqpOpen(AmqpList* fieldList);
            virtual ~AmqpOpen();
            //inline std::string toString(std::size_t margin) const override { return CompositeType::toString(margin, s_fieldTypeList); };
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::OPEN; }
            //std::string valueStr() const override;
        protected:
            static fieldTypeList_t s_fieldTypeList;
        };


        class AmqpBegin: public CompositeType {
        public:
            AmqpBegin(AmqpList* fieldList);
            virtual ~AmqpBegin();
            //inline std::string toString(std::size_t margin) const override { return CompositeType::toString(margin, s_fieldTypeList); };
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::BEGIN; }
            //std::string valueStr() const override;
        protected:
            static fieldTypeList_t s_fieldTypeList;
        };


        class AmqpAttach: public CompositeType {
        public:
            AmqpAttach(AmqpList* fieldList);
            virtual ~AmqpAttach();
            //inline std::string toString(std::size_t margin) const override { return CompositeType::toString(margin, s_fieldTypeList); };
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::ATTACH; }
            //std::string valueStr() const override;
        protected:
            static fieldTypeList_t s_fieldTypeList;
        };


        class AmqpFlow: public CompositeType {
        public:
            AmqpFlow(AmqpList* fieldList);
            virtual ~AmqpFlow();
            //inline std::string toString(std::size_t margin) const override { return CompositeType::toString(margin, s_fieldTypeList); };
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::FLOW; }
            //std::string valueStr() const override;
        protected:
            static fieldTypeList_t s_fieldTypeList;
        };


        class AmqpTransfer: public CompositeType {
        public:
            AmqpTransfer(AmqpList* fieldList);
            virtual ~AmqpTransfer();
            //inline std::string toString(std::size_t margin) const override { return CompositeType::toString(margin, s_fieldTypeList); };
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::TRANSFER; }
            //std::string valueStr() const override;
        protected:
            static fieldTypeList_t s_fieldTypeList;
        };


        class AmqpDisposition: public CompositeType {
        public:
            AmqpDisposition(AmqpList* fieldList);
            virtual ~AmqpDisposition();
            //inline std::string toString(std::size_t margin) const override { return CompositeType::toString(margin, s_fieldTypeList); };
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::DISPOSITION; }
            //std::string valueStr() const override;
        protected:
            static fieldTypeList_t s_fieldTypeList;
        };


        class AmqpDetach: public CompositeType {
        public:
            AmqpDetach(AmqpList* fieldList);
            virtual ~AmqpDetach();
            //inline std::string toString(std::size_t margin) const override { return CompositeType::toString(margin, s_fieldTypeList); };
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::DETACH; }
            //std::string valueStr() const override;
        protected:
            static fieldTypeList_t s_fieldTypeList;
        };


        class AmqpEnd: public CompositeType {
        public:
            AmqpEnd(AmqpList* fieldList);
            virtual ~AmqpEnd();
            //inline std::string toString(std::size_t margin) const override { return CompositeType::toString(margin, s_fieldTypeList); };
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::END; }
            //std::string valueStr() const override;
        protected:
            static fieldTypeList_t s_fieldTypeList;
        };


        class AmqpClose: public CompositeType {
        public:
            AmqpClose(AmqpList* fieldList);
            virtual ~AmqpClose();
            //inline std::string toString(std::size_t margin) const override { return CompositeType::toString(margin, s_fieldTypeList); };
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::CLOSE; }
            //std::string valueStr() const override;
        protected:
            static fieldTypeList_t s_fieldTypeList;
        };


        class AmqpErrorRecord: public CompositeType {
        public:
            AmqpErrorRecord(AmqpList* fieldList);
            virtual ~AmqpErrorRecord();
            //inline std::string toString(std::size_t margin) const override { return CompositeType::toString(margin, s_fieldTypeList); };
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::ERROR; }
            //std::string valueStr() const override;
        protected:
            static fieldTypeList_t s_fieldTypeList;
        };


        class AmqpReceived: public CompositeType {
        public:
            AmqpReceived(AmqpList* fieldList);
            virtual ~AmqpReceived();
            //inline std::string toString(std::size_t margin) const override { return CompositeType::toString(margin, s_fieldTypeList); };
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::RECEIVED; }
            //std::string valueStr() const override;
        protected:
            static fieldTypeList_t s_fieldTypeList;
        	static amqp_provides_requires_list_t s_providesList;
        };


        class AmqpAccepted: public CompositeType {
        public:
            AmqpAccepted(AmqpList* fieldList);
            virtual ~AmqpAccepted();
            //inline std::string toString(std::size_t margin) const override { return CompositeType::toString(margin, s_fieldTypeList); };
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::ACCEPTED; }
            //std::string valueStr() const override;
        protected:
            static fieldTypeList_t s_fieldTypeList;
        	static amqp_provides_requires_list_t s_providesList;
        };


        class AmqpRejected: public CompositeType {
        public:
            AmqpRejected(AmqpList* fieldList);
            virtual ~AmqpRejected();
            //inline std::string toString(std::size_t margin) const override { return CompositeType::toString(margin, s_fieldTypeList); };
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::REJECTED; }
            //std::string valueStr() const override;
        protected:
            static fieldTypeList_t s_fieldTypeList;
        	static amqp_provides_requires_list_t s_providesList;
        };


        class AmqpReleased: public CompositeType {
        public:
            AmqpReleased(AmqpList* fieldList);
            virtual ~AmqpReleased();
            //inline std::string toString(std::size_t margin) const override { return CompositeType::toString(margin, s_fieldTypeList); };
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::RELEASED; }
            //std::string valueStr() const override;
        protected:
            static fieldTypeList_t s_fieldTypeList;
        	static amqp_provides_requires_list_t s_providesList;
        };


        class AmqpModified: public CompositeType {
        public:
            AmqpModified(AmqpList* fieldList);
            virtual ~AmqpModified();
            //inline std::string toString(std::size_t margin) const override { return CompositeType::toString(margin, s_fieldTypeList); };
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::MODIFIED; }
            //std::string valueStr() const override;
        protected:
            static fieldTypeList_t s_fieldTypeList;
        	static amqp_provides_requires_list_t s_providesList;
        };


        class AmqpSource: public CompositeType {
        public:
            AmqpSource(AmqpList* fieldList);
            virtual ~AmqpSource();
            //inline std::string toString(std::size_t margin) const override { return CompositeType::toString(margin, s_fieldTypeList); };
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::SOURCE; }
            //std::string valueStr() const override;
        protected:
            static fieldTypeList_t s_fieldTypeList;
        	static amqp_provides_requires_list_t s_providesList;
        };


        class AmqpTarget: public CompositeType {
        public:
            AmqpTarget(AmqpList* fieldList);
            virtual ~AmqpTarget();
            //inline std::string toString(std::size_t margin) const override { return CompositeType::toString(margin, s_fieldTypeList); };
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::TARGET; }
            //std::string valueStr() const override;
        protected:
            static fieldTypeList_t s_fieldTypeList;
        	static amqp_provides_requires_list_t s_providesList;
        };

        class AmqpDeleteOnClose: public CompositeType {
        public:
            AmqpDeleteOnClose(AmqpList* fieldList);
            virtual ~AmqpDeleteOnClose();
            //inline std::string toString(std::size_t margin) const override { return CompositeType::toString(margin, s_fieldTypeList); };
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::DELETE_ON_CLOSE; }
            //std::string valueStr() const override;
        protected:
            static fieldTypeList_t s_fieldTypeList;
        };


        class AmqpDeleteOnNoLinks: public CompositeType {
        public:
            AmqpDeleteOnNoLinks(AmqpList* fieldList);
            virtual ~AmqpDeleteOnNoLinks();
            //inline std::string toString(std::size_t margin) const override { return CompositeType::toString(margin, s_fieldTypeList); };
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::DELETE_ON_NO_LINKS; }
            //std::string valueStr() const override;
        protected:
            static fieldTypeList_t s_fieldTypeList;
        };


        class AmqpDeleteOnNoMessages: public CompositeType {
        public:
            AmqpDeleteOnNoMessages(AmqpList* fieldList);
            virtual ~AmqpDeleteOnNoMessages();
            //inline std::string toString(std::size_t margin) const override { return CompositeType::toString(margin, s_fieldTypeList); };
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::DELETE_ON_NO_MESSAGES; }
            //std::string valueStr() const override;
        protected:
            static fieldTypeList_t s_fieldTypeList;
        };


        class AmqpDeleteOnNoLinksOrMessages: public CompositeType {
        public:
            AmqpDeleteOnNoLinksOrMessages(AmqpList* fieldList);
            virtual ~AmqpDeleteOnNoLinksOrMessages();
            //inline std::string toString(std::size_t margin) const override { return CompositeType::toString(margin, s_fieldTypeList); };
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::DELETE_ON_NO_LINKS_OR_MESSAGES; }
            //std::string valueStr() const override;
        protected:
            static fieldTypeList_t s_fieldTypeList;
        };


        class AmqpCoordinator: public CompositeType {
        public:
            AmqpCoordinator(AmqpList* fieldList);
            virtual ~AmqpCoordinator();
            //inline std::string toString(std::size_t margin) const override { return CompositeType::toString(margin, s_fieldTypeList); };
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::COORDINATOR; }
            //std::string valueStr() const override;
        protected:
            static fieldTypeList_t s_fieldTypeList;
        	static amqp_provides_requires_list_t s_providesList;
        };


        class AmqpDeclare: public CompositeType {
        public:
            AmqpDeclare(AmqpList* fieldList);
            virtual ~AmqpDeclare();
            //inline std::string toString(std::size_t margin) const override { return CompositeType::toString(margin, s_fieldTypeList); };
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::DECLARE; }
            //std::string valueStr() const override;
        protected:
            static fieldTypeList_t s_fieldTypeList;
        };


        class AmqpDischarge: public CompositeType {
        public:
            AmqpDischarge(AmqpList* fieldList);
            virtual ~AmqpDischarge();
            //inline std::string toString(std::size_t margin) const override { return CompositeType::toString(margin, s_fieldTypeList); };
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::DISCHARGE; }
            //std::string valueStr() const override;
        protected:
            static fieldTypeList_t s_fieldTypeList;
        };


        class AmqpDeclared: public CompositeType {
        public:
            AmqpDeclared(AmqpList* fieldList);
            virtual ~AmqpDeclared();
            //inline std::string toString(std::size_t margin) const override { return CompositeType::toString(margin, s_fieldTypeList); };
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::DECLARED; }
            //std::string valueStr() const override;
        protected:
            static fieldTypeList_t s_fieldTypeList;
        	static amqp_provides_requires_list_t s_providesList;
        };


        class AmqpTransactionalState: public CompositeType {
        public:
            AmqpTransactionalState(AmqpList* fieldList);
            virtual ~AmqpTransactionalState();
            //inline std::string toString(std::size_t margin) const override { return CompositeType::toString(margin, s_fieldTypeList); };
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::TRANSACTIONAL_STATE; }
            //std::string valueStr() const override;
        protected:
            static fieldTypeList_t s_fieldTypeList;
        	static amqp_provides_requires_list_t s_providesList;
        };


        class AmqpSaslMechanisms: public CompositeType {
        public:
            AmqpSaslMechanisms(AmqpList* fieldList);
            virtual ~AmqpSaslMechanisms();
            //inline std::string toString(std::size_t margin) const override { return CompositeType::toString(margin, s_fieldTypeList); };
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::SASL_MECHANISMS; }
            //std::string valueStr() const override;
        protected:
            static fieldTypeList_t s_fieldTypeList;
        };


        class AmqpSaslInit: public CompositeType {
        public:
            AmqpSaslInit(AmqpList* fieldList);
            virtual ~AmqpSaslInit();
            //inline std::string toString(std::size_t margin) const override { return CompositeType::toString(margin, s_fieldTypeList); };
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::SASL_INIT; }
            //std::string valueStr() const override;
        protected:
            static fieldTypeList_t s_fieldTypeList;
        };


        class AmqpSaslChallenge: public CompositeType {
        public:
            AmqpSaslChallenge(AmqpList* fieldList);
            virtual ~AmqpSaslChallenge();
            //inline std::string toString(std::size_t margin) const override { return CompositeType::toString(margin, s_fieldTypeList); };
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::SASL_CHALLENGE; }
            //std::string valueStr() const override;
        protected:
            static fieldTypeList_t s_fieldTypeList;
        };


        class AmqpSaslResponse: public CompositeType {
        public:
            AmqpSaslResponse(AmqpList* fieldList);
            virtual ~AmqpSaslResponse();
            //inline std::string toString(std::size_t margin) const override { return CompositeType::toString(margin, s_fieldTypeList); };
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::SASL_RESPONSE; }
            //std::string valueStr() const override;
        protected:
            static fieldTypeList_t s_fieldTypeList;
        };


        class AmqpSaslOutcome: public CompositeType {
        public:
            AmqpSaslOutcome(AmqpList* fieldList);
            virtual ~AmqpSaslOutcome();
            //inline std::string toString(std::size_t margin) const override { return CompositeType::toString(margin, s_fieldTypeList); };
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::SASL_OUTCOME; }
            //std::string valueStr() const override;
        protected:
            static fieldTypeList_t s_fieldTypeList;
        };


        class AmqpMessageHeader: public CompositeType {
        public:
            AmqpMessageHeader(AmqpList* fieldList);
            virtual ~AmqpMessageHeader();
            //inline std::string toString(std::size_t margin) const override { return CompositeType::toString(margin, s_fieldTypeList); };
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::HEADER; }
            //std::string valueStr() const override;
        protected:
            static fieldTypeList_t s_fieldTypeList;
        };


        class AmqpMessageProperties: public CompositeType {
        public:
            AmqpMessageProperties(AmqpList* fieldList);
            virtual ~AmqpMessageProperties();
            //inline std::string toString(std::size_t margin) const override { return CompositeType::toString(margin, s_fieldTypeList); };
            inline amqpCompositeType_t type() const override { return amqpCompositeType_t::PROPERTIES; }
            //std::string valueStr() const override;
        protected:
            static fieldTypeList_t s_fieldTypeList;
        };


    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQP10_TYPE_HPP_ */
