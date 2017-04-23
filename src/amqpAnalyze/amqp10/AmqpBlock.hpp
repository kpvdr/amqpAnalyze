/*
 * AmqpBlock.hpp
 *
 *  Created on: Apr 18, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_AMQP10_AMQPBLOCK_HPP_
#define SRC_AMQPANALYZE_AMQP10_AMQPBLOCK_HPP_

#include <sstream>
#include <vector>

namespace amqpAnalyze
{
    class Error;
    typedef std::vector<const Error*> error_ptr_list_t;

    namespace amqp10
    {
        class FrameBuffer;

        class AmqpBlock
        {
        public:
            AmqpBlock(uint64_t packetNum, std::size_t dataOffset);
            virtual ~AmqpBlock();

            void addError(const amqpAnalyze::Error* errorPtr);
            virtual std::ostringstream& appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin, bool colorFlag) const = 0;
            virtual std::ostringstream& appendStringEpilog(std::ostringstream& oss, std::size_t margin, bool colorFlag) const;
            std::size_t dataOffset() const;
            const error_ptr_list_t errorPtrList() const;
            bool hasErrors() const;
            uint64_t packetNum() const;
            virtual void validate() = 0;

        protected:
            const uint64_t _packetNum;
            const std::size_t _dataOffset;
            error_ptr_list_t _errorPtrList;
        };

        // Function pointer to error handler AmqpBlock::addError
        typedef void (AmqpBlock::*addErrorFp)(const amqpAnalyze::Error*);

        typedef std::vector<AmqpBlock*> amqp_block_list_t;
        typedef amqp_block_list_t::iterator amqp_block_list_itr_t;
        typedef amqp_block_list_t::const_iterator amqp_block_list_citr_t;

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQP10_AMQPBLOCK_HPP_ */
