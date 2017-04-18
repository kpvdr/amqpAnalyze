/*
 * Section.hpp
 *
 *  Created on: Apr 13, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_AMQP10_SECTION_HPP_
#define SRC_AMQPANALYZE_AMQP10_SECTION_HPP_

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
        class Section {
        public:
            Section(std::size_t frameOffset);
            virtual ~Section();
            virtual void appendString(std::ostringstream& oss, std::size_t margin) const;
            virtual sectionType_t type() const = 0;
            static Section* decode(FrameBuffer& frameBuffer);
        protected:
            const std::size_t _frameOffset;
            static std::map<sectionType_t, const char*> s_sectionTypeName;
        };

        typedef std::vector<Section*> sectionPtrList_t;
        typedef sectionPtrList_t::iterator sectionPtrList_itr_t;
        typedef sectionPtrList_t::const_iterator sectionPtrList_citr_t;


        class AmqpHeader: public Section {
        public:
            AmqpHeader(std::size_t frameOffset, AmqpList* listPtr);
            virtual ~AmqpHeader();
            void appendString(std::ostringstream& oss, std::size_t margin) const override;
            inline sectionType_t type() const { return sectionType_t::HEADER; };
        protected:
            AmqpList* _listPtr;
            static fieldTypeList_t s_fieldTypeList;
        };



        class AmqpDeliveryAnnotations: public Section {
        public:
            AmqpDeliveryAnnotations(std::size_t frameOffset, AmqpAnnotations* annotationsPtr);
            virtual ~AmqpDeliveryAnnotations();
            void appendString(std::ostringstream& oss, std::size_t margin) const override;
            inline sectionType_t type() const { return sectionType_t::DELIVERY_ANNOTATIONS; };
        protected:
            AmqpAnnotations* _annotationsPtr;
        };




        class AmqpMessageAnnotations: public Section {
        public:
            AmqpMessageAnnotations(std::size_t frameOffset, AmqpAnnotations* annotationsPtr);
            virtual ~AmqpMessageAnnotations();
            void appendString(std::ostringstream& oss, std::size_t margin) const override;
            inline sectionType_t type() const { return sectionType_t::MESSAGE_ANNOTATIONS; };
        protected:
            AmqpAnnotations* _annotationsPtr;
        };




        class AmqpProperties: public Section {
        public:
            AmqpProperties(std::size_t frameOffset, AmqpList* listPtr);
            virtual ~AmqpProperties();
            void appendString(std::ostringstream& oss, std::size_t margin) const override;
            inline sectionType_t type() const { return sectionType_t::PROPERTIES; };
        protected:
            AmqpList* _listPtr;
            static fieldTypeList_t s_fieldTypeList;
        };




        class AmqpApplicationProperties: public Section {
        public:
            AmqpApplicationProperties(std::size_t frameOffset, AmqpMap* mapPtr);
            virtual ~AmqpApplicationProperties();
            void appendString(std::ostringstream& oss, std::size_t margin) const override;
            inline sectionType_t type() const { return sectionType_t::APPLICATION_PROPERTIES; };
        protected:
            AmqpMap* _mapPtr;
        };




        class AmqpData: public Section {
        public:
            AmqpData(std::size_t frameOffset, AmqpBinary* binaryPtr);
            virtual ~AmqpData();
            void appendString(std::ostringstream& oss, std::size_t margin) const override;
            inline sectionType_t type() const { return sectionType_t::RAW_DATA; };
        protected:
            AmqpBinary* _binaryPtr;
       };




        class AmqpSequence: public Section {
        public:
            AmqpSequence(std::size_t frameOffset, AmqpList* listPtr);
            virtual ~AmqpSequence();
            void appendString(std::ostringstream& oss, std::size_t margin) const override;
            inline sectionType_t type() const { return sectionType_t::AMQP_SEQUENCE; };
        protected:
            AmqpList* _listPtr;
       };




        class AmqpValue: public Section {
        public:
            AmqpValue(std::size_t frameOffset, PrimitiveType* valuePtr);
            virtual ~AmqpValue();
            void appendString(std::ostringstream& oss, std::size_t margin) const override;
            inline sectionType_t type() const { return sectionType_t::AMQP_VALUE; };
        protected:
            PrimitiveType* _valuePtr;
        };




        class AmqpFooter: public Section {
        public:
            AmqpFooter(std::size_t frameOffset, AmqpAnnotations* annotationsPtr);
            virtual ~AmqpFooter();
            void appendString(std::ostringstream& oss, std::size_t margin) const override;
            inline sectionType_t type() const { return sectionType_t::FOOTER; };
        protected:
            AmqpAnnotations* _annotationsPtr;
        };


    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQP10_SECTION_HPP_ */
