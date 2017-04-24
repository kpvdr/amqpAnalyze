/*
 * ProtocolHeader.hpp
 *
 *  Created on: Apr 3, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_AMQP10_PROTOCOLHEADER_HPP_
#define SRC_AMQPANALYZE_AMQP10_PROTOCOLHEADER_HPP_

#include <amqpAnalyze/amqp10/AmqpBlock.hpp>

namespace amqpAnalyze
{
    namespace amqp10
    {

        enum class ProtocolId_t:uint8_t {
            AMQP = 0,
            TLS,
            SASL
        };

        class ProtocolHeader: public AmqpBlock
        {
        public:
            struct hdr {
                uint32_t _magic;
                ProtocolId_t _protocolId;
                uint8_t _major;
                uint8_t _minor;
                uint8_t _revision;
                hdr(const struct hdr* hdrPtr);
            };

            ProtocolHeader(FrameBuffer& frameBuffer);
            ~ProtocolHeader();

            std::ostringstream& appendString(std::ostringstream& oss, std::size_t margin, bool ignoreMargin, bool colorFlag) const override;
            ProtocolId_t protocolId() const;
            void validate();
        protected:
            struct hdr _hdr;
            static std::map<ProtocolId_t, const char*> s_protocolIdName;
        };

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQP10_PROTOCOLHEADER_HPP_ */
