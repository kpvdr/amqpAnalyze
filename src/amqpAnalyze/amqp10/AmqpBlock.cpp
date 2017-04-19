/*
 * AmqpBlock.cpp
 *
 *  Created on: Apr 18, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/amqp10/AmqpBlock.hpp>

namespace amqpAnalyze
{
    namespace amqp10
    {

        AmqpBlock::AmqpBlock(std::size_t dataOffset):
                    _dataOffset(dataOffset)
        {}

        AmqpBlock::~AmqpBlock() {}

        std::size_t AmqpBlock::dataOffset() const {
            return _dataOffset;
        }

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */
