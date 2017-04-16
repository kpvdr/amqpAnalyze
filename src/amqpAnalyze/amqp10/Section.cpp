/*
 * Section.cpp
 *
 *  Created on: Apr 13, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/amqp10/Section.hpp>

#include <amqpAnalyze/amqp10/FrameBuffer.hpp>
#include <amqpAnalyze/Error.hpp>

#include <iomanip>

namespace amqpAnalyze
{
    namespace amqp10
    {

        Section::Section(std::size_t frameOffset): _frameOffset(frameOffset) {}
        Section::~Section() {}
        void Section::appendString(std::ostringstream& oss, std::size_t margin) const {
            oss << "[" << std::setw(4) << std::setfill('0') << std::hex << _frameOffset
                            << "] + ";
        }
        // static
        Section* Section::decode(FrameBuffer& frameBuffer) {
            size_t frameOffset = frameBuffer.getOffset();
            const uint8_t lfb = frameBuffer.getUint8();
            if (lfb != 0) {
                throw amqpAnalyze::Error(MSG("[" << std::hex << std::setfill('0') << std::setw(4) << frameOffset
                                             << "] Unexpected leading frame byte: expected 0x0, found 0x" << (int)lfb));
            }
            std::unique_ptr<PrimitiveType> descriptorPtr((PrimitiveType*)Type::decode(frameBuffer));
            switch (descriptorPtr->type()) {
                case amqpPrimitiveType_t::ULONG_TYPE: {
                    AmqpUlong* longDescriptorPtr = (AmqpUlong*)descriptorPtr.get(); // TODO: This feels ugly, need more elegant solution here
                    switch((sectionType_t)longDescriptorPtr->value()) {
                        case sectionType_t::HEADER:
                            return new AmqpHeader(frameOffset, (AmqpList*)Type::decode(frameBuffer));
                        case sectionType_t::DELIVERY_ANNOTATIONS:
                            return new AmqpDeliveryAnnotations(frameOffset, (AmqpAnnotations*)Type::decode(frameBuffer));
                        case sectionType_t::MESSAGE_ANNOTATIONS:
                            return new AmqpMessageAnnotations(frameOffset, (AmqpAnnotations*)Type::decode(frameBuffer));
                        case sectionType_t::PROPERTIES:
                            return new AmqpProperties(frameOffset, (AmqpList*)Type::decode(frameBuffer));
                        case sectionType_t::APPLICATION_PROPERTIES:
                            return new AmqpApplicationProperties(frameOffset, (AmqpMap*)Type::decode(frameBuffer));
                        case sectionType_t::RAW_DATA:
                            return new AmqpData(frameOffset, (AmqpBinary*)Type::decode(frameBuffer));
                        case sectionType_t::AMQP_SEQUENCE:
                            return new AmqpSequence(frameOffset, (AmqpList*)Type::decode(frameBuffer));
                        case sectionType_t::AMQP_VALUE:
                            return new AmqpValue(frameOffset, (PrimitiveType*)Type::decode(frameBuffer));
                        case sectionType_t::FOOTER:
                            return new AmqpFooter(frameOffset, (AmqpAnnotations*)Type::decode(frameBuffer));
                        default:;
                            throw amqpAnalyze::Error(MSG("[" << std::hex << std::setfill('0') << std::setw(4) << frameOffset
                                                         << "] Invalid AMQP section descriptor: " << longDescriptorPtr->valueStr()));
                    }
                }
                case  amqpPrimitiveType_t::SYMBOL_TYPE: {
                    throw amqpAnalyze::Error(MSG("[" << std::hex << std::setfill('0') << std::setw(4) << frameOffset
                                                 << "] AMQP symbol descriptors not handled (" << descriptorPtr->typeStr() << ")"));
                }
                default:
                    throw amqpAnalyze::Error(MSG("[" << std::hex << std::setfill('0') << std::setw(4) << frameOffset
                                                 << "] Invalid AMQP descriptor type: " << descriptorPtr->typeStr()));
            }
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
        void AmqpHeader::appendString(std::ostringstream& oss, std::size_t margin) const {
            Section::appendString(oss, margin);
            _listPtr->appendString(oss, margin);
        }



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
        void AmqpDeliveryAnnotations::appendString(std::ostringstream& oss, std::size_t margin) const {
            Section::appendString(oss, margin);
            _annotationsPtr->appendString(oss, margin);
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
        void AmqpMessageAnnotations::appendString(std::ostringstream& oss, std::size_t margin) const {
            Section::appendString(oss, margin);
            _annotationsPtr->appendString(oss, margin);
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
        void AmqpProperties::appendString(std::ostringstream& oss, std::size_t margin) const {
            Section::appendString(oss, margin);
            _listPtr->appendString(oss, margin);
        }




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
        void AmqpApplicationProperties::appendString(std::ostringstream& oss, std::size_t margin) const {
            Section::appendString(oss, margin);
            _mapPtr->appendString(oss, margin);
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
        void AmqpData::appendString(std::ostringstream& oss, std::size_t margin) const {
            Section::appendString(oss, margin);
            oss << _binaryPtr->typeValueStr();
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
        void AmqpSequence::appendString(std::ostringstream& oss, std::size_t margin) const {
            Section::appendString(oss, margin);
            _listPtr->appendString(oss, margin);
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
        void AmqpValue::appendString(std::ostringstream& oss, std::size_t margin) const {
            Section::appendString(oss, margin);
            oss << _valuePtr->typeValueStr();
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
        void AmqpFooter::appendString(std::ostringstream& oss, std::size_t margin) const {
            Section::appendString(oss, margin);
            _annotationsPtr->appendString(oss, margin);
        }


    } /* namespace amqp10 */
} /* namespace amqpAnalyze */
