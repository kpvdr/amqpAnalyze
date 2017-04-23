/*
 * FrameHeader.hpp
 *
 *  Created on: Apr 3, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_AMQP10_PERFORMATIVE_HPP_
#define SRC_AMQPANALYZE_AMQP10_PERFORMATIVE_HPP_

#include <amqpAnalyze/amqp10/AmqpBlock.hpp>
//#include <amqpAnalyze/amqp10/Frame.hpp>
//#include <cstdint>
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
        class Performative: public AmqpBlock {
        public:
            Performative(uint64_t packetNum, std::size_t dataOffset, AmqpList* fieldListPtr);
            virtual ~Performative();

            std::ostringstream& appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin, bool colorFlag) const override;
            inline const AmqpList* fieldList() const { return _fieldListPtr; }
            virtual const fieldTypeList_t& fieldTypeList() const = 0;
            virtual performativeType_t type() const = 0;
            inline const char* typeStr() const { return s_performativeTypeName.at(type()); }
            void validate() override;

        protected:
            AmqpList* _fieldListPtr;

            static std::map<performativeType_t, const char*> s_performativeTypeName;
        };


        class AmqpOpen: public Performative {
        public:
            AmqpOpen(uint64_t packetNum, std::size_t dataOffset, AmqpList* fieldListPtr);
            virtual ~AmqpOpen();
            inline const fieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; };
            inline performativeType_t type() const override { return performativeType_t::OPEN; }
            void validate() override;
            static const fieldTypeList_t s_fieldTypeList;
        };


        class AmqpBegin: public Performative {
        public:
            AmqpBegin(uint64_t packetNum, std::size_t dataOffset, AmqpList* fieldListPtr);
            virtual ~AmqpBegin();
            inline const fieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; };
            inline performativeType_t type() const override { return performativeType_t::BEGIN; }
            void validate() override;
            static const fieldTypeList_t s_fieldTypeList;
        };


        class AmqpAttach: public Performative {
        public:
            AmqpAttach(uint64_t packetNum, std::size_t dataOffset, AmqpList* fieldListPtr);
            virtual ~AmqpAttach();
            inline const fieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; };
            inline performativeType_t type() const override { return performativeType_t::ATTACH; }
            void validate() override;
            static const fieldTypeList_t s_fieldTypeList;
        };


        class AmqpFlow: public Performative {
        public:
            AmqpFlow(uint64_t packetNum, std::size_t dataOffset, AmqpList* fieldListPtr);
            virtual ~AmqpFlow();
            inline const fieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; };
            inline performativeType_t type() const override { return performativeType_t::FLOW; }
            void validate() override;
            static const fieldTypeList_t s_fieldTypeList;
        };


        class AmqpTransfer: public Performative {
        public:
            AmqpTransfer(uint64_t packetNum, std::size_t dataOffset, AmqpList* fieldListPtr);
            virtual ~AmqpTransfer();
            inline const fieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; };
            inline performativeType_t type() const override { return performativeType_t::TRANSFER; }
            void validate() override;
            static const fieldTypeList_t s_fieldTypeList;
        };


        class AmqpDisposition: public Performative {
        public:
            AmqpDisposition(uint64_t packetNum, std::size_t dataOffset, AmqpList* fieldListPtr);
            virtual ~AmqpDisposition();
            inline const fieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; };
            inline performativeType_t type() const override { return performativeType_t::DISPOSITION; }
            void validate() override;
            static const fieldTypeList_t s_fieldTypeList;
        };


        class AmqpDetach: public Performative {
        public:
            AmqpDetach(uint64_t packetNum, std::size_t dataOffset, AmqpList* fieldListPtr);
            virtual ~AmqpDetach();
            inline const fieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; };
            inline performativeType_t type() const override { return performativeType_t::DETACH; }
            void validate() override;
            static const fieldTypeList_t s_fieldTypeList;
        };


        class AmqpEnd: public Performative {
        public:
            AmqpEnd(uint64_t packetNum, std::size_t dataOffset, AmqpList* fieldListPtr);
            virtual ~AmqpEnd();
            inline const fieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; };
            inline performativeType_t type() const override { return performativeType_t::END; }
            void validate() override;
            static const fieldTypeList_t s_fieldTypeList;
        };


        class AmqpClose: public Performative {
        public:
            AmqpClose(uint64_t packetNum, std::size_t dataOffset, AmqpList* fieldListPtr);
            virtual ~AmqpClose();
            inline const fieldTypeList_t& fieldTypeList() const override { return s_fieldTypeList; };
            inline performativeType_t type() const override { return performativeType_t::CLOSE; }
            void validate() override;
            static const fieldTypeList_t s_fieldTypeList;
        };


    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQP10_PERFORMATIVE_HPP_ */
