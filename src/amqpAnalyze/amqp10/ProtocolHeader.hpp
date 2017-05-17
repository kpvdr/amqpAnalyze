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

        struct AmqpVersion {
            uint8_t _major;
            uint8_t _minor;
            uint8_t _revision;
            AmqpVersion();
            AmqpVersion(uint8_t major, uint8_t minor, uint8_t revision);
            inline bool isEqual(const AmqpVersion& v) const { return _major == v._major && _minor == v._minor && _revision == v._revision; }
            inline bool isZero() const { return _major == 0 && _minor == 0 && _revision == 0; }
            void set(const AmqpVersion& v);
        };

        class ProtocolHeader: public AmqpBlock
        {
        public:
            struct hdr {
                uint32_t _magic;
                ProtocolId_t _protocolId;
                struct AmqpVersion _version;
                hdr(const struct hdr* hdrPtr);
            };

            ProtocolHeader(FrameBuffer10& frameBuffer);
            ~ProtocolHeader();

            std::ostringstream& appendString(std::ostringstream& oss, std::size_t margin, bool ignoreMargin) const override;
            inline AmqpBlockType_t blockType() const override { return AmqpBlockType_t::PROTOCOL_HEADER; }
            ProtocolId_t protocolId() const;
            void validate();
            inline const AmqpVersion& version() const { return _hdr._version; }
        protected:
            struct hdr _hdr;
            static std::map<ProtocolId_t, const char*> s_protocolIdName;
        };

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQP10_PROTOCOLHEADER_HPP_ */
