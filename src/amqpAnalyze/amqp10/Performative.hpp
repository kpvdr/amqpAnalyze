/*
 * FrameHeader.hpp
 *
 *  Created on: Apr 3, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_AMQP10_PERFORMATIVE_HPP_
#define SRC_AMQPANALYZE_AMQP10_PERFORMATIVE_HPP_

#include <amqpAnalyze/amqp10/FrameBase.hpp>

#include <cstdint>
#include <map>
#include <sstream>
#include <vector>

namespace amqpAnalyze
{
    namespace amqp10
    {
        class AmqpList;
        class FieldType;
        typedef std::vector<FieldType> fieldTypeList_t;
        class FrameBuffer;
        class Section;
        typedef std::vector<Section*> sectionPtrList_t;

        enum frameType_t:uint8_t {
            AMQP_FRAME = 0,
            SASL_FRAME
        };
        class FrameHeader: public FrameBase {
        public:
            FrameHeader(std::size_t frameOffset, uint32_t frameSize, uint8_t dataOffset, frameType_t type, uint16_t typeSpecific);
            virtual ~FrameHeader();
            virtual void appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const override;
            virtual std::size_t frameSize() const override;
            inline virtual const char* typeStr() const { return "heartbeat"; }
        protected:
            const uint32_t _frameSize;
            const uint8_t _dataOffset;
            const frameType_t _type;
            const uint16_t _typeSpecific;
            std::size_t _dataOffsetBytes;
            std::size_t _extendedHeaderSize;
            static std::map<frameType_t, const char*> s_frameTypeName;
        };


        enum class performativeType_t:uint64_t {
            OPEN=0x10,
            BEGIN,
            ATTACH,
            FLOW,
            TRANSFER,
            DISPOSITION,
            DETACH,
            END,
            CLOSE
        };
        class Performative: public FrameHeader {
        public:
            Performative(std::size_t frameOffset, uint32_t frameSize, uint8_t dataOffset, frameType_t type, uint16_t typeSpecific, AmqpList* fieldListPtr);
            virtual ~Performative();
            void appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const override;
            inline const AmqpList* fieldList() const { return _fieldListPtr; }
            std::size_t frameSize() const override;
            inline const sectionPtrList_t& sectionPtrList() const { return _sectionPtrList; }
            virtual performativeType_t type() const = 0;
            inline const char* typeStr() const { return s_performativeTypeName.at(type()); }

            static FrameHeader* decode(std::size_t frameOffset, FrameBuffer& frameBuffer);
        protected:
            AmqpList* _fieldListPtr;
            sectionPtrList_t _sectionPtrList;

            static Performative* decodeAmqpFrame(std::size_t frameOffset,
                                                 uint32_t frameSize,
                                                 uint8_t dataOffset,
                                                 frameType_t type,
                                                 uint16_t typeSpecific,
                                                 FrameBuffer& frameBuffer);
            static Performative* decodeSaslFrame(std::size_t frameOffset,
                                                 uint32_t frameSize,
                                                 uint8_t dataOffset,
                                                 frameType_t type,
                                                 uint16_t typeSpecific,
                                                 FrameBuffer& frameBuffer);
            static std::map<performativeType_t, const char*> s_performativeTypeName;
        };


        class AmqpOpen: public Performative {
        public:
            AmqpOpen(std::size_t frameOffset, uint32_t frameSize, uint8_t dataOffset, frameType_t type, uint16_t typeSpecific, AmqpList* fieldListPtr);
            virtual ~AmqpOpen();
            inline performativeType_t type() const override { return performativeType_t::OPEN; }
        protected:
            static fieldTypeList_t s_fieldTypeList;
        };


        class AmqpBegin: public Performative {
        public:
            AmqpBegin(std::size_t frameOffset, uint32_t frameSize, uint8_t dataOffset, frameType_t type, uint16_t typeSpecific, AmqpList* fieldListPtr);
            virtual ~AmqpBegin();
            inline performativeType_t type() const override { return performativeType_t::BEGIN; }
        protected:
            static fieldTypeList_t s_fieldTypeList;
        };


        class AmqpAttach: public Performative {
        public:
            AmqpAttach(std::size_t frameOffset, uint32_t frameSize, uint8_t dataOffset, frameType_t type, uint16_t typeSpecific, AmqpList* fieldListPtr);
            virtual ~AmqpAttach();
            inline performativeType_t type() const override { return performativeType_t::ATTACH; }
        protected:
            static fieldTypeList_t s_fieldTypeList;
        };


        class AmqpFlow: public Performative {
        public:
            AmqpFlow(std::size_t frameOffset, uint32_t frameSize, uint8_t dataOffset, frameType_t type, uint16_t typeSpecific, AmqpList* fieldListPtr);
            virtual ~AmqpFlow();
            inline performativeType_t type() const override { return performativeType_t::FLOW; }
        protected:
            static fieldTypeList_t s_fieldTypeList;
        };


        class AmqpTransfer: public Performative {
        public:
            AmqpTransfer(std::size_t frameOffset, uint32_t frameSize, uint8_t dataOffset, frameType_t type, uint16_t typeSpecific, AmqpList* fieldListPtr);
            virtual ~AmqpTransfer();
            inline performativeType_t type() const override { return performativeType_t::TRANSFER; }
        protected:
            static fieldTypeList_t s_fieldTypeList;
        };


        class AmqpDisposition: public Performative {
        public:
            AmqpDisposition(std::size_t frameOffset, uint32_t frameSize, uint8_t dataOffset, frameType_t type, uint16_t typeSpecific, AmqpList* fieldListPtr);
            virtual ~AmqpDisposition();
            inline performativeType_t type() const override { return performativeType_t::DISPOSITION; }
        protected:
            static fieldTypeList_t s_fieldTypeList;
        };


        class AmqpDetach: public Performative {
        public:
            AmqpDetach(std::size_t frameOffset, uint32_t frameSize, uint8_t dataOffset, frameType_t type, uint16_t typeSpecific, AmqpList* fieldListPtr);
            virtual ~AmqpDetach();
            inline performativeType_t type() const override { return performativeType_t::DETACH; }
        protected:
            static fieldTypeList_t s_fieldTypeList;
        };


        class AmqpEnd: public Performative {
        public:
            AmqpEnd(std::size_t frameOffset, uint32_t frameSize, uint8_t dataOffset, frameType_t type, uint16_t typeSpecific, AmqpList* fieldListPtr);
            virtual ~AmqpEnd();
            inline performativeType_t type() const override { return performativeType_t::END; }
        protected:
            static fieldTypeList_t s_fieldTypeList;
        };


        class AmqpClose: public Performative {
        public:
            AmqpClose(std::size_t frameOffset, uint32_t frameSize, uint8_t dataOffset, frameType_t type, uint16_t typeSpecific, AmqpList* fieldListPtr);
            virtual ~AmqpClose();
            inline performativeType_t type() const override { return performativeType_t::CLOSE; }
        protected:
            static fieldTypeList_t s_fieldTypeList;
        };


    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQP10_PERFORMATIVE_HPP_ */
