/*
 * Buffer.hpp
 *
 *  Created on: Apr 5, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_AMQP10_BUFFER_HPP_
#define SRC_AMQPANALYZE_AMQP10_BUFFER_HPP_

#include <amqpAnalyze/amqp10/Type.hpp>
#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace amqpAnalyze
{
    namespace amqp10
    {

        class Buffer
        {
        protected:
            const char* _dataPtr;
            std::size_t _dataLength;
            std::size_t _dataOffset;
        public:
            Buffer(const char* dataPtr, std::size_t n);
            virtual ~Buffer();
            std::size_t getSize() const;
            std::size_t getOffset() const;
            std::size_t getRemaining() const;
            bool isEmpty() const;

            void ignore(std::size_t size);
            bool getBool();
            uint8_t getUint8();
            uint16_t getUint16();
            uint32_t getUint32();
            uint64_t getUint64();
            int8_t getInt8();
            int16_t getInt16();
            int32_t getInt32();
            int64_t getInt64();
            float getFloat();
            double getDouble();
            amqp_decimal32_t& getDecimal32(amqp_decimal32_t& value);
            amqp_decimal64_t& getDecimal64(amqp_decimal64_t& value);
            amqp_decimal128_t& getDecimal128(amqp_decimal128_t& value);
            char32_t getChar();
            amqp_uuid_t& getUuid(amqp_uuid_t& value);
            amqp_binary_t& getBinary(amqp_binary_t& value, std::size_t size);
            amqp_string_t& getString(amqp_string_t& value, std::size_t size);
            amqp_symbol_t& getSymbol(amqp_symbol_t& value, std::size_t size);
            amqp_list_t& getList(amqp_list_t& value, std::size_t size, std::size_t count);
            amqp_map_t& getMap(amqp_map_t& value, std::size_t size, std::size_t count);
            amqp_array_t& getArray(amqp_array_t& value, std::size_t size, std::size_t count);
        protected:
            void checkSize(std::size_t size);
        };

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQP10_BUFFER_HPP_ */
