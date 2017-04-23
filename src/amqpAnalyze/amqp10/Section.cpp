/*
 * Section.cpp
 *
 *  Created on: Apr 13, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/amqp10/Section.hpp>

#include <amqpAnalyze/amqp10/Decoder.hpp>
#include <amqpAnalyze/amqp10/FieldType.hpp>
#include <amqpAnalyze/amqp10/FrameBuffer.hpp>
#include <amqpAnalyze/Error.hpp>
#include <iomanip>
#include <std/AnsiTermColors.hpp>

namespace amqpAnalyze
{
    namespace amqp10
    {

        Section::Section(uint64_t packetNum, std::size_t dataOffset): AmqpBlock(packetNum, dataOffset) {}
        Section::~Section() {}
        std::ostringstream& Section::appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin, bool colorFlag) const {
            std::string m(margin, ' ');
            if (!ignoreFirstMargin) oss << "\n" << m;
            oss << "[" << std::setw(4) << std::setfill('0') << std::hex << _dataOffset << "] s ";
            return oss;
        }
        void Section::validate() {}
        // static
        std::map<sectionType_t, const char*> Section::s_sectionTypeName = {
            {sectionType_t::HEADER, "header"},
            {sectionType_t::DELIVERY_ANNOTATIONS, "delivery-annotations"},
            {sectionType_t::MESSAGE_ANNOTATIONS, "message-annotations"},
            {sectionType_t::PROPERTIES, "properties"},
            {sectionType_t::APPLICATION_PROPERTIES, "application-properties"},
            {sectionType_t::RAW_DATA, "data"},
            {sectionType_t::AMQP_SEQUENCE, "amqp-sequence"},
            {sectionType_t::AMQP_VALUE, "amqp-value"},
            {sectionType_t::FOOTER, "footer"}
        };


        AmqpHeader::AmqpHeader(uint64_t packetNum, std::size_t dataOffset, AmqpList* listPtr):
            Section(packetNum, dataOffset),
            _listPtr(listPtr)
        {}
        AmqpHeader::~AmqpHeader() {
            if (_listPtr != nullptr) {
                delete _listPtr;
                _listPtr = nullptr;
            }
        }
        std::ostringstream& AmqpHeader::appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin, bool colorFlag) const {
            Section::appendString(oss, margin, ignoreFirstMargin, colorFlag);
            std::string t(s_sectionTypeName[type()]);
            oss << COLOR(FGND_BYLW, t, colorFlag);
            _listPtr->appendString(oss, margin + t.length() + 9, true, colorFlag);
            return appendStringEpilog(oss, margin + 9, colorFlag);
        }
        void AmqpHeader::validate() {
            Section::validate();
            if (_listPtr != nullptr) {
                _listPtr->validate(s_fieldTypeList, &AmqpBlock::addError, this);
            }
        }
        // static
        const fieldTypeList_t AmqpHeader::s_fieldTypeList = {
            FieldType("durable", amqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType("priority", amqpPrimitiveType_t::UBYTE_TYPE, false, false),
            FieldType("ttl", amqpPrimitiveType_t::MILLISECONDS_TYPE, false, false),
            FieldType("first-acquirer", amqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType("delivery-count", amqpPrimitiveType_t::UINT_TYPE, false, false)
        };



        AmqpDeliveryAnnotations::AmqpDeliveryAnnotations(uint64_t packetNum, std::size_t dataOffset, AmqpAnnotations* annotationsPtr):
            Section(packetNum, dataOffset),
            _annotationsPtr(annotationsPtr)
        {}
        AmqpDeliveryAnnotations::~AmqpDeliveryAnnotations() {
            if (_annotationsPtr != nullptr) {
                delete _annotationsPtr;
                _annotationsPtr = nullptr;
            }
        }
        std::ostringstream& AmqpDeliveryAnnotations::appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin, bool colorFlag) const {
            Section::appendString(oss, margin, ignoreFirstMargin, colorFlag);
            oss << COLOR(FGND_BYLW, s_sectionTypeName[type()], colorFlag);
            _annotationsPtr->appendString(oss, margin, true, colorFlag);
            return appendStringEpilog(oss, margin + 9, colorFlag);
        }
        void AmqpDeliveryAnnotations::validate() {
            Section::validate();
        }




        AmqpMessageAnnotations::AmqpMessageAnnotations(uint64_t packetNum, std::size_t dataOffset, AmqpAnnotations* annotationsPtr):
            Section(packetNum, dataOffset),
            _annotationsPtr(annotationsPtr)
        {}
        AmqpMessageAnnotations::~AmqpMessageAnnotations() {
            if (_annotationsPtr != nullptr) {
                delete _annotationsPtr;
                _annotationsPtr = nullptr;
            }
        }
        std::ostringstream& AmqpMessageAnnotations::appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin, bool colorFlag) const {
            Section::appendString(oss, margin, ignoreFirstMargin, colorFlag);
            oss << COLOR(FGND_BYLW, s_sectionTypeName[type()], colorFlag);
            _annotationsPtr->appendString(oss, margin, true, colorFlag);
            return appendStringEpilog(oss, margin + 9, colorFlag);
        }
        void AmqpMessageAnnotations::validate() {
            Section::validate();
        }




        AmqpProperties::AmqpProperties(uint64_t packetNum, std::size_t dataOffset, AmqpList* listPtr):
            Section(packetNum, dataOffset),
            _listPtr(listPtr)
        {}
        AmqpProperties::~AmqpProperties() {
            if (_listPtr != nullptr) {
                delete _listPtr;
                _listPtr = nullptr;
            }
        }
        std::ostringstream& AmqpProperties::appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin, bool colorFlag) const {
            Section::appendString(oss, margin, ignoreFirstMargin, colorFlag);
            std::string t(s_sectionTypeName[type()]);
            oss << COLOR(FGND_BYLW, s_sectionTypeName[type()], colorFlag);
            _listPtr->appendString(oss, margin + t.length() + 9, true, colorFlag);
            return appendStringEpilog(oss, margin + 9, colorFlag);
        }
        void AmqpProperties::validate() {
            Section::validate();
            if (_listPtr != nullptr) {
                _listPtr->validate(s_fieldTypeList, &AmqpBlock::addError, this);
            }
        }
        // static
        const fieldTypeList_t AmqpProperties::s_fieldTypeList = {
            FieldType("message-id", "*", false, false, {amqpRequiresProvides_t::MESSAGE_ID}),
            FieldType("user-id", amqpPrimitiveType_t::BINARY_TYPE, false, false),
            FieldType("to", "*", false, false, {amqpRequiresProvides_t::ADDRESS}),
            FieldType("subject", amqpPrimitiveType_t::STRING_TYPE, false, false),
            FieldType("reply-to", "*", false, false, {amqpRequiresProvides_t::ADDRESS}),
            FieldType("correlation-id", "*", false, false, {amqpRequiresProvides_t::MESSAGE_ID}),
            FieldType("content-type", amqpPrimitiveType_t::SYMBOL_TYPE, false, false),
            FieldType("content-encoding", amqpPrimitiveType_t::SYMBOL_TYPE, false, false),
            FieldType("absolute-expiry-time", amqpPrimitiveType_t::TIMESTAMP_TYPE, false, false),
            FieldType("creation-time", amqpPrimitiveType_t::TIMESTAMP_TYPE, false, false),
            FieldType("group-id", amqpPrimitiveType_t::STRING_TYPE, false, false),
            FieldType("group-sequence", amqpPrimitiveType_t::SEQUENCE_NUMBER_TYPE, false, false),
            FieldType("reply-to-group-id", amqpPrimitiveType_t::STRING_TYPE, false, false)
        };




        AmqpApplicationProperties::AmqpApplicationProperties(uint64_t packetNum, std::size_t dataOffset, AmqpMap* mapPtr):
            Section(packetNum, dataOffset),
            _mapPtr(mapPtr)
        {}
        AmqpApplicationProperties::~AmqpApplicationProperties() {
            if (_mapPtr != nullptr) {
                delete _mapPtr;
                _mapPtr = nullptr;
            }
        }
        std::ostringstream& AmqpApplicationProperties::appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin, bool colorFlag) const {
            Section::appendString(oss, margin, ignoreFirstMargin, colorFlag);
            oss << COLOR(FGND_BYLW, s_sectionTypeName[type()], colorFlag);
            _mapPtr->appendString(oss, margin, true, colorFlag);
            return appendStringEpilog(oss, margin + 9, colorFlag);
        }
        void AmqpApplicationProperties::validate() {
            Section::validate();
        }




        AmqpData::AmqpData(uint64_t packetNum, std::size_t dataOffset, AmqpBinary* binaryPtr):
            Section(packetNum, dataOffset),
            _binaryPtr(binaryPtr)
        {}
        AmqpData::~AmqpData() {
            if (_binaryPtr != nullptr) {
                delete _binaryPtr;
                _binaryPtr = nullptr;
            }
        }
        std::ostringstream& AmqpData::appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin, bool colorFlag) const {
            Section::appendString(oss, margin, ignoreFirstMargin, colorFlag);
            oss << COLOR(FGND_BYLW, s_sectionTypeName[type()], colorFlag) << ": " << _binaryPtr->typeValueStr(colorFlag);
            return appendStringEpilog(oss, margin + 9, colorFlag);
        }
        void AmqpData::validate() {
            Section::validate();
        }




        AmqpSequence::AmqpSequence(uint64_t packetNum, std::size_t dataOffset, AmqpList* listPtr):
            Section(packetNum, dataOffset),
            _listPtr(listPtr)
        {}
        AmqpSequence::~AmqpSequence() {
            if (_listPtr != nullptr) {
                delete _listPtr;
                _listPtr = nullptr;
            }
        }
        std::ostringstream& AmqpSequence::appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin, bool colorFlag) const {
            Section::appendString(oss, margin, ignoreFirstMargin, colorFlag);
            std::string t(s_sectionTypeName[type()]);
            oss << COLOR(FGND_BYLW, t, colorFlag);
            _listPtr->appendString(oss, margin + t.length() + 9, true, colorFlag);
            return appendStringEpilog(oss, margin + 9, colorFlag);
        }
        void AmqpSequence::validate() {
            Section::validate();
        }




        AmqpValue::AmqpValue(uint64_t packetNum, std::size_t dataOffset, PrimitiveType* valuePtr):
            Section(packetNum, dataOffset),
            _valuePtr(valuePtr)
        {}
        AmqpValue::~AmqpValue() {
            if (_valuePtr != nullptr) {
                delete _valuePtr;
                _valuePtr = nullptr;
            }
        }
        std::ostringstream& AmqpValue::appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin, bool colorFlag) const {
            Section::appendString(oss, margin, ignoreFirstMargin, colorFlag);
            oss << COLOR(FGND_BYLW, s_sectionTypeName[type()], colorFlag) << ": " << _valuePtr->typeValueStr(colorFlag);
            return appendStringEpilog(oss, margin + 9, colorFlag);
        }
        void AmqpValue::validate() {
            Section::validate();
        }




        AmqpFooter::AmqpFooter(uint64_t packetNum, std::size_t dataOffset, AmqpAnnotations* annotationsPtr):
            Section(packetNum, dataOffset),
            _annotationsPtr(annotationsPtr)
        {}
        AmqpFooter::~AmqpFooter() {
            if (_annotationsPtr != nullptr) {
                delete _annotationsPtr;
                _annotationsPtr = nullptr;
            }
        }
        std::ostringstream& AmqpFooter::appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin, bool colorFlag) const {
            Section::appendString(oss, margin, ignoreFirstMargin, colorFlag);
            oss << COLOR(FGND_BYLW, s_sectionTypeName[type()], colorFlag) << ": ";
            _annotationsPtr->appendString(oss, margin, ignoreFirstMargin, colorFlag);
            return appendStringEpilog(oss, margin + 9, colorFlag);
        }
        void AmqpFooter::validate() {
            Section::validate();
        }


    } /* namespace amqp10 */
} /* namespace amqpAnalyze */
