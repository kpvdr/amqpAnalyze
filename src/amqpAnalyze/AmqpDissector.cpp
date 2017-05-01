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
#include <amqpAnalyze/TcpDissector.hpp>
#include <amqpAnalyze/Options.hpp>
#include <amqpAnalyze/TcpAddressInfo.hpp>
#include <amqpAnalyze/Utils.hpp>
#include <iomanip>
#include <cmath>
#include <std/AnsiTermColors.hpp>
#include <typeinfo>

namespace amqpAnalyze {

AmqpDissector::AmqpDissector(const Dissector* parent,
                             uint64_t packetNum,
                             const struct pcap_pkthdr* pcapPacketHeaderPtr,
                             const uint8_t* packetPtr,
                             uint32_t packetOffs,
                             DissectorList_t& protocolList,
                             std::size_t amqpDataSize):
		Dissector(parent, packetNum, packetOffs, protocolList),
		_amqpBlockList()
{
    // TODO: use FrameBuffer for this
    if (g_optionsPtr->s_showAmqpDataFlag) {
        std::ostringstream oss;
        for (std::size_t i=0; i<std::ceil(amqpDataSize/16.0); ++i) {
            if (i > 0) oss << "\n";
            std::size_t numChars = amqpDataSize >= ((i+1)*16) ? 16 : amqpDataSize - (i*16);
            oss << "  " << std::hex << std::setw(4) << std::setfill('0') << (i*16) << "  ";
            for (size_t c=0; c<16; ++c) {
                oss << (c==8?"  ":" ");
                if (c < numChars)
                    oss << std::setw(2) << (int)*(unsigned char*)(packetPtr + packetOffs + (i*16) + c);
                else
                    oss << "  ";
            }
            oss << " |";
            for (std::size_t c=0; c<16; ++c) {
                char x = *(unsigned char*)(packetPtr + packetOffs + (i*16) + c);
                oss << (c==8?" ":"");
                if (c < numChars)
                    oss << (std::isprint(x) ? x : '.');
                else
                    oss << ' ';
            }
            oss << "|";
        }
        _debugHexFrameData.assign(oss.str());
    }
//std::cout << _debugHexFrameData << "\n\n";


    try {
        amqp10::FrameBuffer frameBuffer(packetNum, packetPtr + packetOffs, amqpDataSize);
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
            const TcpDissector* tcpDissectorPtr(dynamic_cast<const TcpDissector*>(_parent));
            if (tcpDissectorPtr == nullptr) {
                throw amqpAnalyze::Error(MSG("AmqpDissector::AmqpDissector(): Unexpected dissector found: expected \"TcpDissector\", found \"" << _parent->name() << "\""));
            }
            g_amqpConnectionHandlerPtr->handleFrame(tcpDissectorPtr->getTcpAddressInfo(), amqpBlockPtr);
        }
    } catch (const Error& e) {
        std::cout << AC_F_BRED(g_optionsPtr->s_colorFlag) << "Error: " << e.what() << AC_RST(g_optionsPtr->s_colorFlag) << std::endl;
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
    oss << "\n" << std::string(margin, ' ') << COLOR(FGND_GRN, "AMQP", g_optionsPtr->s_colorFlag) << ": ";
    for (amqp10::AmqpBlockListCitr_t i=_amqpBlockList.begin(); i!=_amqpBlockList.end(); ++i) {
        (*i)->appendString(oss, margin + 6, i == _amqpBlockList.begin());
    }
}

} /* namespace amqpAnalyze */
