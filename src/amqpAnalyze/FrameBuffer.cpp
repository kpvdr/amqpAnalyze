/*
 * Buffer.cpp
 *
 *  Created on: Apr 5, 2017
 *      Author: kpvdr
 */

#include "FrameBuffer.hpp"

//#include <amqpAnalyze/amqp10/Decoder.hpp>
#include <amqpAnalyze/DecodeError.hpp>
#include <amqpAnalyze/Options.hpp>
#include <cmath>
#include <iomanip>

namespace amqpAnalyze
{

    FrameBuffer::FrameBuffer(uint64_t packetNum, const uint8_t* dataPtr, std::size_t dataLength):
        _packetNum(packetNum),
        _dataPtr(dataPtr),
        _dataLength(dataLength),
        _dataOffset(0),
        _offsetSnapshotStack()
    {}

    FrameBuffer::~FrameBuffer() {
        for (std::vector<const amqpAnalyze::ColorDatum*>::iterator i=_colorList.begin(); i!=_colorList.end(); ++i) {
            delete *i;
        }
        _colorList.clear();
    }

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

    void FrameBuffer::addColorDatum(std::size_t offset, std::size_t len, amqpAnalyze::DisplayColorType_t colorType) {
        _colorList.push_back(new ColorDatum(offset, len, colorType));
    }

    void FrameBuffer::addColorDatum(const amqpAnalyze::ColorDatum* colorDatum) {
        _colorList.push_back(colorDatum);
    }

    std::string FrameBuffer::getHexDump() const {
        std::ostringstream oss;
        ColorList_Citr_t colorListCitrHexPanel = _colorList.cbegin();
        DisplayColorType_t currentColorHexPanel = DisplayColorType_t::RESET;
        ColorList_Citr_t colorListCitrCharPanel = _colorList.cbegin();
        DisplayColorType_t currentColorCharPanel = DisplayColorType_t::RESET;
        for (std::size_t i=0; i<std::ceil(_dataLength/16.0); ++i) {
            if (i > 0) oss << "\n";
            std::size_t numChars = _dataLength >= ((i+1)*16) ? 16 : _dataLength - (i*16);
            oss << "  " << std::hex << std::setw(4) << std::setfill('0') << (i*16) << "  ";
            hexDumpPreloop(oss, colorListCitrHexPanel, currentColorHexPanel);
            for (std::size_t c=0; c<16; ++c) {
                std::size_t currentIndex = (i*16) + c;
                oss << (c==8?"  ":" ");
                if (c < numChars) {
                    hexDumpPreChar(oss, colorListCitrHexPanel, currentColorHexPanel, currentIndex);
                    oss << std::setw(2) << (int)*(unsigned char*)(_dataPtr + currentIndex);
                    hexDumpPostChar(oss, colorListCitrHexPanel, currentColorHexPanel, currentIndex);
                } else {
                    oss << "  ";
                }
            }
            hexDumpPostloop(oss, colorListCitrHexPanel, currentColorHexPanel);
            oss << " |";
            hexDumpPreloop(oss, colorListCitrCharPanel, currentColorCharPanel);
            for (std::size_t c=0; c<16; ++c) {
                std::size_t currentIndex = (i*16) + c;
                char x = *(unsigned char*)(_dataPtr + currentIndex);
                oss << (c==8?" ":"");
                if (c < numChars) {
                    hexDumpPreChar(oss, colorListCitrCharPanel, currentColorCharPanel, currentIndex);
                    oss << (std::isprint(x) ? x : '.');
                    hexDumpPostChar(oss, colorListCitrCharPanel, currentColorCharPanel, currentIndex);
                } else {
                    oss << ' ';
                }
            }
            hexDumpPostloop(oss, colorListCitrCharPanel, currentColorCharPanel);
            oss << "|";
        }
        return oss.str();
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
        uint32_t x(be32toh(*(uint32_t*)(_dataPtr + _dataOffset)));
        _dataOffset += sizeof(float);
        return *(float*)&x;
   }

    double FrameBuffer::getDouble() {
        checkSize(sizeof(double), "getDouble");
        uint64_t x(be64toh(*(uint64_t*)(_dataPtr + _dataOffset)));
        _dataOffset += sizeof(double);
        return *(double*)&x;
    }


    // protected
    void FrameBuffer::checkSize(std::size_t size, const char* opName) {
        if (_dataLength - _dataOffset < size)
            throw DecodeError(*this, MSG("FrameBuffer." << opName << "(): Insufficient buffer data to extract 0x" << std::hex << size << " bytes: data_size=0x" << _dataLength << "; curr_offs=" << _dataOffset));
    }

    void FrameBuffer::hexDumpPostChar(std::ostringstream& oss, ColorList_Citr_t& colorListCitr, DisplayColorType_t& currentColor, std::size_t currentIndex) const {
        if (colorListCitr!=_colorList.end() && currentColor != DisplayColorType_t::RESET && ((*colorListCitr)->_offset + (*colorListCitr)->_len) == currentIndex+1) {
            currentColor = DisplayColorType_t::RESET;
            oss << Color::getColor(currentColor);
            ++colorListCitr;
        }
    }

    void FrameBuffer::hexDumpPostloop(std::ostringstream& oss, const ColorList_Citr_t& colorListCitr, DisplayColorType_t currentColor) const {
        if (colorListCitr!=_colorList.end() && currentColor != DisplayColorType_t::RESET) {
            oss << Color::getColor(DisplayColorType_t::RESET);
        }
    }

    void FrameBuffer::hexDumpPreChar(std::ostringstream& oss, const ColorList_Citr_t& colorListCitr, DisplayColorType_t& currentColor, std::size_t currentIndex) const {
        if (colorListCitr!=_colorList.end() && currentColor == DisplayColorType_t::RESET && (*colorListCitr)->_offset == currentIndex) {
            currentColor = (*colorListCitr)->_colorType;
            oss << Color::getColor(currentColor);
        }
    }

    void FrameBuffer::hexDumpPreloop(std::ostringstream& oss, const ColorList_Citr_t& colorListCitr, DisplayColorType_t currentColor) const {
        if (colorListCitr!=_colorList.end() && currentColor != DisplayColorType_t::RESET) {
            oss << Color::getColor(currentColor);
        }
    }

} /* namespace amqpAnalyze */
