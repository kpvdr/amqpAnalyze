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
    namespace amqp10
    {

        class AmqpBlock
        {
        public:
            AmqpBlock(std::size_t dataOffset);
            virtual ~AmqpBlock();

            virtual std::ostringstream& appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const = 0;
            std::size_t dataOffset() const;

        protected:
            const std::size_t _dataOffset;
        };

        typedef std::vector<AmqpBlock*> amqp_block_list_t;
        typedef amqp_block_list_t::iterator amqp_block_list_itr_t;
        typedef amqp_block_list_t::const_iterator amqp_block_list_citr_t;

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQP10_AMQPBLOCK_HPP_ */
