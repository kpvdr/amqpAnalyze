/*
 * AmqpHeader.cpp
 *
 *  Created on: Apr 3, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/amqp10/ProtocolHeader.hpp>

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

        ProtocolHeader::ProtocolHeader(std::size_t frameOffset, const struct ProtocolHeader::hdr* hdrPtr):
                FrameBase(frameOffset),
                _hdr(hdrPtr)
        {
            if (_hdr._magic != 0x414d5150 || // "AMQP"
                _hdr._major != 1 ||
                _hdr._minor != 0) {
                throw amqpAnalyze::Error(MSG("Not AMQP 1.0 header: magic=0x" << std::hex << _hdr._magic
                                             << " major=0x" << (int)_hdr._major << " minor=0x" << (int)_hdr._minor));
            }
            if (_hdr._protocolId != 0 &&
                _hdr._protocolId != 2 &&
                _hdr._protocolId != 3) {
                throw amqpAnalyze::Error(MSG("Invalid AMQP protocol id: 0x" << std::hex << (int)_hdr._protocolId));
            }
        }

        ProtocolHeader::~ProtocolHeader() {}

        void ProtocolHeader::appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const {
            if (margin > 0 && !ignoreFirstMargin) oss << "\n" << std::string(margin, ' ');
            oss << "[" << std::setw(4) << std::setfill('0') << std::hex << _frameOffset << std::dec
                << "] " << std::b_cyan << "AMQP header v1.0." << (int)_hdr._revision << std::res << " pid=0x" << std::hex << (int)_hdr._protocolId
                << " (" << s_protocolIdName[_hdr._protocolId] << ")";
        }

        std::size_t ProtocolHeader::frameSize() const {
            return sizeof(struct hdr);
        }

        // static
        std::map<uint8_t, const char*> ProtocolHeader::s_protocolIdName = {
            {0, "AMQP"},
            {2, "TLS"},
            {3, "SASL"}};

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */
