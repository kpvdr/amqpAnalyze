/*
 * Buffer.cpp
 *
 *  Created on: Apr 5, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/amqp10/FrameBuffer.hpp>

#include <amqpAnalyze/amqp10/Decoder.hpp>
#include <amqpAnalyze/Error.hpp>

#include <endian.h>
#include <iomanip>
#include <sstream>

namespace amqpAnalyze
{
    namespace amqp10
    {

        FrameBuffer::FrameBuffer(uint64_t packetNum, const uint8_t* dataPtr, std::size_t dataLength):
            _packetNum(packetNum),
            _dataPtr(dataPtr),
            _dataLength(dataLength),
            _dataOffset(0),
            _offsetSnapshotStack()
        {}

        FrameBuffer::~FrameBuffer() {}

        bool FrameBuffer::empty() const {
            return _dataLength == _dataOffset;
        }

        std::size_t FrameBuffer::FrameBuffer::getSize() const {
            return _dataLength;
        }

        std::size_t FrameBuffer::getOffset() const {
            return _dataOffset;
        }

        std::size_t FrameBuffer::getRemaining() const {
            return _dataLength - _dataOffset;
        }

        const uint8_t* FrameBuffer::getDataPtr() const {
            return _dataPtr + _dataOffset;
        }

        std::size_t FrameBuffer::getFrameOffsetSnapshot() const {
            return _offsetSnapshotStack.back();
        }

        uint64_t FrameBuffer::getPacketNum() const {
            return _packetNum;
        }
        std::size_t FrameBuffer::popFrameOffsetSnapshot() {
            if (_offsetSnapshotStack.empty()) {
                throw amqpAnalyze::Error(MSG("FrameBuffer::popFrameOffsetSnapshot(): Stack empty"));
            }
            std::size_t offs(_offsetSnapshotStack.back());
            _offsetSnapshotStack.pop_back();
            return offs;
        }

        std::size_t FrameBuffer::pushFrameOffsetSnapshot() {
            _offsetSnapshotStack.push_back(_dataOffset);
            return _dataOffset;
        }

        void FrameBuffer::ignore(std::size_t size) {
            checkSize(size, "ignore");
            _dataOffset += size;
        }

        uint8_t* FrameBuffer::getStructPtr(std::size_t size) {
            checkSize(size, "getStructPtr");
            uint8_t* ptr = (uint8_t*)(_dataPtr + _dataOffset);
            _dataOffset += size;
            return ptr;
        }

        bool FrameBuffer::getBool() {
            checkSize(sizeof(uint8_t), "getBool");
            uint8_t d(*(uint8_t*)(_dataPtr + _dataOffset));
            _dataOffset += sizeof(uint8_t);
            if (d == 0) return false;
            if (d == 1) return true;
            throw amqpAnalyze::Error(MSG("Illegal boolean value 0x") << std::hex << d);
        }

        uint8_t FrameBuffer::getUint8() {
            checkSize(sizeof(uint8_t), "getUint8");
            uint8_t d(*(uint8_t*)(_dataPtr + _dataOffset));
            _dataOffset += sizeof(uint8_t);
            return d;
        }

        uint16_t FrameBuffer::getUint16() {
            checkSize(sizeof(uint16_t), "getUint16");
            uint16_t d(be16toh(*(uint16_t*)(_dataPtr + _dataOffset)));
            _dataOffset += sizeof(uint16_t);
            return d;
        }

        uint32_t FrameBuffer::getUint32() {
            checkSize(sizeof(uint32_t), "getUint32");
            uint32_t d(be32toh(*(uint32_t*)(_dataPtr + _dataOffset)));
            _dataOffset += sizeof(uint32_t);
            return d;
        }

        uint64_t FrameBuffer::getUint64() {
            checkSize(sizeof(uint64_t), "getUint64");
            uint64_t d(be64toh(*(uint64_t*)(_dataPtr + _dataOffset)));
            _dataOffset += sizeof(uint64_t);
            return d;
        }

        int8_t FrameBuffer::getInt8() {
            checkSize(sizeof(int8_t), "getInt8");
            int8_t d(*(int8_t*)(_dataPtr + _dataOffset));
            _dataOffset += sizeof(int8_t);
            return d;
        }

        int16_t FrameBuffer::getInt16() {
            checkSize(sizeof(int16_t), "getInt16");
            int16_t d(be16toh(*(int16_t*)(_dataPtr + _dataOffset)));
            _dataOffset += sizeof(int16_t);
            return d;
        }

        int32_t FrameBuffer::getInt32() {
            checkSize(sizeof(int32_t), "getInt32");
            int32_t d(be32toh(*(int32_t*)(_dataPtr + _dataOffset)));
            _dataOffset += sizeof(int32_t);
            return d;
        }

        int64_t FrameBuffer::getInt64() {
            checkSize(sizeof(int64_t), "getInt64");
            int64_t d(be64toh(*(int64_t*)(_dataPtr + _dataOffset)));
            _dataOffset += sizeof(int64_t);
            return d;
        }

        float FrameBuffer::getFloat() {
            checkSize(sizeof(float), "getFloat");
            float d(be32toh(*(float*)(_dataPtr + _dataOffset)));
            _dataOffset += sizeof(float);
            return d;
       }

        double FrameBuffer::getDouble() {
            checkSize(sizeof(double), "getDouble");
            double d(be64toh(*(double*)(_dataPtr + _dataOffset)));
            _dataOffset += sizeof(double);
            return d;
        }

        amqp_decimal32_t& FrameBuffer::getDecimal32(amqp_decimal32_t& value) {
            checkSize(4 * sizeof(uint8_t), "getDecimal32");
            for (std::size_t i=0; i<4; ++i) {
                value[i] = *(uint8_t*)(_dataPtr + _dataOffset);
                _dataOffset += sizeof(uint8_t);
            }
            return value;
        }

        amqp_decimal64_t& FrameBuffer::getDecimal64(amqp_decimal64_t& value) {
            checkSize(8 * sizeof(uint8_t), "getDecimal64");
            for (std::size_t i=0; i<8; ++i) {
                value[i] = *(uint8_t*)(_dataPtr + _dataOffset);
                _dataOffset += sizeof(uint8_t);
            }
            return value;
        }

        amqp_decimal128_t& FrameBuffer::getDecimal128(amqp_decimal128_t& value) {
            checkSize(16 * sizeof(uint8_t), "getDecimal128");
            for (std::size_t i=0; i<16; ++i) {
                value[i] = *(uint8_t*)(_dataPtr + _dataOffset);
                _dataOffset += sizeof(uint8_t);
            }
            return value;
        }

        char32_t FrameBuffer::getChar() {
            checkSize(sizeof(char32_t), "getChar");
            char32_t c(be32toh(*(char32_t*)(_dataPtr + _dataOffset)));
            _dataOffset += sizeof(char32_t);
            return c;
        }

        amqp_uuid_t& FrameBuffer::getUuid(amqp_uuid_t& value) {
            checkSize(16 * sizeof(uint8_t), "getUuid");
            for (std::size_t i=0; i<16; ++i) {
                value[i] = *(uint8_t*)(_dataPtr + _dataOffset);
                _dataOffset += sizeof(uint8_t);
            }
            return value;
        }

        amqp_binary_t& FrameBuffer::getBinary(amqp_binary_t& value, std::size_t size) {
            checkSize(size, "getBinary");
            for (std::size_t i=0; i<size; ++i) {
                value.push_back(*(uint8_t*)(_dataPtr + _dataOffset + i));
            }
            _dataOffset += size;
            return value;
        }

        amqp_string_t& FrameBuffer::getString(amqp_string_t& value, std::size_t size) {
            checkSize(size, "getString");
            value.assign((const char*)(_dataPtr + _dataOffset), size);
            _dataOffset += size;
            return value;
        }

        amqp_symbol_t& FrameBuffer::getSymbol(amqp_symbol_t& value, std::size_t size) {
            checkSize(size, "getSymbol");
            value.assign((const char*)(_dataPtr + _dataOffset), size);
            _dataOffset += size;
            return value;
        }

        amqp_list_t& FrameBuffer::getList(amqp_list_t& value, std::size_t size, std::size_t count) {
            const size_t startOffs = _dataOffset;
            for (std::size_t i=0; i<count; ++i) {
                value.push_back((PrimitiveType*)Decoder::decode(*this));
            }
            if (_dataOffset - startOffs != size) {
                throw amqpAnalyze::AmqpDecodeError(*this, MSG("FrameBuffer::getList(): List size mismatch: expected 0x" << std::hex << size << ", found 0x" << (_dataOffset - startOffs)));
            }
            return value;
        }

        amqp_map_t& FrameBuffer::getMap(amqp_map_t& value, std::size_t size, std::size_t count) {
            const size_t startOffs = _dataOffset;
            for (std::size_t i=0; i<count; i+=2) {
                PrimitiveType* key = (PrimitiveType*)Decoder::decode(*this);
                PrimitiveType* mapVal = (PrimitiveType*)Decoder::decode(*this);
                value[key] = mapVal;
            }
            if (_dataOffset - startOffs != size) {
                throw amqpAnalyze::AmqpDecodeError(*this, MSG("FrameBuffer::getMap(): Map size mismatch: expected 0x" << std::hex << size << ", found 0x" << (_dataOffset - startOffs)));
            }
            return value;
        }

        amqp_array_t& FrameBuffer::getArray(amqp_array_t& value, std::size_t size, std::size_t count) {
            const size_t startOffs = _dataOffset;
            uint8_t ctor = getUint8();
            for (std::size_t i=0; i<count; ++i) {
                value.push_back((PrimitiveType*)Decoder::decodePrimitive(ctor, *this));
            }
            if (_dataOffset - startOffs != size) {
                throw amqpAnalyze::AmqpDecodeError(*this, MSG("FrameBuffer::getArray(): Array size mismatch: expected 0x" << std::hex << size << ", found 0x" << (_dataOffset - startOffs)));
            }
           return value;
        }

        // protected
        void FrameBuffer::checkSize(std::size_t size, const char* opName) {
            if (_dataLength - _dataOffset < size)
                throw amqpAnalyze::AmqpDecodeError(*this, MSG("FrameBuffer." << opName << "(): Insufficient buffer data to extract 0x" << std::hex << size << " bytes: data_size=0x" << _dataLength << "; curr_offs=" << _dataOffset));
        }

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */
