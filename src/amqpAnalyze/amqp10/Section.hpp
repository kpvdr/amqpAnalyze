/*
 * Section.hpp
 *
 *  Created on: Apr 13, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_AMQP10_SECTION_HPP_
#define SRC_AMQPANALYZE_AMQP10_SECTION_HPP_

#include <amqpAnalyze/amqp10/AmqpBlock.hpp>

namespace amqpAnalyze
{
    namespace amqp10
    {
        class AmqpAnnotations;
        class AmqpBinary;
        class AmqpList;
        class AmqpMap;

        enum class SectionType_t:uint64_t {
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
            Section(uint64_t packetNum, std::size_t dataOffset);
            virtual ~Section();

            virtual std::ostringstream&  appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const override;
            inline AmqpBlockType_t blockType() const override { return AmqpBlockType_t::SECTION; }
            virtual SectionType_t type() const = 0;
            void validate() override;

        protected:
            static std::map<SectionType_t, const char*> s_sectionTypeName;
        };

        typedef std::vector<Section*> sectionPtrList_t;
        typedef sectionPtrList_t::iterator sectionPtrList_itr_t;
        typedef sectionPtrList_t::const_iterator sectionPtrList_citr_t;


        class AmqpHeader: public Section {
        public:
            AmqpHeader(uint64_t packetNum, std::size_t dataOffset, AmqpList* listPtr);
            virtual ~AmqpHeader();
            std::ostringstream&  appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const override;
            inline SectionType_t type() const override { return SectionType_t::HEADER; };
            void validate() override;
            static const FieldTypeList_t s_fieldTypeList;
        protected:
            AmqpList* _listPtr;
        };



        class AmqpDeliveryAnnotations: public Section {
        public:
            AmqpDeliveryAnnotations(uint64_t packetNum, std::size_t dataOffset, AmqpAnnotations* annotationsPtr);
            virtual ~AmqpDeliveryAnnotations();
            std::ostringstream&  appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const override;
            inline SectionType_t type() const override { return SectionType_t::DELIVERY_ANNOTATIONS; };
            void validate() override;
        protected:
            AmqpAnnotations* _annotationsPtr;
        };




        class AmqpMessageAnnotations: public Section {
        public:
            AmqpMessageAnnotations(uint64_t packetNum, std::size_t dataOffset, AmqpAnnotations* annotationsPtr);
            virtual ~AmqpMessageAnnotations();
            std::ostringstream&  appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const override;
            inline SectionType_t type() const override { return SectionType_t::MESSAGE_ANNOTATIONS; };
            void validate() override;
        protected:
            AmqpAnnotations* _annotationsPtr;
        };




        class AmqpProperties: public Section {
        public:
            AmqpProperties(uint64_t packetNum, std::size_t dataOffset, AmqpList* listPtr);
            virtual ~AmqpProperties();
            std::ostringstream&  appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const override;
            inline SectionType_t type() const override { return SectionType_t::PROPERTIES; };
            void validate() override;
            static const FieldTypeList_t s_fieldTypeList;
        protected:
            AmqpList* _listPtr;
        };




        class AmqpApplicationProperties: public Section {
        public:
            AmqpApplicationProperties(uint64_t packetNum, std::size_t dataOffset, AmqpMap* mapPtr);
            virtual ~AmqpApplicationProperties();
            std::ostringstream&  appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const override;
            inline SectionType_t type() const override { return SectionType_t::APPLICATION_PROPERTIES; };
            void validate() override;
        protected:
            AmqpMap* _mapPtr;
        };




        class AmqpData: public Section {
        public:
            AmqpData(uint64_t packetNum, std::size_t dataOffset, AmqpBinary* binaryPtr);
            virtual ~AmqpData();
            std::ostringstream&  appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const override;
            inline SectionType_t type() const override { return SectionType_t::RAW_DATA; };
            void validate() override;
        protected:
            AmqpBinary* _binaryPtr;
       };




        class AmqpSequence: public Section {
        public:
            AmqpSequence(uint64_t packetNum, std::size_t dataOffset, AmqpList* listPtr);
            virtual ~AmqpSequence();
            std::ostringstream&  appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const override;
            inline SectionType_t type() const override { return SectionType_t::AMQP_SEQUENCE; };
            void validate() override;
        protected:
            AmqpList* _listPtr;
       };




        class AmqpValue: public Section {
        public:
            AmqpValue(uint64_t packetNum, std::size_t dataOffset, PrimitiveType* valuePtr);
            virtual ~AmqpValue();
            std::ostringstream&  appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const override;
            inline SectionType_t type() const override { return SectionType_t::AMQP_VALUE; };
            void validate() override;
        protected:
            PrimitiveType* _valuePtr;
        };




        class AmqpFooter: public Section {
        public:
            AmqpFooter(uint64_t packetNum, std::size_t dataOffset, AmqpAnnotations* annotationsPtr);
            virtual ~AmqpFooter();
            std::ostringstream&  appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const override;
            inline SectionType_t type() const override { return SectionType_t::FOOTER; };
            void validate() override;
        protected:
            AmqpAnnotations* _annotationsPtr;
        };


    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQP10_SECTION_HPP_ */
