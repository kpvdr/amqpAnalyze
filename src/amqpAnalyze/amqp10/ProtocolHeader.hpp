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
       protected:
            struct hdr _hdr;
            static std::map<uint8_t, const char*> s_protocolIdName;
        public:
            ProtocolHeader(std::size_t frameOffset, const struct ProtocolHeader::hdr* hdrPtr);
            ~ProtocolHeader();
            void appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const;
            std::size_t frameSize() const;
        };

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQP10_PROTOCOLHEADER_HPP_ */
