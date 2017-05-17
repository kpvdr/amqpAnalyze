/*
 * FrameBuffer.hpp
 *
 *  Created on: May 17, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_AMQP10_FRAMEBUFFER10_HPP_
#define SRC_AMQPANALYZE_AMQP10_FRAMEBUFFER10_HPP_

#include <amqpAnalyze/amqp10/FwdDecls.hpp>
#include <amqpAnalyze/FrameBuffer.hpp>

namespace amqpAnalyze
{
    namespace amqp10
    {

        class FrameBuffer10: public amqpAnalyze::FrameBuffer
        {
        public:
            FrameBuffer10(uint64_t packetNum, const uint8_t* dataPtr, std::size_t dataLength);
            virtual ~FrameBuffer10();

            // AMQP 1.0 specific type decode methods

            AmqpDecimal32_t& getDecimal32(AmqpDecimal32_t& value);
            AmqpDecimal64_t& getDecimal64(AmqpDecimal64_t& value);
            AmqpDecimal128_t& getDecimal128(AmqpDecimal128_t& value);
            char32_t getChar();
            AmqpUuid_t& getUuid(AmqpUuid_t& value);
            AmqpBinary_t& getBinary(AmqpBinary_t& value, std::size_t size);
            std::string& getString(std::string& value, std::size_t size);
            std::string& getSymbol(std::string& value, std::size_t size);
            AmqpList_t& getList(AmqpList_t& value, std::size_t size, std::size_t count);
            AmqpMap_t& getMap(AmqpMap_t& value, std::size_t size, std::size_t count);
            AmqpArray_t& getArray(AmqpArray_t& value, std::size_t size, std::size_t count);
        };

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQP10_FRAMEBUFFER10_HPP_ */
