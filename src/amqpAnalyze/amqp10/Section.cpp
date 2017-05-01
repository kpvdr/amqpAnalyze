/*
 * Section.cpp
 *
 *  Created on: Apr 13, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/amqp10/Section.hpp>

#include <amqpAnalyze/amqp10/FieldType.hpp>
#include <amqpAnalyze/amqp10/ProvidesRequires.hpp>
#include <amqpAnalyze/amqp10/Type.hpp>
#include <amqpAnalyze/Color.hpp>
#include <amqpAnalyze/Options.hpp>
#include <iomanip>

namespace amqpAnalyze
{
    namespace amqp10
    {

        Section::Section(uint64_t packetNum, std::size_t dataOffset): AmqpBlock(packetNum, dataOffset) {}
        Section::~Section() {}
        std::ostringstream& Section::appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const {
            std::string m(margin, ' ');
            if (!ignoreFirstMargin) oss << "\n" << m;
            oss << "[" << std::setw(4) << std::setfill('0') << std::hex << _dataOffset << "] s ";
            return oss;
        }
        void Section::validate() {}
        // static
        std::map<SectionType_t, const char*> Section::s_sectionTypeName = {
            {SectionType_t::HEADER, "header"},
            {SectionType_t::DELIVERY_ANNOTATIONS, "delivery-annotations"},
            {SectionType_t::MESSAGE_ANNOTATIONS, "message-annotations"},
            {SectionType_t::PROPERTIES, "properties"},
            {SectionType_t::APPLICATION_PROPERTIES, "application-properties"},
            {SectionType_t::RAW_DATA, "data"},
            {SectionType_t::AMQP_SEQUENCE, "amqp-sequence"},
            {SectionType_t::AMQP_VALUE, "amqp-value"},
            {SectionType_t::FOOTER, "footer"}
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
        std::ostringstream& AmqpHeader::appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const {
            Section::appendString(oss, margin, ignoreFirstMargin);
            std::string t(s_sectionTypeName[type()]);
            oss << Color::color(DisplayColorType_t::AMQP_SECTION, t);
            _listPtr->appendString(oss, margin + t.length() + 9, true, g_optionsPtr->s_colorFlag);
            return appendStringEpilog(oss, margin + 9);
        }
        void AmqpHeader::validate() {
            Section::validate();
            if (_listPtr != nullptr) {
                _listPtr->validate(s_fieldTypeList, &AmqpBlock::addError, this);
            }
        }
        // static
        const FieldTypeList_t AmqpHeader::s_fieldTypeList = {
            FieldType("durable", AmqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType("priority", AmqpPrimitiveType_t::UBYTE_TYPE, false, false),
            FieldType("ttl", AmqpPrimitiveType_t::MILLISECONDS_TYPE, false, false),
            FieldType("first-acquirer", AmqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType("delivery-count", AmqpPrimitiveType_t::UINT_TYPE, false, false)
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
        std::ostringstream& AmqpDeliveryAnnotations::appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const {
            Section::appendString(oss, margin, ignoreFirstMargin);
            oss << Color::color(DisplayColorType_t::AMQP_SECTION, s_sectionTypeName[type()]);
            _annotationsPtr->appendString(oss, margin, true, g_optionsPtr->s_colorFlag);
            return appendStringEpilog(oss, margin + 9);
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
        std::ostringstream& AmqpMessageAnnotations::appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const {
            Section::appendString(oss, margin, ignoreFirstMargin);
            oss << Color::color(DisplayColorType_t::AMQP_SECTION, s_sectionTypeName[type()]);
            _annotationsPtr->appendString(oss, margin, true, g_optionsPtr->s_colorFlag);
            return appendStringEpilog(oss, margin + 9);
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
        std::ostringstream& AmqpProperties::appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const {
            Section::appendString(oss, margin, ignoreFirstMargin);
            std::string t(s_sectionTypeName[type()]);
            oss << Color::color(DisplayColorType_t::AMQP_SECTION, s_sectionTypeName[type()]);
            _listPtr->appendString(oss, margin + t.length() + 9, true, g_optionsPtr->s_colorFlag);
            return appendStringEpilog(oss, margin + 9);
        }
        void AmqpProperties::validate() {
            Section::validate();
            if (_listPtr != nullptr) {
                _listPtr->validate(s_fieldTypeList, &AmqpBlock::addError, this);
            }
        }
        // static
        const FieldTypeList_t AmqpProperties::s_fieldTypeList = {
            FieldType("message-id", "*", false, false, {AmqpRequiresProvides_t::MESSAGE_ID}),
            FieldType("user-id", AmqpPrimitiveType_t::BINARY_TYPE, false, false),
            FieldType("to", "*", false, false, {AmqpRequiresProvides_t::ADDRESS}),
            FieldType("subject", AmqpPrimitiveType_t::STRING_TYPE, false, false),
            FieldType("reply-to", "*", false, false, {AmqpRequiresProvides_t::ADDRESS}),
            FieldType("correlation-id", "*", false, false, {AmqpRequiresProvides_t::MESSAGE_ID}),
            FieldType("content-type", AmqpPrimitiveType_t::SYMBOL_TYPE, false, false),
            FieldType("content-encoding", AmqpPrimitiveType_t::SYMBOL_TYPE, false, false),
            FieldType("absolute-expiry-time", AmqpPrimitiveType_t::TIMESTAMP_TYPE, false, false),
            FieldType("creation-time", AmqpPrimitiveType_t::TIMESTAMP_TYPE, false, false),
            FieldType("group-id", AmqpPrimitiveType_t::STRING_TYPE, false, false),
            FieldType("group-sequence", AmqpPrimitiveType_t::SEQUENCE_NUMBER_TYPE, false, false),
            FieldType("reply-to-group-id", AmqpPrimitiveType_t::STRING_TYPE, false, false)
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
        std::ostringstream& AmqpApplicationProperties::appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const {
            Section::appendString(oss, margin, ignoreFirstMargin);
            oss << Color::color(DisplayColorType_t::AMQP_SECTION, s_sectionTypeName[type()]);
            _mapPtr->appendString(oss, margin, true, g_optionsPtr->s_colorFlag);
            return appendStringEpilog(oss, margin + 9);
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
        std::ostringstream& AmqpData::appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const {
            Section::appendString(oss, margin, ignoreFirstMargin);
            oss << Color::color(DisplayColorType_t::AMQP_SECTION, s_sectionTypeName[type()]) << ": " << _binaryPtr->typeValueStr(g_optionsPtr->s_colorFlag);
            return appendStringEpilog(oss, margin + 9);
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
        std::ostringstream& AmqpSequence::appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const {
            Section::appendString(oss, margin, ignoreFirstMargin);
            std::string t(s_sectionTypeName[type()]);
            oss << Color::color(DisplayColorType_t::AMQP_SECTION, t);
            _listPtr->appendString(oss, margin + t.length() + 9, true, g_optionsPtr->s_colorFlag);
            return appendStringEpilog(oss, margin + 9);
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
        std::ostringstream& AmqpValue::appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const {
            Section::appendString(oss, margin, ignoreFirstMargin);
            oss << Color::color(DisplayColorType_t::AMQP_SECTION, s_sectionTypeName[type()]) << ": " << _valuePtr->typeValueStr(g_optionsPtr->s_colorFlag);
            return appendStringEpilog(oss, margin + 9);
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
        std::ostringstream& AmqpFooter::appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const {
            Section::appendString(oss, margin, ignoreFirstMargin);
            oss << Color::color(DisplayColorType_t::AMQP_SECTION, s_sectionTypeName[type()]) << ": ";
            _annotationsPtr->appendString(oss, margin, ignoreFirstMargin, g_optionsPtr->s_colorFlag);
            return appendStringEpilog(oss, margin + 9);
        }
        void AmqpFooter::validate() {
            Section::validate();
        }


    } /* namespace amqp10 */
} /* namespace amqpAnalyze */
