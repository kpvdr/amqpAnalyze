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
#include <amqpAnalyze/amqp10/FrameBuffer10.hpp>
#include <amqpAnalyze/amqp10/ProtocolHeader.hpp>
#include <amqpAnalyze/Options.hpp>
#include <amqpAnalyze/Packet.hpp>
#include <amqpAnalyze/TcpConnection.hpp>
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
            _tcpDissectorPtr(dynamic_cast<TcpDissector*>(parent)),
            _debugHexFrameData(),
            _amqpBlockList()
    {
        if (_tcpDissectorPtr == nullptr) {
            throw amqpAnalyze::Error(MSG("AmqpDissector::AmqpDissector(): Unexpected dissector found: expected \"TcpDissector\", found \"" << _parent->name() << "\""));
        }
        try {
            TcpConnection* tcpConnectionPtr = _tcpDissectorPtr->tcpConnection();
            if (tcpConnectionPtr == nullptr) {
                throw amqpAnalyze::Error(MSG("AmqpDissector::AmqpDissector(): TcpDissector has null TcpConnection"));
            }
            AmqpVersions_t currentAmqpVersion = tcpConnectionPtr->amqpVersion(); // Get AMQP version associated with this connection
            const AmqpVersions_t thisPacketAmqpVersion = protocolHeaderVersion(); // If this is an AMQP protocol header, get its version
            if (thisPacketAmqpVersion != AmqpVersions_t::NONE) {
                // This is an AMQP protocol header
                if (currentAmqpVersion != AmqpVersions_t::NONE && !_tcpDissectorPtr->replyFlag()) {
                    // This TCP connection already has seen an AMQP header and is not a reply
                    throw amqpAnalyze::Error(MSG("AmqpDissector::AmqpDissector(): Protocol header " << s_amqpVerionStr[thisPacketAmqpVersion]
                                    << " found, but current AMQP version for this connection already set (to " << s_amqpVerionStr[currentAmqpVersion] << ")"));

                }
                tcpConnectionPtr->setAmqpVersion(thisPacketAmqpVersion);
                currentAmqpVersion = thisPacketAmqpVersion;
            }
            switch (currentAmqpVersion) {
            case AmqpVersions_t::AMQP_0_8:
                handleAmqp0_8();
                break;
            case AmqpVersions_t::AMQP_0_9:
                handleAmqp0_9();
                break;
            case AmqpVersions_t::AMQP_0_9_1:
                handleAmqp0_9_1();
                break;
            case AmqpVersions_t::AMQP_0_10:
                handleAmqp0_10();
                break;
            case AmqpVersions_t::AMQP_1_0:
                handleAmqp1_0();
                break;
            default:; // ignore, not handled
            }
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
        if (g_optionsPtr->s_showAmqpDataFlag && !_debugHexFrameData.empty()) oss << "\n" << _debugHexFrameData;
        oss << "\n" << std::string(margin, ' ') << Color::color(DisplayColorType_t::DISSECTOR_NAME, "AMQP") << ": ";
        for (amqp10::AmqpBlockListCitr_t i=_amqpBlockList.begin(); i!=_amqpBlockList.end(); ++i) {
            (*i)->appendString(oss, margin + 6, i == _amqpBlockList.begin());
        }
        appendErrors(oss, margin);
    }

    // static
    const char* AmqpDissector::amqpVersionStr(AmqpVersions_t amqpVersion) {
        return s_amqpVerionStr[amqpVersion];
    }

    // protected

    void AmqpDissector::handleAmqp0_8() {
        addError(new Error(MSG("AMQP 0-8 not handled")));
    }

    void AmqpDissector::handleAmqp0_9() {
        addError(new Error(MSG("AMQP 0-9 not handled")));
    }

    void AmqpDissector::handleAmqp0_9_1() {
        addError(new Error(MSG("AMQP 0-9-1 not handled")));
    }

    void AmqpDissector::handleAmqp0_10() {
        addError(new Error(MSG("AMQP 0-10 not handled")));
    }

    void AmqpDissector::handleAmqp1_0() {
        amqp10::FrameBuffer10 frameBuffer(_packetPtr->packetNum(), _packetPtr->dataPtr() + _dataOffs, _amqpDataSize);
        while (!frameBuffer.empty()) {
            if (std::string((const char*)frameBuffer.getDataPtr(), 4).compare("AMQP") == 0) {
                amqp10::ProtocolHeader* protocolHeaderPtr = new amqp10::ProtocolHeader(frameBuffer);
                if (g_optionsPtr->s_validateFlag) protocolHeaderPtr->validate();
                _amqpBlockList.push_back(protocolHeaderPtr);
                g_amqpConnectionHandler.handleProtocolHeader(_tcpDissectorPtr->tcpConnection(), _tcpDissectorPtr->replyFlag(), protocolHeaderPtr);
            } else {
                amqp10::Frame* framePtr = new amqp10::Frame(frameBuffer);
                if (g_optionsPtr->s_validateFlag) framePtr->validate();
                _amqpBlockList.push_back(framePtr);
                g_amqpConnectionHandler.handleFrame(_tcpDissectorPtr->tcpConnection(), _tcpDissectorPtr->replyFlag(), framePtr);
            }
            if (g_optionsPtr->s_showAmqpDataFlag) _debugHexFrameData.assign(frameBuffer.getHexDump());
        }
    }

    AmqpVersions_t AmqpDissector::protocolHeaderVersion() {
        if (_amqpDataSize >= 8) { // Must have at least 8 bytes
            const uint8_t* dataPtr = _packetPtr->dataPtr() + _dataOffs;
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
                        break;
                    default:  return AmqpVersions_t::UNKNOWN;
                    }
                    break;
                case 1: // AMQP 0-x (other than 0-9-1)
                    if (*(dataPtr + 6) != 0) return AmqpVersions_t::UNKNOWN; // 7th byte, major, must be 0
                    switch (*(dataPtr + 7)) { // 8th byte, minor
                    case 8: return AmqpVersions_t::AMQP_0_8;
                    case 9: return AmqpVersions_t::AMQP_0_9;
                    case 10: return AmqpVersions_t::AMQP_0_10;
                    default: return AmqpVersions_t::UNKNOWN;
                    }
                    break;
                default:
                    // Has AMQP magic, but unrecognized version
                    return AmqpVersions_t::UNKNOWN;
                }
            }
        }
        // Not AMQP header (No AMQP magic in first 4 bytes)
        return AmqpVersions_t::NONE;
    }

} /* namespace amqpAnalyze */
