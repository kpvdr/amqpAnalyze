/*
 * FrameHeader.hpp
 *
 *  Created on: Apr 3, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_AMQP10_FRAME_HPP_
#define SRC_AMQPANALYZE_AMQP10_FRAME_HPP_

#include <amqpAnalyze/amqp10/FrameBase.hpp>

#include <cstdint>
#include <map>
#include <sstream>

namespace amqpAnalyze
{
    namespace amqp10
    {

        class CompositeType;
        class Type;

        enum frameType_e:uint8_t {AMQP_FRAME = 0,
                                  SASL_FRAME};

        class Frame: public FrameBase
        {
        public:
            struct hdr {
                uint32_t _frameSize;
                uint8_t _dataOffset;
                frameType_e _type;
                uint16_t _typeSpecific;
                hdr(const hdr* hdrPtr);
            };
        protected:
            hdr _hdr;
            std::size_t _dataOffsetBytes;
            std::size_t _extendedHeaderSize;
            CompositeType* _performative;
            std::vector<Type*> _sectionList;
            static std::map<frameType_e, const char*> s_frameTypeName;
        public:
            Frame(std::size_t frameOffset, const struct Frame::hdr* hdrPtr);
            ~Frame();
            void appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const;
            std::size_t frameSize() const;
        };

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQP10_FRAME_HPP_ */
