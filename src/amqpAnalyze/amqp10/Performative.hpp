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
            Performative(std::size_t dataOffset, AmqpList* fieldListPtr);
            virtual ~Performative();

            std::ostringstream& appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const override;
            inline const AmqpList* fieldList() const { return _fieldListPtr; }
            virtual fieldTypeList_t& fieldTypeList() const = 0;
            virtual performativeType_t type() const = 0;
            inline const char* typeStr() const { return s_performativeTypeName.at(type()); }

        protected:
            AmqpList* _fieldListPtr;

            static std::map<performativeType_t, const char*> s_performativeTypeName;
        };


        class AmqpOpen: public Performative {
        public:
            AmqpOpen(std::size_t dataOffset, AmqpList* fieldListPtr);
            virtual ~AmqpOpen();
            inline fieldTypeList_t& fieldTypeList() const { return s_fieldTypeList; };
            inline performativeType_t type() const override { return performativeType_t::OPEN; }
            static fieldTypeList_t s_fieldTypeList;
        };


        class AmqpBegin: public Performative {
        public:
            AmqpBegin(std::size_t dataOffset, AmqpList* fieldListPtr);
            virtual ~AmqpBegin();
            inline fieldTypeList_t& fieldTypeList() const { return s_fieldTypeList; };
            inline performativeType_t type() const override { return performativeType_t::BEGIN; }
            static fieldTypeList_t s_fieldTypeList;
        };


        class AmqpAttach: public Performative {
        public:
            AmqpAttach(std::size_t dataOffset, AmqpList* fieldListPtr);
            virtual ~AmqpAttach();
            inline fieldTypeList_t& fieldTypeList() const { return s_fieldTypeList; };
            inline performativeType_t type() const override { return performativeType_t::ATTACH; }
            static fieldTypeList_t s_fieldTypeList;
        };


        class AmqpFlow: public Performative {
        public:
            AmqpFlow(std::size_t dataOffset, AmqpList* fieldListPtr);
            virtual ~AmqpFlow();
            inline fieldTypeList_t& fieldTypeList() const { return s_fieldTypeList; };
            inline performativeType_t type() const override { return performativeType_t::FLOW; }
            static fieldTypeList_t s_fieldTypeList;
        };


        class AmqpTransfer: public Performative {
        public:
            AmqpTransfer(std::size_t dataOffset, AmqpList* fieldListPtr);
            virtual ~AmqpTransfer();
            inline fieldTypeList_t& fieldTypeList() const { return s_fieldTypeList; };
            inline performativeType_t type() const override { return performativeType_t::TRANSFER; }
            static fieldTypeList_t s_fieldTypeList;
        };


        class AmqpDisposition: public Performative {
        public:
            AmqpDisposition(std::size_t dataOffset, AmqpList* fieldListPtr);
            virtual ~AmqpDisposition();
            inline fieldTypeList_t& fieldTypeList() const { return s_fieldTypeList; };
            inline performativeType_t type() const override { return performativeType_t::DISPOSITION; }
            static fieldTypeList_t s_fieldTypeList;
        };


        class AmqpDetach: public Performative {
        public:
            AmqpDetach(std::size_t dataOffset, AmqpList* fieldListPtr);
            virtual ~AmqpDetach();
            inline fieldTypeList_t& fieldTypeList() const { return s_fieldTypeList; };
            inline performativeType_t type() const override { return performativeType_t::DETACH; }
            static fieldTypeList_t s_fieldTypeList;
        };


        class AmqpEnd: public Performative {
        public:
            AmqpEnd(std::size_t dataOffset, AmqpList* fieldListPtr);
            virtual ~AmqpEnd();
            inline fieldTypeList_t& fieldTypeList() const { return s_fieldTypeList; };
            inline performativeType_t type() const override { return performativeType_t::END; }
            static fieldTypeList_t s_fieldTypeList;
        };


        class AmqpClose: public Performative {
        public:
            AmqpClose(std::size_t dataOffset, AmqpList* fieldListPtr);
            virtual ~AmqpClose();
            inline fieldTypeList_t& fieldTypeList() const { return s_fieldTypeList; };
            inline performativeType_t type() const override { return performativeType_t::CLOSE; }
            static fieldTypeList_t s_fieldTypeList;
        };


    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQP10_PERFORMATIVE_HPP_ */
