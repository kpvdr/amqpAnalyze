/*
 * Type.hpp
 *
 *  Created on: Apr 5, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_AMQP10_TYPE_HPP_
#define SRC_AMQPANALYZE_AMQP10_TYPE_HPP_

#include <amqpAnalyze/amqp10/FwdDecls.hpp>

namespace amqpAnalyze
{
    namespace amqp10
    {

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
            virtual const AmqpProvidesRequiresList_t& providesList() const = 0;
            virtual const char* typeStr() const = 0; // type as string
            virtual std::string typeValueStr(bool colorFlag, const char* valueDelim = "()") const; // formatted string type(value) without margins
            virtual inline void validate(addErrorFp errorHandler, AmqpBlock* errorHandlerInstance) {}
            virtual std::string valueStr(bool colorFlag) const = 0; // value only as string

            static bool provides(AmqpRequiresProvides_t requires, const AmqpProvidesRequiresList_t& prList);
            static std::string providesListAsString(const AmqpProvidesRequiresList_t& prList);

        protected:
            const char* _name;
        };


        //=========================
        //--- AMQP simple types ---
        //=========================

        enum class AmqpPrimitiveType_t: uint8_t {
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

            const inline AmqpProvidesRequiresList_t& providesList() const override { return s_providesList; }
            virtual AmqpPrimitiveType_t type() const = 0;
            inline const char* typeStr() const override { return s_amqpSimpleTypeNames.at(type()); }
            static std::map<AmqpPrimitiveType_t, const char*> s_amqpSimpleTypeNames;

        protected:
            static AmqpProvidesRequiresList_t s_providesList;
        };
        

        class AmqpNull: public PrimitiveType {
        public:
            AmqpNull();
            AmqpNull(const char* name);
            virtual ~AmqpNull();

            inline bool isNull() const { return true; }
            std::string nameValueStr(bool colorFlag, const char* valueDelim = "()") const override;
            std::string nameTypeValueStr(bool colorFlag, const char* valueDelim = "()") const override;
            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::NULL_TYPE; }
            std::string typeValueStr(bool colorFlag, const char* valueDelim = "()") const override;
            std::string valueStr(bool colorFlag) const override;
        };


        class AmqpBoolean: public PrimitiveType {
        public:
            AmqpBoolean(bool v);
            AmqpBoolean(bool v, const char* name);
            virtual ~AmqpBoolean();

            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::BOOLEAN_TYPE; }
            inline bool value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            bool _value;
        };


        class AmqpUbyte: public PrimitiveType {
        public:
            AmqpUbyte(uint8_t v);
            AmqpUbyte(uint8_t v, const char* name);
            virtual ~AmqpUbyte();

            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::UBYTE_TYPE; }
            inline uint8_t value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            uint8_t _value;
        };


        class AmqpUshort: public PrimitiveType {
        public:
            AmqpUshort(uint16_t v);
            AmqpUshort(uint16_t v, const char* name);
            virtual ~AmqpUshort();

            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::USHORT_TYPE; }
            inline uint16_t value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            uint16_t _value;
        };


        class AmqpUint: public PrimitiveType {
        public:
            AmqpUint(uint32_t v);
            AmqpUint(uint32_t v, const char* name);
            virtual ~AmqpUint();

            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::UINT_TYPE; }
            inline uint32_t value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            uint32_t _value;
        };


        class AmqpUlong: public PrimitiveType {
        public:
            AmqpUlong(uint64_t v);
            AmqpUlong(uint64_t v, const char* name);
            virtual ~AmqpUlong();

            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::ULONG_TYPE; }
            inline uint64_t value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            uint64_t _value;
        };


        class AmqpByte: public PrimitiveType {
        public:
            AmqpByte(int8_t v);
            AmqpByte(int8_t v, const char* name);
            virtual ~AmqpByte();

            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::BYTE_TYPE; }
            inline int8_t value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            int8_t _value;
        };


        class AmqpShort: public PrimitiveType {
        public:
            AmqpShort(int16_t v);
            AmqpShort(int16_t v, const char* name);
            virtual ~AmqpShort();

            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::SHORT_TYPE; }
            inline int16_t value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            int16_t _value;
        };


        class AmqpInt: public PrimitiveType {
        public:
            AmqpInt(int32_t v);
            AmqpInt(int32_t v, const char* name);
            virtual ~AmqpInt();

            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::INT_TYPE; }
            inline int32_t value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            int32_t _value;
        };


        class AmqpLong: public PrimitiveType {
        public:
            AmqpLong(int64_t v);
            AmqpLong(int64_t v, const char* name);
            virtual ~AmqpLong();

            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::LONG_TYPE; }
            inline int64_t value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            int64_t _value;
        };


        class AmqpFloat: public PrimitiveType {
        public:
            AmqpFloat(float v);
            AmqpFloat(float v, const char* name);
            virtual ~AmqpFloat();

            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::FLOAT_TYPE; }
            inline float value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            float _value;
        };


        class AmqpDouble: public PrimitiveType {
        public:
            AmqpDouble(double v);
            AmqpDouble(double v, const char* name);
            virtual ~AmqpDouble();

            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::DOUBLE_TYPE; }
            inline double value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            double _value;
        };


        class AmqpDecimal32: public PrimitiveType {
        public:
            AmqpDecimal32();
            AmqpDecimal32(const char* name);
            virtual ~AmqpDecimal32();

            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::DECIMAL32_TYPE; }
            inline std::string typeValueStr(bool colorFlag, const char* valueDelim = "()") const { return Type::typeValueStr("[]"); }
            inline AmqpDecimal32_t& value() { return _value; }
            inline const AmqpDecimal32_t& value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            AmqpDecimal32_t _value;
        };


        class AmqpDecimal64: public PrimitiveType {
        public:
            AmqpDecimal64();
            AmqpDecimal64(const char* name);
            virtual ~AmqpDecimal64();

            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::DECIMAL64_TYPE; }
            inline std::string typeValueStr(bool colorFlag, const char* valueDelim = "()") const { return Type::typeValueStr("[]"); }
            inline AmqpDecimal64_t& value() { return _value; }
            inline const AmqpDecimal64_t& value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            AmqpDecimal64_t _value;
        };


        class AmqpDecimal128: public PrimitiveType {
        public:
            AmqpDecimal128();
            AmqpDecimal128(const char* name);
            virtual ~AmqpDecimal128();

            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::DECIMAL128_TYPE; }
            inline std::string typeValueStr(bool colorFlag, const char* valueDelim = "()") const { return Type::typeValueStr("[]"); }
            inline AmqpDecimal128_t& value() { return _value; }
            inline const AmqpDecimal128_t& value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            AmqpDecimal128_t _value;
        };


        class AmqpChar: public PrimitiveType {
        public:
            AmqpChar(char32_t v);
            AmqpChar(char32_t v, const char* name);
            virtual ~AmqpChar();

            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::CHAR_TYPE; }
            inline char32_t value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            char32_t _value;
        };


        class AmqpTimestamp: public PrimitiveType {
        public:
            AmqpTimestamp(uint64_t v);
            AmqpTimestamp(uint64_t v, const char* name);
            virtual ~AmqpTimestamp();

            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::TIMESTAMP_TYPE; }
            inline uint64_t value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            uint64_t _value;
        };


        class AmqpUuid: public PrimitiveType {
        public:
            AmqpUuid();
            AmqpUuid(const char* name);
            virtual ~AmqpUuid();

            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::UUID_TYPE; }
            inline AmqpUuid_t& value() { return _value; }
            inline const AmqpUuid_t& value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            AmqpUuid_t _value;
        };


        class AmqpBinary: public PrimitiveType {
        public:
            AmqpBinary();
            AmqpBinary(const char* name);
            virtual ~AmqpBinary();

            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::BINARY_TYPE; }
            inline AmqpBinary_t& value() { return _value; }
            inline const AmqpBinary_t& value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            AmqpBinary_t _value;
        };


        class AmqpString: public PrimitiveType {
        public:
            AmqpString();
            AmqpString(const char* name);
            virtual ~AmqpString();

            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::STRING_TYPE; }
            inline std::string& value() { return _value; }
            inline const std::string& value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            std::string _value;
        };


        class AmqpSymbol: public PrimitiveType {
        public:
            AmqpSymbol();
            AmqpSymbol(const char* name);
            virtual ~AmqpSymbol();

            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::SYMBOL_TYPE; }
            inline std::string& value() { return _value; }
            inline const std::string& value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            std::string _value;
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


        class AmqpList: public CompoundType {
        public:
            AmqpList();
            AmqpList(const char* name);
            virtual ~AmqpList();

            void appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin, bool colorFlag) const override;
            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::LIST_TYPE; }
            inline std::string typeValueStr(bool colorFlag) const { return Type::typeValueStr(colorFlag, "[]"); }
            void validate(const FieldTypeList_t& fieldList, addErrorFp errorHandler, AmqpBlock* errorHandlerInstance);
            inline AmqpList_t& value() { return _value; }
            inline const AmqpList_t& value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            AmqpList_t _value;
        };


        class AmqpMap: public CompoundType {
        public:
            AmqpMap();
            AmqpMap(const char* name);
            virtual ~AmqpMap();

            void appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin, bool colorFlag) const override;
            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::MAP_TYPE; }
            inline std::string typeValueStr(bool colorFlag) const { return Type::typeValueStr(colorFlag, "{}"); }
            inline AmqpMap_t& value() { return _value; }
            inline const AmqpMap_t& value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            AmqpMap_t _value;
        };


        class AmqpArray: public CompoundType {
        public:
            AmqpArray();
            AmqpArray(const char* name);
            virtual ~AmqpArray();

            void appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin, bool colorFlag) const override;
            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::ARRAY_TYPE; }
            inline std::string typeValueStr(bool colorFlag) const { return Type::typeValueStr(colorFlag, "[]"); }
            inline AmqpArray_t& value() { return _value; }
            inline const AmqpArray_t& value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            AmqpArray_t _value;
        };


        //=============================
        //--- AMQP restricted types ---
        //=============================


        enum class AmqpRole_t: bool {
            SENDER=false,
            RECEIVER=true
        };

        class AmqpRole: public AmqpBoolean {
        public:
            AmqpRole(AmqpRole_t v);
            AmqpRole(AmqpRole_t v, const char* name);
            virtual ~AmqpRole();

            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::ROLE_TYPE; }
            inline AmqpRole_t value() const { return (AmqpRole_t)_value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            static std::map<AmqpRole_t, const char*> s_choiceNames;
        };


        enum class AmqpSenderSettleMode_t:uint8_t {
            UNSETTLED=0,
            SETTLED,
            MIXED
        };

        class AmqpSenderSettleMode: public AmqpUbyte {
        public:
            AmqpSenderSettleMode(AmqpSenderSettleMode_t v);
            AmqpSenderSettleMode(AmqpSenderSettleMode_t v, const char* name);
            virtual ~AmqpSenderSettleMode();

            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::SENDER_SETTLE_MODE_TYPE; }
            inline AmqpSenderSettleMode_t value() const { return (AmqpSenderSettleMode_t)_value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            static std::map<AmqpSenderSettleMode_t, const char*> s_choiceNames;
        };


        enum class AmqpReceiverSettleMode_t:uint8_t {
            FIRST=0,
            SECOND
        };

        class AmqpReceiverSettleMode: public AmqpUbyte {
        public:
            AmqpReceiverSettleMode(AmqpReceiverSettleMode_t v);
            AmqpReceiverSettleMode(AmqpReceiverSettleMode_t v, const char* name);
            virtual ~AmqpReceiverSettleMode();

            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::RECEIVER_SETTLE_MODE_TYPE; }
            inline AmqpReceiverSettleMode_t value() const { return (AmqpReceiverSettleMode_t)_value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            static std::map<AmqpReceiverSettleMode_t, const char*> s_choiceNames;
        };


        class AmqpHandle: public AmqpUint {
        public:
            AmqpHandle(uint32_t v);
            AmqpHandle(uint32_t v, const char* name);
            virtual ~AmqpHandle();

            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::HANDLE_TYPE; }
            inline uint32_t value() const { return _value; }
        };


        class AmqpSeconds: public AmqpUint {
        public:
            AmqpSeconds(uint32_t v);
            AmqpSeconds(uint32_t v, const char* name);
            virtual ~AmqpSeconds();

            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::SECONDS_TYPE; }
            inline uint32_t value() const { return _value; }
        };


        class AmqpMilliseconds: public AmqpUint {
        public:
            AmqpMilliseconds(uint32_t v);
            AmqpMilliseconds(uint32_t v, const char* name);
            virtual ~AmqpMilliseconds();

            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::MILLISECONDS_TYPE; }
            inline uint32_t value() const { return _value; }
        };


        class AmqpDeliveryTag: public AmqpBinary {
        public:
            AmqpDeliveryTag();
            AmqpDeliveryTag(const char* name);
            virtual ~AmqpDeliveryTag();

            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::DELIVERY_TAG_TYPE; }
            inline AmqpBinary_t& value() { return _value; }
            inline const AmqpBinary_t& value() const { return _value; }
        };


        class AmqpSequenceNum: public AmqpUint {
        public:
            AmqpSequenceNum(uint32_t v);
            AmqpSequenceNum(uint32_t v, const char* name);
            virtual ~AmqpSequenceNum();

            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::DELIVERY_NUMBER_TYPE; }
            inline uint32_t value() const { return _value; }
        };


        class AmqpDeliveryNum: public AmqpSequenceNum {
        public:
            AmqpDeliveryNum(uint32_t v);
            AmqpDeliveryNum(uint32_t v, const char* name);
            virtual ~AmqpDeliveryNum();

            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::TRANSFER_NUMBER_TYPE; }
            inline uint32_t value() const { return _value; }
        };


        class AmqpTransferNum: public AmqpSequenceNum {
        public:
            AmqpTransferNum(uint32_t v);
            AmqpTransferNum(uint32_t v, const char* name);
            virtual ~AmqpTransferNum();

            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::SEQUENCE_NUMBER_TYPE; }
            inline uint32_t value() const { return _value; }
        };


        class AmqpMessageFormat: public AmqpUint {
        public:
            AmqpMessageFormat(uint32_t v);
            AmqpMessageFormat(uint32_t v, const char* name);
            virtual ~AmqpMessageFormat();

            inline uint32_t getFormat() const { return (uint32_t)(_value >> 8); }
            inline uint8_t getVersion() const { return (uint8_t)(_value & uint32_t(0xff)); }
            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::MESSAGE_FORMAT_TYPE; }
            inline uint32_t value() const { return _value; }
            std::string valueStr(bool colorFlag) const override;
        };


        class AmqpLanguageTag: public AmqpSymbol {
        public:
            AmqpLanguageTag();
            AmqpLanguageTag(const char* name);
            virtual ~AmqpLanguageTag();

            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::LANGUAGE_TAG_TYPE; }
            inline std::string& value() { return _value; }
            inline const std::string& value() const { return _value; }
        };


        class AmqpFields: public AmqpMap {
        public:
            AmqpFields();
            AmqpFields(const char* name);
            virtual ~AmqpFields();

            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::FIELDS_TYPE; }
            inline AmqpMap_t& value() { return _value; }
            inline const AmqpMap_t& value() const { return _value; }
        };


        class AmqpError: public AmqpSymbol {
        public:
            AmqpError(const std::string& v);
            AmqpError(const std::string& v, const char* name);
            virtual ~AmqpError();

            inline const AmqpProvidesRequiresList_t& providesList() const override { return s_providesList; }
            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::AMQP_ERROR_TYPE; }
            inline std::string& value() { return _value; }
            inline const std::string& value() const { return _value; }

        protected:
            void checkAssignValue(const std::string& v);
            static AmqpProvidesRequiresList_t s_providesList;
            static AmqpErrorList_t s_amqpErrors;
        };


        class AmqpConnectionError: public AmqpSymbol {
        public:
            AmqpConnectionError(const std::string& v);
            AmqpConnectionError(const std::string& v, const char* name);
            virtual ~AmqpConnectionError();

            inline const AmqpProvidesRequiresList_t& providesList() const override { return s_providesList; }
            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::CONNECTION_ERROR_TYPE; }
            inline std::string& value() { return _value; }
            inline const std::string& value() const { return _value; }

        protected:
            void checkAssignValue(const std::string& v);
            static AmqpProvidesRequiresList_t s_providesList;
            static AmqpConnectionErrorList_t s_amqpConnectionErrors;
        };


        class AmqpSessionError: public AmqpSymbol {
        public:
            AmqpSessionError(const std::string& v);
            AmqpSessionError(const std::string& v, const char* name);
            virtual ~AmqpSessionError();

            inline const AmqpProvidesRequiresList_t& providesList() const override { return s_providesList; }
            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::SESSION_ERROR_TYPE; }
            inline std::string& value() { return _value; }
            inline const std::string& value() const { return _value; }

        protected:
            void checkAssignValue(const std::string& v);
            static AmqpProvidesRequiresList_t s_providesList;
            static AmqpSessionErrorList_t s_amqpSessionErrors;
        };


        class AmqpLinkError: public AmqpSymbol {
        public:
            AmqpLinkError(const std::string& v);
            AmqpLinkError(const std::string& v, const char* name);
            virtual ~AmqpLinkError();

            inline const AmqpProvidesRequiresList_t& providesList() const override { return s_providesList; }
            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::LINK_ERROR_TYPE; }
            inline std::string& value() { return _value; }
            inline const std::string& value() const { return _value; }

        protected:
            void checkAssignValue(const std::string& v);
            static AmqpProvidesRequiresList_t s_providesList;
            static AmqpLinkErrorList_t s_amqpLinkErrors;
        };


        class AmqpAnnotations: public AmqpMap {
        public:
            AmqpAnnotations();
            AmqpAnnotations(const char* name);
            virtual ~AmqpAnnotations();

            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::ANNOTATIONS_TYPE; }
            inline AmqpMap_t& value() { return _value; }
            inline const AmqpMap_t& value() const { return _value; }
        };




        class AmqpMessageIdUlong: public AmqpUlong {
        public:
            AmqpMessageIdUlong(uint64_t v);
            AmqpMessageIdUlong(uint64_t v, const char* name);
            virtual ~AmqpMessageIdUlong();

            inline const AmqpProvidesRequiresList_t& providesList() const override { return s_providesList; }
            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::MESSAGE_ID_ULONG_TYPE; }
            inline uint64_t value() const { return _value; }

        protected:
            static AmqpProvidesRequiresList_t s_providesList;
        };


        class AmqpMessageIdUuid: public AmqpUuid {
        public:
            AmqpMessageIdUuid();
            AmqpMessageIdUuid(const char* name);
            virtual ~AmqpMessageIdUuid();

            inline const AmqpProvidesRequiresList_t& providesList() const override { return s_providesList; }
            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::MESSAGE_ID_UUID_TYPE; }
            inline AmqpUuid_t& value() { return _value; }
            inline const AmqpUuid_t& value() const { return _value; }

        protected:
            static AmqpProvidesRequiresList_t s_providesList;
        };


        class AmqpMessageIdBinary: public AmqpBinary {
        public:
            AmqpMessageIdBinary();
            AmqpMessageIdBinary(const char* name);
            virtual ~AmqpMessageIdBinary();

            inline const AmqpProvidesRequiresList_t& providesList() const override { return s_providesList; }
            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::MESSAGE_ID_BINARY_TYPE; }
            inline AmqpBinary_t& value() { return _value; }
            inline const AmqpBinary_t& value() const { return _value; }

        protected:
            static AmqpProvidesRequiresList_t s_providesList;
        };


        class AmqpMessageIdString: public AmqpString {
        public:
            AmqpMessageIdString();
            AmqpMessageIdString(const char* name);
            virtual ~AmqpMessageIdString();

            inline const AmqpProvidesRequiresList_t& providesList() const override { return s_providesList; }
            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::MESSAGE_ID_STRING_TYPE; }
            inline std::string& value() { return _value; }
            inline const std::string& value() const { return _value; }

        protected:
            static AmqpProvidesRequiresList_t s_providesList;
       };


        class AmqpAddress: public AmqpString {
        public:
            AmqpAddress();
            AmqpAddress(const char* name);
            virtual ~AmqpAddress();

            const inline AmqpProvidesRequiresList_t& providesList() const override { return s_providesList; }
            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::ADDRESS_TYPE; }
            inline std::string& value() { return _value; }
            inline const std::string& value() const { return _value; }

        protected:
            static AmqpProvidesRequiresList_t s_providesList;
        };

        enum class AmqpTerminusDurability_t:uint32_t {
            NONE=0,
            CONFIGURATION,
            UNSETTLED_STATE
        };
        class AmqpTerminusDurability: public AmqpUint {
        public:
            AmqpTerminusDurability(AmqpTerminusDurability_t v);
            AmqpTerminusDurability(AmqpTerminusDurability_t v, const char* name);
            virtual ~AmqpTerminusDurability();

            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::TERMINUS_DURABILITY_TYPE; }
            inline AmqpTerminusDurability_t value() const { return (AmqpTerminusDurability_t)_value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            static std::map<AmqpTerminusDurability_t, const char*> s_choiceNames;
        };


        class AmqpTerminusExpiryPolicy: public AmqpSymbol {
        public:
            AmqpTerminusExpiryPolicy(const std::string& v);
            AmqpTerminusExpiryPolicy(const std::string& v, const char* name);
            virtual ~AmqpTerminusExpiryPolicy();

            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::TERMINUS_EXPIRY_POLICY_TYPE; }
            inline std::string& value() { return _value; }
            inline const std::string& value() const { return _value; }

        protected:
            void checkAssignValue(const std::string& v);
            static AmqpTerminusExpiryPolicyList_t s_terminusExpiryPolicyList;
        };


        class AmqpStandardDistributionMode: public  AmqpSymbol {
        public:
            AmqpStandardDistributionMode(const std::string& v);
            AmqpStandardDistributionMode(const std::string& v, const char* name);
            virtual ~AmqpStandardDistributionMode();

            inline const AmqpProvidesRequiresList_t& providesList() const override { return s_providesList; }
            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::STD_DIST_MODE_TYPE; }
            inline std::string& value() { return _value; }
            inline const std::string& value() const { return _value; }

        protected:
            void checkAssignValue(const std::string& v);
            static AmqpStdDistModeList_t s_StandardDistributionModeList;
            static AmqpProvidesRequiresList_t s_providesList;
        };


        class AmqpFilterSet: public AmqpMap {
        public:
            AmqpFilterSet();
            AmqpFilterSet(const char* name);
            virtual ~AmqpFilterSet();

            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::FILTER_SET_TYPE; }
            inline AmqpMap_t& value() { return _value; }
            inline const AmqpMap_t& value() const { return _value; }
        };


        class AmqpNodeProperties: public AmqpFields {
        public:
            AmqpNodeProperties();
            AmqpNodeProperties(const char* name);
            virtual ~AmqpNodeProperties();

            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::NODE_PROPERTIES_TYPE; }
            inline AmqpMap_t& value() { return _value; }
            inline const AmqpMap_t& value() const { return _value; }
        };


        class AmqpTransactionId: public AmqpBinary {
        public:
            AmqpTransactionId();
            AmqpTransactionId(const char* name);
            virtual ~AmqpTransactionId();

            inline const AmqpProvidesRequiresList_t& providesList() const override { return s_providesList; }
            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::TRANSACTION_ID_TYPE; }
            inline AmqpBinary_t& value() { return _value; }
            inline const AmqpBinary_t& value() const { return _value; }

        protected:
        	static AmqpProvidesRequiresList_t s_providesList;
        };


        class AmqpTransactionalCapability: public AmqpSymbol {
        public:
            AmqpTransactionalCapability(const std::string& v);
            AmqpTransactionalCapability(const std::string& v, const char* name);
            virtual ~AmqpTransactionalCapability();

            inline const AmqpProvidesRequiresList_t& providesList() const override { return s_providesList; }
            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::TRANSACTION_CAPABILITY_TYPE; }
            inline std::string& value() { return _value; }
            inline const std::string& value() const { return _value; }

        protected:
            void checkAssignValue(const std::string& v);
            static AmqpTransactionCapabilityList_t s_transactionCapabilityList;
            static AmqpProvidesRequiresList_t s_providesList;
        };


        class AmqpTransactionError: public AmqpSymbol {
        public:
            AmqpTransactionError(const std::string& v);
            AmqpTransactionError(const std::string& v, const char* name);
            virtual ~AmqpTransactionError();

            inline const AmqpProvidesRequiresList_t& providesList() const override { return s_providesList; }
            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::TRANSACTION_ERROR_TYPE; }
            inline std::string& value() { return _value; }
            inline const std::string& value() const { return _value; }

        protected:
            static AmqpTransactionErrorList_t s_transactionErrorList;
            static AmqpProvidesRequiresList_t s_providesList;
        };


        enum class AmqpSaslCode_t:uint8_t {
            OK=0,
            AUTH,
            SYS,
            SYS_PERM,
            SYS_TEMP
        };
        class AmqpSaslCode: public AmqpUbyte {
        public:
            AmqpSaslCode(AmqpSaslCode_t v);
            AmqpSaslCode(AmqpSaslCode_t v, const char* name);
            virtual ~AmqpSaslCode();

            inline AmqpPrimitiveType_t type() const override { return AmqpPrimitiveType_t::SASL_CODE_TYPE; }
            inline AmqpSaslCode_t value() const { return (AmqpSaslCode_t)_value; }
            std::string valueStr(bool colorFlag) const override;

        protected:
            static std::map<AmqpSaslCode_t, const char*> s_choiceNames;
        };




        //=========================================
        //--- AMQP composite types and sections ---
        //=========================================


        enum class AmqpCompositeType_t:uint64_t {
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
            virtual const FieldTypeList_t& fieldTypeList() const = 0;
            inline const AmqpProvidesRequiresList_t& providesList() const { return s_providesList; }
            virtual AmqpCompositeType_t type() const = 0;
            inline const char* typeStr() const override { return s_amqpCompositeTypeNames.at(type()); }
            virtual void validate(addErrorFp errorHandler, AmqpBlock* errorHandlerInstance) override;
            std::string valueStr(bool colorFlag) const override;
            static std::map<AmqpCompositeType_t, const char*> s_amqpCompositeTypeNames;

        protected:
            AmqpList* _fieldListPtr;
            static AmqpProvidesRequiresList_t s_providesList;
       };


        class AmqpErrorRecord: public CompositeType {
        public:
            AmqpErrorRecord(AmqpList* fieldList);
            AmqpErrorRecord(AmqpList* fieldList, const char* name);
            virtual ~AmqpErrorRecord();

            inline const FieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; }
            inline AmqpCompositeType_t type() const override { return AmqpCompositeType_t::ERROR; }
            static const FieldTypeList_t s_fieldTypeList;
        };


        class AmqpReceived: public CompositeType {
        public:
            AmqpReceived(AmqpList* fieldList);
            AmqpReceived(AmqpList* fieldList, const char* name);
            virtual ~AmqpReceived();

            inline const FieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; }
            inline AmqpCompositeType_t type() const override { return AmqpCompositeType_t::RECEIVED; }
            const inline AmqpProvidesRequiresList_t& providesList() const override { return s_providesList; }
            static const FieldTypeList_t s_fieldTypeList;

        protected:
        	static AmqpProvidesRequiresList_t s_providesList;
        };


        class AmqpAccepted: public CompositeType {
        public:
            AmqpAccepted(AmqpList* fieldList);
            AmqpAccepted(AmqpList* fieldList, const char* name);
            virtual ~AmqpAccepted();

            inline const FieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; }
            inline AmqpCompositeType_t type() const override { return AmqpCompositeType_t::ACCEPTED; }
            const inline AmqpProvidesRequiresList_t& providesList() const override { return s_providesList; }
            static const FieldTypeList_t s_fieldTypeList;

        protected:
        	static AmqpProvidesRequiresList_t s_providesList;
        };


        class AmqpRejected: public CompositeType {
        public:
            AmqpRejected(AmqpList* fieldList);
            AmqpRejected(AmqpList* fieldList, const char* name);
            virtual ~AmqpRejected();

            inline const FieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; }
            inline AmqpCompositeType_t type() const override { return AmqpCompositeType_t::REJECTED; }
            const inline AmqpProvidesRequiresList_t& providesList() const override { return s_providesList; }
            static const FieldTypeList_t s_fieldTypeList;

        protected:
        	static AmqpProvidesRequiresList_t s_providesList;
        };


        class AmqpReleased: public CompositeType {
        public:
            AmqpReleased(AmqpList* fieldList);
            AmqpReleased(AmqpList* fieldList, const char* name);
            virtual ~AmqpReleased();

            inline const FieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; }
            inline AmqpCompositeType_t type() const override { return AmqpCompositeType_t::RELEASED; }
            const inline AmqpProvidesRequiresList_t& providesList() const override { return s_providesList; }
            static const FieldTypeList_t s_fieldTypeList;

        protected:
        	static AmqpProvidesRequiresList_t s_providesList;
        };


        class AmqpModified: public CompositeType {
        public:
            AmqpModified(AmqpList* fieldList);
            AmqpModified(AmqpList* fieldList, const char* name);
            virtual ~AmqpModified();

            inline const FieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; }
            inline AmqpCompositeType_t type() const override { return AmqpCompositeType_t::MODIFIED; }
            const inline AmqpProvidesRequiresList_t& providesList() const override { return s_providesList; }
            static const FieldTypeList_t s_fieldTypeList;

        protected:
        	static AmqpProvidesRequiresList_t s_providesList;
        };


        class AmqpSource: public CompositeType {
        public:
            AmqpSource(AmqpList* fieldList);
            AmqpSource(AmqpList* fieldList, const char* name);
            virtual ~AmqpSource();

            inline const FieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; }
            inline AmqpCompositeType_t type() const override { return AmqpCompositeType_t::SOURCE; }
            const inline AmqpProvidesRequiresList_t& providesList() const override { return s_providesList; }
            static const FieldTypeList_t s_fieldTypeList;

        protected:
        	static AmqpProvidesRequiresList_t s_providesList;
        };


        class AmqpTarget: public CompositeType {
        public:
            AmqpTarget(AmqpList* fieldList);
            AmqpTarget(AmqpList* fieldList, const char* name);
            virtual ~AmqpTarget();

            inline const FieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; }
            inline AmqpCompositeType_t type() const override { return AmqpCompositeType_t::TARGET; }
            const inline AmqpProvidesRequiresList_t& providesList() const override { return s_providesList; }
            static const FieldTypeList_t s_fieldTypeList;

        protected:
        	static AmqpProvidesRequiresList_t s_providesList;
        };

        class AmqpDeleteOnClose: public CompositeType {
        public:
            AmqpDeleteOnClose(AmqpList* fieldList);
            AmqpDeleteOnClose(AmqpList* fieldList, const char* name);
            virtual ~AmqpDeleteOnClose();

            inline const FieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; }
            inline AmqpCompositeType_t type() const override { return AmqpCompositeType_t::DELETE_ON_CLOSE; }
            static const FieldTypeList_t s_fieldTypeList;
        };


        class AmqpDeleteOnNoLinks: public CompositeType {
        public:
            AmqpDeleteOnNoLinks(AmqpList* fieldList);
            AmqpDeleteOnNoLinks(AmqpList* fieldList, const char* name);
            virtual ~AmqpDeleteOnNoLinks();

            inline const FieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; }
            inline AmqpCompositeType_t type() const override { return AmqpCompositeType_t::DELETE_ON_NO_LINKS; }
            static const FieldTypeList_t s_fieldTypeList;
        };


        class AmqpDeleteOnNoMessages: public CompositeType {
        public:
            AmqpDeleteOnNoMessages(AmqpList* fieldList);
            AmqpDeleteOnNoMessages(AmqpList* fieldList, const char* name);
            virtual ~AmqpDeleteOnNoMessages();

            inline const FieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; }
            inline AmqpCompositeType_t type() const override { return AmqpCompositeType_t::DELETE_ON_NO_MESSAGES; }
            static const FieldTypeList_t s_fieldTypeList;
        };


        class AmqpDeleteOnNoLinksOrMessages: public CompositeType {
        public:
            AmqpDeleteOnNoLinksOrMessages(AmqpList* fieldList);
            AmqpDeleteOnNoLinksOrMessages(AmqpList* fieldList, const char* name);
            virtual ~AmqpDeleteOnNoLinksOrMessages();

            inline const FieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; }
            inline AmqpCompositeType_t type() const override { return AmqpCompositeType_t::DELETE_ON_NO_LINKS_OR_MESSAGES; }
            static const FieldTypeList_t s_fieldTypeList;
        };


        class AmqpCoordinator: public CompositeType {
        public:
            AmqpCoordinator(AmqpList* fieldList);
            AmqpCoordinator(AmqpList* fieldList, const char* name);
            virtual ~AmqpCoordinator();

            inline const FieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; }
            inline AmqpCompositeType_t type() const override { return AmqpCompositeType_t::COORDINATOR; }
            const inline AmqpProvidesRequiresList_t& providesList() const override { return s_providesList; }
            static const FieldTypeList_t s_fieldTypeList;

        protected:
        	static AmqpProvidesRequiresList_t s_providesList;
        };


        class AmqpDeclare: public CompositeType {
        public:
            AmqpDeclare(AmqpList* fieldList);
            AmqpDeclare(AmqpList* fieldList, const char* name);
            virtual ~AmqpDeclare();

            inline const FieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; }
            inline AmqpCompositeType_t type() const override { return AmqpCompositeType_t::DECLARE; }
            static const FieldTypeList_t s_fieldTypeList;
        };


        class AmqpDischarge: public CompositeType {
        public:
            AmqpDischarge(AmqpList* fieldList);
            AmqpDischarge(AmqpList* fieldList, const char* name);
            virtual ~AmqpDischarge();

            inline const FieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; }
            inline AmqpCompositeType_t type() const override { return AmqpCompositeType_t::DISCHARGE; }
            static const FieldTypeList_t s_fieldTypeList;
        };


        class AmqpDeclared: public CompositeType {
        public:
            AmqpDeclared(AmqpList* fieldList);
            AmqpDeclared(AmqpList* fieldList, const char* name);
            virtual ~AmqpDeclared();

            inline const FieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; }
            inline AmqpCompositeType_t type() const override { return AmqpCompositeType_t::DECLARED; }
            const inline AmqpProvidesRequiresList_t& providesList() const override { return s_providesList; }
            static const FieldTypeList_t s_fieldTypeList;

        protected:
        	static AmqpProvidesRequiresList_t s_providesList;
        };


        class AmqpTransactionalState: public CompositeType {
        public:
            AmqpTransactionalState(AmqpList* fieldList);
            AmqpTransactionalState(AmqpList* fieldList, const char* name);
            virtual ~AmqpTransactionalState();

            inline const FieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; }
            inline AmqpCompositeType_t type() const override { return AmqpCompositeType_t::TRANSACTIONAL_STATE; }
            const inline AmqpProvidesRequiresList_t& providesList() const override { return s_providesList; }
            static const FieldTypeList_t s_fieldTypeList;

        protected:
        	static AmqpProvidesRequiresList_t s_providesList;
        };


        class AmqpSaslMechanisms: public CompositeType {
        public:
            AmqpSaslMechanisms(AmqpList* fieldList);
            AmqpSaslMechanisms(AmqpList* fieldList, const char* name);
            virtual ~AmqpSaslMechanisms();

            inline const FieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; }
            inline AmqpCompositeType_t type() const override { return AmqpCompositeType_t::SASL_MECHANISMS; }
            static const FieldTypeList_t s_fieldTypeList;
        };


        class AmqpSaslInit: public CompositeType {
        public:
            AmqpSaslInit(AmqpList* fieldList);
            AmqpSaslInit(AmqpList* fieldList, const char* name);
            virtual ~AmqpSaslInit();

            inline const FieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; }
            inline AmqpCompositeType_t type() const override { return AmqpCompositeType_t::SASL_INIT; }
            static const FieldTypeList_t s_fieldTypeList;
        };


        class AmqpSaslChallenge: public CompositeType {
        public:
            AmqpSaslChallenge(AmqpList* fieldList);
            AmqpSaslChallenge(AmqpList* fieldList, const char* name);
            virtual ~AmqpSaslChallenge();

            inline const FieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; }
            inline AmqpCompositeType_t type() const override { return AmqpCompositeType_t::SASL_CHALLENGE; }
            static const FieldTypeList_t s_fieldTypeList;
        };


        class AmqpSaslResponse: public CompositeType {
        public:
            AmqpSaslResponse(AmqpList* fieldList);
            AmqpSaslResponse(AmqpList* fieldList, const char* name);
            virtual ~AmqpSaslResponse();

            inline const FieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; }
            inline AmqpCompositeType_t type() const override { return AmqpCompositeType_t::SASL_RESPONSE; }
            static const FieldTypeList_t s_fieldTypeList;
        };


        class AmqpSaslOutcome: public CompositeType {
        public:
            AmqpSaslOutcome(AmqpList* fieldList);
            AmqpSaslOutcome(AmqpList* fieldList, const char* name);
            virtual ~AmqpSaslOutcome();

            inline const FieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; }
            inline AmqpCompositeType_t type() const override { return AmqpCompositeType_t::SASL_OUTCOME; }
            static const FieldTypeList_t s_fieldTypeList;
        };


    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQP10_TYPE_HPP_ */
