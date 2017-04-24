/*
 * AmqpHeader.cpp
 *
 *  Created on: Apr 3, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/amqp10/ProtocolHeader.hpp>

#include <amqpAnalyze/amqp10/FrameBuffer.hpp>
#include <iomanip>
#include <netinet/in.h>
#include <std/AnsiTermColors.hpp>

namespace amqpAnalyze
{
    namespace amqp10
    {

        ProtocolHeader::hdr::hdr(const struct hdr* hdrPtr):
                _magic(::ntohl(hdrPtr->_magic)),
                _protocolId(hdrPtr->_protocolId),
                _major(hdrPtr->_major),
                _minor(hdrPtr->_minor),
                _revision(hdrPtr->_revision)
        {}

        ProtocolHeader::ProtocolHeader(FrameBuffer& frameBuffer):
                AmqpBlock(frameBuffer.getPacketNum(), frameBuffer.pushFrameOffsetSnapshot()),
                _hdr((hdr*)frameBuffer.getStructPtr(sizeof(hdr)))
        {
            frameBuffer.popFrameOffsetSnapshot();
        }

        ProtocolHeader::~ProtocolHeader() {}

        std::ostringstream& ProtocolHeader::appendString(std::ostringstream& oss, std::size_t margin, bool ignoreMargin, bool colorFlag) const {
            if (margin > 0 && !ignoreMargin) oss << "\n" << std::string(margin, ' ');
            oss << "[" << std::hex << std::setfill('0') << std::setw(4) << _dataOffset << "] h " << std::dec;
            oss << AC(colorFlag, FGND_BCYN) << "AMQP header v" << (int)_hdr._major << "." << (int)_hdr._minor << "." << (int)_hdr._revision;
            oss << AC(colorFlag, RST) << " pid=0x" << std::hex << (int)_hdr._protocolId << " (" << s_protocolIdName[_hdr._protocolId] << ")";
            return appendStringEpilog(oss, margin, colorFlag);
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
