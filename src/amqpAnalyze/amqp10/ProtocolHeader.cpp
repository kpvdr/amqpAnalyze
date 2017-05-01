/*
 * AmqpHeader.cpp
 *
 *  Created on: Apr 3, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/amqp10/ProtocolHeader.hpp>

#include <amqpAnalyze/amqp10/FrameBuffer.hpp>
#include <amqpAnalyze/Color.hpp>
#include <amqpAnalyze/Options.hpp>
#include <iomanip>
#include <netinet/in.h>

namespace amqpAnalyze
{
    namespace amqp10
    {

        AmqpVersion::AmqpVersion(): _major(0), _minor(0), _revision(0) {}

        AmqpVersion::AmqpVersion(uint8_t major, uint8_t minor, uint8_t revision):
            _major(major),
            _minor(minor),
            _revision(revision)
        {}

        void AmqpVersion::set(const AmqpVersion& v) {
            _major = v._major;
            _minor = v._minor;
            _revision = v._revision;
        }

        ProtocolHeader::hdr::hdr(const struct hdr* hdrPtr):
                _magic(::ntohl(hdrPtr->_magic)),
                _protocolId(hdrPtr->_protocolId),
                _version(hdrPtr->_version)
        {}

        ProtocolHeader::ProtocolHeader(FrameBuffer& frameBuffer):
                AmqpBlock(frameBuffer.getPacketNum(), frameBuffer.pushFrameOffsetSnapshot()),
                _hdr((hdr*)frameBuffer.getStructPtr(sizeof(hdr)))
        {
            frameBuffer.addColorDatum(_dataOffset, sizeof(hdr), DisplayColorType_t::AMQP_PROTOCOL_HEADER);
            frameBuffer.popFrameOffsetSnapshot();
        }

        ProtocolHeader::~ProtocolHeader() {}

        std::ostringstream& ProtocolHeader::appendString(std::ostringstream& oss, std::size_t margin, bool ignoreMargin) const {
            if (margin > 0 && !ignoreMargin) oss << "\n" << std::string(margin, ' ');
            oss << "[" << std::hex << std::setfill('0') << std::setw(4) << _dataOffset << "] h " << std::dec;
            oss << Color::color(DisplayColorType_t::AMQP_PROTOCOL_HEADER, MSG("AMQP header v" << (int)_hdr._version._major << "." << (int)_hdr._version._minor << "." << (int)_hdr._version._revision));
            oss << " pid=0x" << std::hex << (int)_hdr._protocolId << " (" << s_protocolIdName[_hdr._protocolId] << ")";
            if (g_optionsPtr->s_showStateFlag && !_stateStr.empty()) oss << " | " << Color::color(DisplayColorType_t::AMQP_STATE_CONNECTION, _stateStr);
            return appendStringEpilog(oss, margin);
        }

        ProtocolId_t ProtocolHeader::protocolId() const {
            return _hdr._protocolId;
        }

        void ProtocolHeader::validate() {
            if (_hdr._protocolId != ProtocolId_t(0) &&
                _hdr._protocolId != ProtocolId_t(2) &&
                _hdr._protocolId != ProtocolId_t(3)) {
                addError(new amqpAnalyze::AmqpValidationError(_packetNum, _dataOffset,
                         MSG("ProtocolHeader::ProtocolHeader(): Invalid AMQP protocol id: 0x" << std::hex
                             << (int)_hdr._protocolId)));
            }
        }

        // static
        std::map<ProtocolId_t, const char*> ProtocolHeader::s_protocolIdName = {
            {ProtocolId_t::AMQP, "AMQP"},
            {ProtocolId_t::TLS, "TLS"},
            {ProtocolId_t::SASL, "SASL"}
        };

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */
