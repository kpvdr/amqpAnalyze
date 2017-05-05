/*
 * AmqpDissector.cpp
 *
 *  Created on: Apr 2, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/AmqpDissector.hpp>

#include <amqpAnalyze.hpp>
#include <amqpAnalyze/amqp10/ConnectionHandler.hpp>
#include <amqpAnalyze/amqp10/Frame.hpp>
#include <amqpAnalyze/amqp10/FrameBuffer.hpp>
#include <amqpAnalyze/amqp10/ProtocolHeader.hpp>
#include <amqpAnalyze/Options.hpp>
#include <amqpAnalyze/Packet.hpp>
#include <amqpAnalyze/TcpAddressInfo.hpp>
#include <amqpAnalyze/TcpDissector.hpp>
#include <iomanip>
#include <iostream>
#include <cmath>
#include <typeinfo>

namespace amqpAnalyze {

    AmqpDissector::AmqpDissector(Packet* packetPtr, uint32_t dataOffs, Dissector* parent, std::size_t amqpDataSize):
            Dissector(packetPtr, dataOffs, parent),
            _amqpBlockList()
    {
        try {
            amqp10::FrameBuffer frameBuffer(_packetPtr->packetNum(), packetPtr->dataPtr() + _dataOffs, amqpDataSize);
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
                g_amqpConnectionHandler.handleFrame(tcpDissectorPtr, amqpBlockPtr);
                if (g_optionsPtr->s_showAmqpDataFlag) _debugHexFrameData.assign(frameBuffer.getHexDump());
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
        if (g_optionsPtr->s_showAmqpDataFlag) oss << "\n" << _debugHexFrameData;
        oss << "\n" << std::string(margin, ' ') << Color::color(DisplayColorType_t::DISSECTOR_NAME, "AMQP") << ": ";
        for (amqp10::AmqpBlockListCitr_t i=_amqpBlockList.begin(); i!=_amqpBlockList.end(); ++i) {
            (*i)->appendString(oss, margin + 6, i == _amqpBlockList.begin());
        }
        appendErrors(oss, margin);
    }

} /* namespace amqpAnalyze */
