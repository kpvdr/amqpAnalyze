/*
 * AmqpHeader.cpp
 *
 *  Created on: Apr 3, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/amqp10/ProtocolHeader.hpp>

#include <amqpAnalyze/amqp10/FrameBuffer.hpp>
#include <amqpAnalyze/Error.hpp>
#include <cstring>
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
                AmqpBlock(frameBuffer.setFrameOffsetSnapshot()),
                _hdr((hdr*)frameBuffer.getStructPtr(sizeof(hdr)))
        {
            if (_hdr._magic != 0x414d5150 || // "AMQP"
                _hdr._major != 1 ||
                _hdr._minor != 0) {
                throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "Not AMQP 1.0 header: magic=0x" << std::hex << _hdr._magic
                                             << " major=0x" << (int)_hdr._major << " minor=0x" << (int)_hdr._minor));
            }
            if (_hdr._protocolId != 0 &&
                _hdr._protocolId != 2 &&
                _hdr._protocolId != 3) {
                throw amqpAnalyze::Error(MSG(frameBuffer.getErrorPrefix() << "Invalid AMQP protocol id: 0x" << std::hex << (int)_hdr._protocolId));
            }
        }

        ProtocolHeader::~ProtocolHeader() {}

        std::ostringstream& ProtocolHeader::appendString(std::ostringstream& oss, std::size_t margin, bool ignoreMargin) const {
            if (margin > 0 && !ignoreMargin) oss << "\n" << std::string(margin, ' ');
            oss << "[" << std::hex << std::setfill('0') << std::setw(4) << _dataOffset << "] h " << std::dec;
            oss << std::b_cyan << "AMQP header v" << (int)_hdr._major << "." << (int)_hdr._minor << "." << (int)_hdr._revision;
            oss << std::res << " pid=0x" << std::hex << (int)_hdr._protocolId << " (" << s_protocolIdName[_hdr._protocolId] << ")";
            return oss;
        }

        protocolId_t ProtocolHeader::protocolId() const {
            return _hdr._protocolId;
        }

        // static
        std::map<uint8_t, const char*> ProtocolHeader::s_protocolIdName = {
            {protocolId_t::AMQP, "AMQP"},
            {protocolId_t::TLS, "TLS"},
            {protocolId_t::SASL, "SASL"}
        };

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */
