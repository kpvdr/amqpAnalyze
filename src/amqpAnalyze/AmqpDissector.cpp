/*
 * AmqpDissector.cpp
 *
 *  Created on: Apr 2, 2017
 *      Author: kpvdr
 */

#include "AmqpDissector.hpp"

#include <amqpAnalyze.hpp>
#include <amqpAnalyze/amqp10/ConnectionHandler.hpp>
#include <amqpAnalyze/amqp10/Frame.hpp>
#include <amqpAnalyze/amqp10/FrameBuffer.hpp>
#include <amqpAnalyze/amqp10/ProtocolHeader.hpp>
#include <amqpAnalyze/Options.hpp>
#include <amqpAnalyze/Packet.hpp>
#include <amqpAnalyze/TcpDissector.hpp>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <netinet/in.h>
#include <typeinfo>

namespace amqpAnalyze {

    // static
    std::map<AmqpVersions_t, const char*> AmqpDissector::s_amqpVerionStr = {
        {AmqpVersions_t::NONE, "<none>"},
        {AmqpVersions_t::UNKNOWN, "<unknown>"},
        {AmqpVersions_t::AMQP_0_8, "AMQP 0-8"},
        {AmqpVersions_t::AMQP_0_9, "AMQP 0-9"},
        {AmqpVersions_t::AMQP_0_9_1, "AMQP 0-9-1"},
        {AmqpVersions_t::AMQP_0_10, "AMQP 0-10"},
        {AmqpVersions_t::AMQP_1_0, "AMQP 1.0"}
    };

    AmqpDissector::AmqpDissector(Packet* packetPtr, uint32_t dataOffs, Dissector* parent, std::size_t amqpDataSize):
            Dissector(packetPtr, dataOffs, parent),
            _amqpDataSize(amqpDataSize),
            _debugHexFrameData(),
            _amqpBlockList()
    {
        try {
            // TODO: use this approach when the current TcpConnection object can be associated with an AMQP protocol
            // so that it can send the following non-header packets on that connection to the correct protocol handler
/*
            switch (protocolHeaderVersion()) {
            case AmqpVersions_t::AMQP_0_10:
                handleAmqp_0_10();
                break;
            case AmqpVersions_t::AMQP_1_0:
                handleAmqp_1_0();
                break;
            case AmqpVersions_t::UNKNOWN:
                std::cout << Color::color(DisplayColorType_t::MSG_ERROR, MSG("Error: Unknown or unhandled AMQP protocol")) << std::endl;
            default: // Not AMQP header, handle according to session info
            }
*/
            handleAmqp_1_0();
        } catch (const Error& e) {
            std::cout << Color::color(DisplayColorType_t::MSG_ERROR, MSG("Error: " << e.what())) << std::endl;
        }
    }

    AmqpDissector::~AmqpDissector() {
        for (amqp10::AmqpBlockListItr_t i=_amqpBlockList.begin(); i!=_amqpBlockList.end(); ++i) {
            delete (*i);
        }
        _amqpBlockList.clear();
    }

    void AmqpDissector::appendString(std::ostringstream& oss, size_t margin) const {
        if (g_optionsPtr->s_showAmqpDataFlag) oss << "\n" << _debugHexFrameData;
        oss << "\n" << std::string(margin, ' ') << Color::color(DisplayColorType_t::DISSECTOR_NAME, "AMQP") << ": ";
        for (amqp10::AmqpBlockListCitr_t i=_amqpBlockList.begin(); i!=_amqpBlockList.end(); ++i) {
            (*i)->appendString(oss, margin + 6, i == _amqpBlockList.begin());
        }
        appendErrors(oss, margin);
    }

    void AmqpDissector::handleAmqp_0_10() {}

    void AmqpDissector::handleAmqp_1_0() {
        amqp10::FrameBuffer frameBuffer(_packetPtr->packetNum(), _packetPtr->dataPtr() + _dataOffs, _amqpDataSize);
        while (!frameBuffer.empty()) {
            amqp10::AmqpBlock* amqpBlockPtr = nullptr;
            if (std::string((const char*)frameBuffer.getDataPtr(), 4).compare("AMQP") == 0) {
                amqpBlockPtr = new amqp10::ProtocolHeader(frameBuffer);
                if (g_optionsPtr->s_validateFlag) amqpBlockPtr->validate();
            } else {
                amqpBlockPtr = new amqp10::Frame(frameBuffer);
                if (g_optionsPtr->s_validateFlag) amqpBlockPtr->validate();
            }
            _amqpBlockList.push_back(amqpBlockPtr);
            TcpDissector* tcpDissectorPtr(dynamic_cast<TcpDissector*>(_parent));
            if (tcpDissectorPtr == nullptr) {
                throw amqpAnalyze::Error(MSG("AmqpDissector::AmqpDissector(): Unexpected dissector found: expected \"TcpDissector\", found \"" << _parent->name() << "\""));
            }
            g_amqpConnectionHandler.handleFrame(tcpDissectorPtr->tcpConnection(), tcpDissectorPtr->replyFlag(), amqpBlockPtr);
            if (g_optionsPtr->s_showAmqpDataFlag) _debugHexFrameData.assign(frameBuffer.getHexDump());
        }
    }

    AmqpVersions_t AmqpDissector::protocolHeaderVersion() {
        if (_amqpDataSize >= 8) { // Must have at least 8 bytes
            const uint8_t* dataPtr = _packetPtr->dataPtr();
            const uint32_t amqpMagic(::ntohl(*((uint32_t*)dataPtr)));
            if (amqpMagic == 0x414d5150) { // First 4 bytes = "AMQP"
                switch (*(dataPtr + 4)) { // 5th byte
                case 0: // AMQP 1.0+, AMQP 0-9-1
                    switch (*(dataPtr + 5)) { // 6th byte, major
                    case 0: // version 0-x
                        if (*(dataPtr + 6) == 9 && *(dataPtr + 7) == 0) return AmqpVersions_t::AMQP_0_9_1;
                        return AmqpVersions_t::UNKNOWN;
                    case 1: // version 1.0+
                        switch (*(dataPtr + 6)) { // 7th byte, minor
                        case 0: return AmqpVersions_t::AMQP_1_0;
                        default: return AmqpVersions_t::UNKNOWN;
                        }
                    default:  return AmqpVersions_t::UNKNOWN;
                    }
                case 1: // AMQP 0-x (other than 0-9-1)
                    if (*(dataPtr + 6) != 0) return AmqpVersions_t::UNKNOWN; // 7th byte, major, must be 0
                    switch (*(dataPtr + 7)) { // 8th byte, minor
                    case 8: return AmqpVersions_t::AMQP_0_8;
                    case 9: return AmqpVersions_t::AMQP_0_9;
                    case 10: return AmqpVersions_t::AMQP_0_10;
                    default: return AmqpVersions_t::UNKNOWN;
                    }
                default:
                    return AmqpVersions_t::UNKNOWN;
                }
                uint8_t offset4 = *(dataPtr + 4);
                if (offset4 == 0) {

                } else if (offset4 == 1) { // AMQP 0-x versions except 0-9-1
                    if (*(dataPtr + 6) != 0) return AmqpVersions_t::UNKNOWN; // major != 0
                    switch (*(dataPtr + 7)) {
                    case 8: return AmqpVersions_t::AMQP_0_8;
                    case 9: return AmqpVersions_t::AMQP_0_9;
                    case 10: return AmqpVersions_t::AMQP_0_10;
                    default: return AmqpVersions_t::UNKNOWN;
                    }
                } else { // AMQP 1.0+ versions (and 0-9-1)

                }
            }
        }
        return AmqpVersions_t::NONE;
    }

} /* namespace amqpAnalyze */
