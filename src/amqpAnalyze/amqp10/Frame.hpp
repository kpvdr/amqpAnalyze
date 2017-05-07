/*
 * Frame.hpp
 *
 *  Created on: Apr 3, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_AMQP10_FRAME_HPP_
#define SRC_AMQPANALYZE_AMQP10_FRAME_HPP_

#include <amqpAnalyze/amqp10/AmqpBlock.hpp>

namespace amqpAnalyze
{
    namespace amqp10
    {
        class FramePlaceholder;
        class Performative;

        enum class FrameType_t:uint8_t {
            AMQP_FRAME = 0,
            SASL_FRAME
        };

        class Frame: public AmqpBlock
        {
        public:
            struct hdr {
                const uint32_t _frameSize;
                const uint8_t _doff;
                const FrameType_t _type;
                const uint16_t _typeSpecific;
                hdr(const struct hdr* hdrPtr);
            };
            Frame(FrameBuffer& frameBuffer);
            virtual ~Frame();

            std::ostringstream& appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const override;
            inline AmqpBlockType_t blockType() const override { return AmqpBlockType_t::FRAME; }
            uint8_t doff() const;
            uint32_t doffBytes() const;
            const std::string& extendedHeader() const;
            std::size_t extendedHeaderSize() const;
            inline const FramePlaceholder* framePlaceholder() const { return _framePlaceholder; }
            uint32_t frameSize() const;
            FrameType_t frameType() const;
            inline Performative* performative() { return _performative; }
            inline const Performative* performative() const { return _performative; }
            inline const bool isEmpty() const { return _performative == nullptr && _framePlaceholder == nullptr; }
            const AmqpBlockList_t& sectionPtrList() const;
            uint16_t typeSpecific() const;
            void validate() override;

        protected:
            const struct hdr _hdr;
            const std::size_t _extendedHeaderSize;
            std::string _extendedHeader;
            Performative* _performative;
            FramePlaceholder* _framePlaceholder;
            AmqpBlockList_t _sectionPtrList;
            static std::map<FrameType_t, const char*> s_frameTypeName;
        };

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQP10_FRAME_HPP_ */
