/*
 * Section.hpp
 *
 *  Created on: Apr 13, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_AMQP10_SECTION_HPP_
#define SRC_AMQPANALYZE_AMQP10_SECTION_HPP_

#include <amqpAnalyze/amqp10/AmqpBlock.hpp>
#include <cstdint>
#include <map>
#include <sstream>
#include <vector>

namespace amqpAnalyze
{
    namespace amqp10
    {

        class AmqpAnnotations;
        class AmqpBinary;
        class AmqpList;
        class AmqpMap;
        class FieldType;
        typedef std::vector<FieldType> fieldTypeList_t;
        class FrameBuffer;
        class PrimitiveType;

        enum class sectionType_t:uint64_t {
            HEADER=0x70,
            DELIVERY_ANNOTATIONS,
            MESSAGE_ANNOTATIONS,
            PROPERTIES,
            APPLICATION_PROPERTIES,
            RAW_DATA,
            AMQP_SEQUENCE,
            AMQP_VALUE,
            FOOTER
        };
        class Section: public AmqpBlock
        {
        public:
            Section(std::size_t dataOffset);
            virtual ~Section();
            virtual std::ostringstream&  appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const override;
            virtual sectionType_t type() const = 0;
        protected:
            static std::map<sectionType_t, const char*> s_sectionTypeName;
        };

        typedef std::vector<Section*> sectionPtrList_t;
        typedef sectionPtrList_t::iterator sectionPtrList_itr_t;
        typedef sectionPtrList_t::const_iterator sectionPtrList_citr_t;


        class AmqpHeader: public Section {
        public:
            AmqpHeader(std::size_t frameOffset, AmqpList* listPtr);
            virtual ~AmqpHeader();
            std::ostringstream&  appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const override;
            inline sectionType_t type() const { return sectionType_t::HEADER; };
            static fieldTypeList_t s_fieldTypeList;
        protected:
            AmqpList* _listPtr;
        };



        class AmqpDeliveryAnnotations: public Section {
        public:
            AmqpDeliveryAnnotations(std::size_t frameOffset, AmqpAnnotations* annotationsPtr);
            virtual ~AmqpDeliveryAnnotations();
            std::ostringstream&  appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const override;
            inline sectionType_t type() const { return sectionType_t::DELIVERY_ANNOTATIONS; };
        protected:
            AmqpAnnotations* _annotationsPtr;
        };




        class AmqpMessageAnnotations: public Section {
        public:
            AmqpMessageAnnotations(std::size_t frameOffset, AmqpAnnotations* annotationsPtr);
            virtual ~AmqpMessageAnnotations();
            std::ostringstream&  appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const override;
            inline sectionType_t type() const { return sectionType_t::MESSAGE_ANNOTATIONS; };
        protected:
            AmqpAnnotations* _annotationsPtr;
        };




        class AmqpProperties: public Section {
        public:
            AmqpProperties(std::size_t frameOffset, AmqpList* listPtr);
            virtual ~AmqpProperties();
            std::ostringstream&  appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const override;
            inline sectionType_t type() const { return sectionType_t::PROPERTIES; };
            static fieldTypeList_t s_fieldTypeList;
        protected:
            AmqpList* _listPtr;
        };




        class AmqpApplicationProperties: public Section {
        public:
            AmqpApplicationProperties(std::size_t frameOffset, AmqpMap* mapPtr);
            virtual ~AmqpApplicationProperties();
            std::ostringstream&  appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const override;
            inline sectionType_t type() const { return sectionType_t::APPLICATION_PROPERTIES; };
        protected:
            AmqpMap* _mapPtr;
        };




        class AmqpData: public Section {
        public:
            AmqpData(std::size_t frameOffset, AmqpBinary* binaryPtr);
            virtual ~AmqpData();
            std::ostringstream&  appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const override;
            inline sectionType_t type() const { return sectionType_t::RAW_DATA; };
        protected:
            AmqpBinary* _binaryPtr;
       };




        class AmqpSequence: public Section {
        public:
            AmqpSequence(std::size_t frameOffset, AmqpList* listPtr);
            virtual ~AmqpSequence();
            std::ostringstream&  appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const override;
            inline sectionType_t type() const { return sectionType_t::AMQP_SEQUENCE; };
        protected:
            AmqpList* _listPtr;
       };




        class AmqpValue: public Section {
        public:
            AmqpValue(std::size_t frameOffset, PrimitiveType* valuePtr);
            virtual ~AmqpValue();
            std::ostringstream&  appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const override;
            inline sectionType_t type() const { return sectionType_t::AMQP_VALUE; };
        protected:
            PrimitiveType* _valuePtr;
        };




        class AmqpFooter: public Section {
        public:
            AmqpFooter(std::size_t frameOffset, AmqpAnnotations* annotationsPtr);
            virtual ~AmqpFooter();
            std::ostringstream&  appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const override;
            inline sectionType_t type() const { return sectionType_t::FOOTER; };
        protected:
            AmqpAnnotations* _annotationsPtr;
        };


    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQP10_SECTION_HPP_ */
