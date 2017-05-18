/*
 * AmqpBlock.hpp
 *
 *  Created on: Apr 18, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_AMQP10_AMQPBLOCK_HPP_
#define SRC_AMQPANALYZE_AMQP10_AMQPBLOCK_HPP_

#include <amqpAnalyze/amqp10/FwdDecls.hpp>
#include <amqpAnalyze/FwdDecls.hpp>
#include <amqpAnalyze/Error.hpp>
#include <sstream>

namespace amqpAnalyze
{
    namespace amqp10
    {

        enum class AmqpBlockType_t {
            PROTOCOL_HEADER,
            FRAME,
            FRAME_ERROR,
            PERFORMATIVE,
            SECTION
        };

        class AmqpBlock
        {
        public:
            AmqpBlock(uint64_t packetNum, std::size_t dataOffset);
            virtual ~AmqpBlock();

            void addError(const amqpAnalyze::Error* errorPtr);
            void appendStateStr(const std::string& stateStr);
            virtual std::ostringstream& appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const = 0;
            virtual std::ostringstream& appendStringEpilog(std::ostringstream& oss, std::size_t margin) const;
            virtual AmqpBlockType_t blockType() const = 0;
            std::size_t dataOffset() const;
            const ErrorPtrList_t errorPtrList() const;
            bool hasErrors() const;
            const char* name() const;
            uint64_t packetNum() const;
            void setStateStr(const std::string& stateStr);
            const std::string& stateStr() const;
            virtual void validate() = 0;

        protected:
            const uint64_t _packetNum;
            const std::size_t _dataOffset;
            std::string _stateStr;
            ErrorPtrList_t _errorPtrList;
            static std::map<AmqpBlockType_t, const char*> s_AmqpBlockTypeNames;
        };

        // Function pointer to error handler AmqpBlock::addError
        //typedef void (AmqpBlock::*addErrorFp)(const amqpAnalyze::Error*);

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQP10_AMQPBLOCK_HPP_ */
