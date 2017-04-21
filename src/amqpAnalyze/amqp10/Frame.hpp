/*
 * Frame.hpp
 *
 *  Created on: Apr 3, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_AMQP10_FRAME_HPP_
#define SRC_AMQPANALYZE_AMQP10_FRAME_HPP_

#include <amqpAnalyze/amqp10/AmqpBlock.hpp>
#include <amqpAnalyze/amqp10/Type.hpp>
#include <cstdint>
#include <map>
#include <std/AnsiTermColors.hpp>
#include <string>

namespace amqpAnalyze
{
    namespace amqp10
    {

        class FrameBuffer;
        class Performative;

        enum frameType_t:uint8_t {
            AMQP_FRAME = 0,
            SASL_FRAME
        };

        class Frame: public AmqpBlock
        {
        public:
            struct hdr {
                const uint32_t _frameSize;
                const uint8_t _doff;
                const frameType_t _type;
                const uint16_t _typeSpecific;
                hdr(const struct hdr* hdrPtr);
            };
            Frame(FrameBuffer& frameBuffer);
            virtual ~Frame();

            std::ostringstream& appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const override;
            uint8_t doff() const;
            uint32_t doffBytes() const;
            const std::string& extendedHeader() const;
            std::size_t extendedHeaderSize() const;
            uint32_t frameSize() const;
            frameType_t frameType() const;
            const amqp_block_list_t& sectionPtrList() const;
            uint16_t typeSpecific() const;
            void validate() override;

        protected:
            const struct hdr _hdr;
            const std::size_t _extendedHeaderSize;
            std::string _extendedHeader;
            AmqpBlock* _performative;
            amqp_block_list_t _sectionPtrList;
            static std::map<frameType_t, const char*> s_frameTypeName;
        };

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQP10_FRAME_HPP_ */
