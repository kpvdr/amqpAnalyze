/*
 * Buffer.cpp
 *
 *  Created on: Apr 5, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/amqp10/Buffer.hpp>

#include <amqpAnalyze/Error.hpp>

#include <endian.h>

// debug
//#include <iostream>

namespace amqpAnalyze
{
    namespace amqp10
    {

        Buffer::Buffer(const char* dataPtr, std::size_t n):
                _dataPtr(dataPtr),
                _dataLength(n),
                _dataOffset(0)
        {}

        Buffer::~Buffer() {}

        std::size_t Buffer::Buffer::getSize() const {
            return _dataLength;
        }

        std::size_t Buffer::getOffset() const {
            return _dataOffset;
        }

        void Buffer::ignore(std::size_t size) {
            checkSize(size);
            _dataOffset += size;
        }

        bool Buffer::getBool() {
            checkSize(sizeof(uint8_t));
            uint8_t d(*(uint8_t*)(_dataPtr + _dataOffset));
            _dataOffset += sizeof(uint8_t);
            if (d == 0) return false;
            if (d == 1) return true;
            throw amqpAnalyze::Error(MSG("Illegal boolean value 0x") << std::hex << d);
        }

        uint8_t Buffer::getUint8() {
            checkSize(sizeof(uint8_t));
            uint8_t d(*(uint8_t*)(_dataPtr + _dataOffset));
            _dataOffset += sizeof(uint8_t);
            return d;
        }

        uint16_t Buffer::getUint16() {
            checkSize(sizeof(uint16_t));
            uint16_t d(be16toh(*(uint16_t*)(_dataPtr + _dataOffset)));
            _dataOffset += sizeof(uint16_t);
            return d;
        }

        uint32_t Buffer::getUint32() {
            checkSize(sizeof(uint32_t));
            uint32_t d(be32toh(*(uint32_t*)(_dataPtr + _dataOffset)));
            _dataOffset += sizeof(uint32_t);
            return d;
        }

        uint64_t Buffer::getUint64() {
            checkSize(sizeof(uint64_t));
            uint64_t d(be64toh(*(uint64_t*)(_dataPtr + _dataOffset)));
            _dataOffset += sizeof(uint64_t);
            return d;
        }

        int8_t Buffer::getInt8() {
            checkSize(sizeof(int8_t));
            int8_t d(*(int8_t*)(_dataPtr + _dataOffset));
            _dataOffset += sizeof(int8_t);
            return d;
        }

        int16_t Buffer::getInt16() {
            checkSize(sizeof(int16_t));
            int16_t d(be16toh(*(int16_t*)(_dataPtr + _dataOffset)));
            _dataOffset += sizeof(int16_t);
            return d;
        }

        int32_t Buffer::getInt32() {
            checkSize(sizeof(int32_t));
            int32_t d(be32toh(*(int32_t*)(_dataPtr + _dataOffset)));
            _dataOffset += sizeof(int32_t);
            return d;
        }

        int64_t Buffer::getInt64() {
            checkSize(sizeof(int64_t));
            int64_t d(be64toh(*(int64_t*)(_dataPtr + _dataOffset)));
            _dataOffset += sizeof(int64_t);
            return d;
        }

        float Buffer::getFloat() {
            checkSize(sizeof(float));
            float d(be32toh(*(float*)(_dataPtr + _dataOffset)));
            _dataOffset += sizeof(float);
            return d;
       }

        double Buffer::getDouble() {
            checkSize(sizeof(double));
            double d(be64toh(*(double*)(_dataPtr + _dataOffset)));
            _dataOffset += sizeof(double);
            return d;
        }

        amqp_decimal32_t& Buffer::getDecimal32(amqp_decimal32_t& value) {
            checkSize(sizeof(4));
            for (std::size_t i=0; i<4; ++i) {
                value[i] = *(uint8_t*)(_dataPtr + _dataOffset);
                _dataOffset += sizeof(uint8_t);
            }
            return value;
        }

        amqp_decimal64_t& Buffer::getDecimal64(amqp_decimal64_t& value) {
            checkSize(sizeof(8));
            for (std::size_t i=0; i<8; ++i) {
                value[i] = *(uint8_t*)(_dataPtr + _dataOffset);
                _dataOffset += sizeof(uint8_t);
            }
            return value;
        }

        amqp_decimal128_t& Buffer::getDecimal128(amqp_decimal128_t& value) {
            checkSize(sizeof(16));
            for (std::size_t i=0; i<16; ++i) {
                value[i] = *(uint8_t*)(_dataPtr + _dataOffset);
                _dataOffset += sizeof(uint8_t);
            }
            return value;
        }

        char32_t Buffer::getChar() {
            checkSize(sizeof(char32_t));
            char32_t c(be32toh(*(char32_t*)(_dataPtr + _dataOffset)));
            _dataOffset += sizeof(char32_t);
            return c;
        }

        amqp_uuid_t& Buffer::getUuid(amqp_uuid_t& value) {
            checkSize(sizeof(16));
            for (std::size_t i=0; i<16; ++i) {
                value[i] = *(uint8_t*)(_dataPtr + _dataOffset);
                _dataOffset += sizeof(uint8_t);
            }
            return value;
        }

        amqp_binary_t& Buffer::getBinary(amqp_binary_t& value, std::size_t size) {
            checkSize(size);
            for (std::size_t i=0; i<size; ++i) {
                value.push_back(*(uint8_t*)(_dataPtr + _dataOffset + i));
            }
            _dataOffset += size;
            return value;
        }

        amqp_string_t& Buffer::getString(amqp_string_t& value, std::size_t size) {
            checkSize(sizeof(size));
            value.assign((const char*)(_dataPtr + _dataOffset), size);
            _dataOffset += size;
            return value;
        }

        amqp_symbol_t& Buffer::getSymbol(amqp_symbol_t& value, std::size_t size) {
            checkSize(sizeof(size));
            value.assign((const char*)(_dataPtr + _dataOffset), size);
            _dataOffset += size;
            return value;
        }

        amqp_list_t& Buffer::getList(amqp_list_t& value, std::size_t size, std::size_t count) {
            const size_t startOffs = _dataOffset;
            //std::cout << std::dec << "*** getList(size=" << size << ", count=" << count << "): soffs=" << startOffs << std::endl;
            for (std::size_t i=0; i<count; ++i) {
                //std::cout << "  > " << i << " " << std::flush;
                //AmqpType* t =  AmqpType::decode(*this);
                //std::cout << t << " coffs=" << _dataOffset << std::endl;
                //value.push_back(t);
                value.push_back((PrimitiveType*)Type::decode(*this));
            }
            if (_dataOffset - startOffs != size) {
                throw amqpAnalyze::Error(MSG("List size mismatch: expected 0x" << std::hex << size << ", found 0x" << (_dataOffset - startOffs)));
            }
            return value;
        }

        amqp_map_t& Buffer::getMap(amqp_map_t& value, std::size_t size, std::size_t count) {
            const size_t startOffs = _dataOffset;
            //std::cout << std::dec << "*** getMap(size=" << size << ", count=" << count << "): soffs=" << startOffs << std::endl;
            for (std::size_t i=0; i<count; i+=2) {
                //std::cout << "  > " << i << " " << std::flush;
                PrimitiveType* key = (PrimitiveType*)Type::decode(*this);
                PrimitiveType* mapVal = (PrimitiveType*)Type::decode(*this);
                value[key] = mapVal;
                //std::cout << " " << key << ": " << mapVal << " coffs=" << _dataOffset << std::flush;
            }
            if (_dataOffset - startOffs != size) {
                throw amqpAnalyze::Error(MSG("Map size mismatch: expected 0x" << std::hex << size << ", found 0x" << (_dataOffset - startOffs)));
            }
            return value;
        }

        amqp_array_t& Buffer::getArray(amqp_array_t& value, std::size_t size, std::size_t count) {
            const size_t startOffs = _dataOffset;
            //std::cout << std::dec << "*** getArray(size=" << size << ", count=" << count << "): soffs=" << startOffs << std::endl;
            uint8_t ctor = getUint8();
            for (std::size_t i=0; i<count; ++i) {
                //std::cout << "  > " << i << " " << std::flush;
                //AmqpType* t = AmqpType::decode(ctor, *this);
                //std::cout << t << " coffs=" << _dataOffset << std::endl;
                //value.push_back(t);
                value.push_back((PrimitiveType*)Type::decodePrimitive(ctor, *this));
            }
            if (_dataOffset - startOffs != size) {
                throw amqpAnalyze::Error(MSG("Array size mismatch: expected 0x" << std::hex << size << ", found 0x" << (_dataOffset - startOffs)));
            }
           return value;
        }

        // protected
        void Buffer::checkSize(std::size_t size) {
            if (_dataLength - _dataOffset < size)
                throw amqpAnalyze::Error(MSG("Insufficient buffer data to extract " << size << " bytes: data_size=" << _dataLength << "; curr_offs=" << _dataOffset));
        }

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */
