/*
 * AmqpHeader.hpp
 *
 *  Created on: Apr 3, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_AMQP10_PROTOCOLHEADER_HPP_
#define SRC_AMQPANALYZE_AMQP10_PROTOCOLHEADER_HPP_

#include <amqpAnalyze/amqp10/FrameBase.hpp>

#include <cstdint>
#include <map>
#include <sstream>

namespace amqpAnalyze
{
    namespace amqp10
    {

        class FrameBuffer;

        class ProtocolHeader: public FrameBase
        {
        public:
            struct hdr {
                uint32_t _magic;
                uint8_t _protocolId;
                uint8_t _major;
                uint8_t _minor;
                uint8_t _revision;
                hdr(const struct hdr* hdrPtr);
            };
        public:
            ProtocolHeader(FrameBuffer& frameBuffer);
            ~ProtocolHeader();
            void appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const;
            std::size_t frameSize() const;
        protected:
             struct hdr _hdr;
             static std::map<uint8_t, const char*> s_protocolIdName;
        };

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQP10_PROTOCOLHEADER_HPP_ */
