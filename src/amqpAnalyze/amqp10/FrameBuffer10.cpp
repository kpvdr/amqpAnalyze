/*
 * FrameBuffer.cpp
 *
 *  Created on: May 17, 2017
 *      Author: kpvdr
 */

#include "FrameBuffer10.hpp"

#include <amqpAnalyze/amqp10/Decoder.hpp>
#include <amqpAnalyze/amqp10/Type.hpp>
#include <amqpAnalyze/DecodeError.hpp>


namespace amqpAnalyze
{
    namespace amqp10
    {

        FrameBuffer10::FrameBuffer10(uint64_t packetNum, const uint8_t* dataPtr, std::size_t dataLength):
            amqpAnalyze::FrameBuffer(packetNum, dataPtr, dataLength)
        {}

        FrameBuffer10::~FrameBuffer10() {}

        AmqpDecimal32_t& FrameBuffer10::getDecimal32(AmqpDecimal32_t& value) {
            checkSize(4 * sizeof(uint8_t), "getDecimal32");
            for (std::size_t i=0; i<4; ++i) {
                value[i] = *(uint8_t*)(_dataPtr + _dataOffset);
                _dataOffset += sizeof(uint8_t);
            }
            return value;
        }

        AmqpDecimal64_t& FrameBuffer10::getDecimal64(AmqpDecimal64_t& value) {
            checkSize(8 * sizeof(uint8_t), "getDecimal64");
            for (std::size_t i=0; i<8; ++i) {
                value[i] = *(uint8_t*)(_dataPtr + _dataOffset);
                _dataOffset += sizeof(uint8_t);
            }
            return value;
        }

        AmqpDecimal128_t& FrameBuffer10::getDecimal128(AmqpDecimal128_t& value) {
            checkSize(16 * sizeof(uint8_t), "getDecimal128");
            for (std::size_t i=0; i<16; ++i) {
                value[i] = *(uint8_t*)(_dataPtr + _dataOffset);
                _dataOffset += sizeof(uint8_t);
            }
            return value;
        }

        char32_t FrameBuffer10::getChar() {
            checkSize(sizeof(char32_t), "getChar");
            char32_t c(be32toh(*(char32_t*)(_dataPtr + _dataOffset)));
            _dataOffset += sizeof(char32_t);
            return c;
        }

        AmqpUuid_t& FrameBuffer10::getUuid(AmqpUuid_t& value) {
            checkSize(16 * sizeof(uint8_t), "getUuid");
            for (std::size_t i=0; i<16; ++i) {
                value[i] = *(uint8_t*)(_dataPtr + _dataOffset);
                _dataOffset += sizeof(uint8_t);
            }
            return value;
        }

        AmqpBinary_t& FrameBuffer10::getBinary(AmqpBinary_t& value, std::size_t size) {
            checkSize(size, "getBinary");
            for (std::size_t i=0; i<size; ++i) {
                value.push_back(*(uint8_t*)(_dataPtr + _dataOffset + i));
            }
            _dataOffset += size;
            return value;
        }

        std::string& FrameBuffer10::getString(std::string& value, std::size_t size) {
            checkSize(size, "getString");
            value.assign((const char*)(_dataPtr + _dataOffset), size);
            _dataOffset += size;
            return value;
        }

        std::string& FrameBuffer10::getSymbol(std::string& value, std::size_t size) {
            checkSize(size, "getSymbol");
            value.assign((const char*)(_dataPtr + _dataOffset), size);
            _dataOffset += size;
            return value;
        }

        AmqpList_t& FrameBuffer10::getList(AmqpList_t& value, std::size_t size, std::size_t count) {
            const size_t startOffs = _dataOffset;
            for (std::size_t i=0; i<count; ++i) {
                value.push_back((PrimitiveType*)Decoder::decode(*this));
            }
            if (_dataOffset - startOffs != size) {
                throw DecodeError(*this, MSG("FrameBuffer::getList(): List size mismatch: expected 0x" << std::hex << size << ", found 0x" << (_dataOffset - startOffs)));
            }
            return value;
        }

        AmqpMap_t& FrameBuffer10::getMap(AmqpMap_t& value, std::size_t size, std::size_t count) {
            const size_t startOffs = _dataOffset;
            for (std::size_t i=0; i<count; i+=2) {
                PrimitiveType* key = (PrimitiveType*)Decoder::decode(*this);
                PrimitiveType* mapVal = (PrimitiveType*)Decoder::decode(*this);
                value[key] = mapVal;
            }
            if (_dataOffset - startOffs != size) {
                throw DecodeError(*this, MSG("FrameBuffer::getMap(): Map size mismatch: expected 0x" << std::hex << size << ", found 0x" << (_dataOffset - startOffs)));
            }
            return value;
        }

        AmqpArray_t& FrameBuffer10::getArray(AmqpArray_t& value, std::size_t size, std::size_t count) {
            const size_t startOffs = _dataOffset;
            uint8_t ctor = getUint8();
            for (std::size_t i=0; i<count; ++i) {
                value.push_back((PrimitiveType*)Decoder::decodePrimitive(ctor, *this));
            }
            if (_dataOffset - startOffs != size) {
                throw DecodeError(*this, MSG("FrameBuffer::getArray(): Array size mismatch: expected 0x" << std::hex << size << ", found 0x" << (_dataOffset - startOffs)));
            }
           return value;
        }

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */
