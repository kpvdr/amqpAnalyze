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
#include <std/AnsiTermColors.hpp>

#include <iomanip>

namespace amqpAnalyze
{
    namespace amqp10
    {

        Section::Section(std::size_t dataOffset): AmqpBlock(dataOffset) {}
        Section::~Section() {}
        std::ostringstream& Section::appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const {
            if (!ignoreFirstMargin) oss << "\n" << std::string(margin, ' ');
            oss << "[" << std::setw(4) << std::setfill('0') << std::hex << _dataOffset << "] s ";
        }
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


        AmqpHeader::AmqpHeader(std::size_t frameOffset, AmqpList* listPtr):
            Section(frameOffset),
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
            oss << std::b_yellow << t << std::res;
            _listPtr->appendString(oss, margin + t.length() + 9, true);
            return oss;
        }
        // static
        fieldTypeList_t AmqpHeader::s_fieldTypeList = {
            FieldType("durable", amqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType("priority", amqpPrimitiveType_t::UBYTE_TYPE, false, false),
            FieldType("ttl", amqpPrimitiveType_t::MILLISECONDS_TYPE, false, false),
            FieldType("first-acquirer", amqpPrimitiveType_t::BOOLEAN_TYPE, false, false),
            FieldType("delivery-count", amqpPrimitiveType_t::UINT_TYPE, false, false)
        };



        AmqpDeliveryAnnotations::AmqpDeliveryAnnotations(std::size_t frameOffset, AmqpAnnotations* annotationsPtr):
            Section(frameOffset),
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
            oss << std::b_yellow << s_sectionTypeName[type()] << std::res << ": ";
            _annotationsPtr->appendString(oss, margin, true);
            return oss;
        }




        AmqpMessageAnnotations::AmqpMessageAnnotations(std::size_t frameOffset, AmqpAnnotations* annotationsPtr):
            Section(frameOffset),
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
            oss << std::b_yellow << s_sectionTypeName[type()] << std::res << ": ";
            _annotationsPtr->appendString(oss, margin, true);
            return oss;
        }




        AmqpProperties::AmqpProperties(std::size_t frameOffset, AmqpList* listPtr):
            Section(frameOffset),
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
            oss << std::b_yellow << t << std::res;
            _listPtr->appendString(oss, margin + t.length() + 9, true);
            return oss;
        }
        // static
        fieldTypeList_t AmqpProperties::s_fieldTypeList = {
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




        AmqpApplicationProperties::AmqpApplicationProperties(std::size_t frameOffset, AmqpMap* mapPtr):
            Section(frameOffset),
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
            oss << std::b_yellow << s_sectionTypeName[type()] << std::res << ": ";
            _mapPtr->appendString(oss, margin, true);
            return oss;
        }




        AmqpData::AmqpData(std::size_t frameOffset, AmqpBinary* binaryPtr):
            Section(frameOffset),
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
            oss << std::b_yellow << s_sectionTypeName[type()] << std::res << ": " << _binaryPtr->typeValueStr();
            return oss;
        }




        AmqpSequence::AmqpSequence(std::size_t frameOffset, AmqpList* listPtr):
            Section(frameOffset),
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
            oss << std::b_yellow << t << std::res;
            _listPtr->appendString(oss, margin + t.length() + 9, true);
            return oss;
        }




        AmqpValue::AmqpValue(std::size_t frameOffset, PrimitiveType* valuePtr):
            Section(frameOffset),
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
            oss << std::b_yellow << s_sectionTypeName[type()] << std::res << ": " << _valuePtr->typeValueStr();
            return oss;
        }




        AmqpFooter::AmqpFooter(std::size_t frameOffset, AmqpAnnotations* annotationsPtr):
            Section(frameOffset),
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
            oss << std::b_yellow << s_sectionTypeName[type()] << std::res << ": ";
            _annotationsPtr->appendString(oss, margin, ignoreFirstMargin);
            return oss;
        }


    } /* namespace amqp10 */
} /* namespace amqpAnalyze */
