/*
 * FrameBuffer.hpp
 *
 *  Created on: Apr 5, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_AMQP10_FRAMEBUFFER_HPP_
#define SRC_AMQPANALYZE_AMQP10_FRAMEBUFFER_HPP_

#include <amqpAnalyze/amqp10/Type.hpp>
#include <amqpAnalyze/Color.hpp>

namespace amqpAnalyze
{
    namespace amqp10
    {

        class FrameBuffer
        {
        public:
            FrameBuffer(uint64_t packetNum, const uint8_t* dataPtr, std::size_t dataLength);
            virtual ~FrameBuffer();

            bool empty() const;
            std::size_t getSize() const;
            std::size_t getOffset() const;
            std::size_t getRemaining() const;
            const uint8_t* getDataPtr() const;
            std::size_t getFrameOffsetSnapshot() const;
            uint64_t getPacketNum() const;
            std::size_t popFrameOffsetSnapshot();
            std::size_t pushFrameOffsetSnapshot();

            void addColorDatum(std::size_t offset, std::size_t len, amqpAnalyze::DisplayColorType_t colorType);
            void addColorDatum(const amqpAnalyze::ColorDatum* colorDatum);
            std::string getHexDump() const;

            void ignore(std::size_t size);
            uint8_t* getStructPtr(std::size_t size);
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

        protected:
            const uint64_t _packetNum;
            const uint8_t* _dataPtr;
            std::size_t _dataLength;
            std::size_t _dataOffset;
            std::vector<std::size_t> _offsetSnapshotStack;
            ColorList_t _colorList;

            void checkSize(std::size_t size, const char* opName);
            void hexDumpPostChar(std::ostringstream& oss, ColorList_Citr_t& colorListCitr, DisplayColorType_t& currentColor, std::size_t currentIndex) const;
            void hexDumpPostloop(std::ostringstream& oss, const ColorList_Citr_t& colorListCitr, DisplayColorType_t currentColor) const;
            void hexDumpPreChar(std::ostringstream& oss, const ColorList_Citr_t& colorListCitr, DisplayColorType_t& currentColor, std::size_t currentIndex) const;
            void hexDumpPreloop(std::ostringstream& oss, const ColorList_Citr_t& colorListCitr, DisplayColorType_t currentColor) const;
        };

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQP10_FRAMEBUFFER_HPP_ */
