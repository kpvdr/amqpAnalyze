/*
 * AmqpDissector.cpp
 *
 *  Created on: Apr 2, 2017
 *      Author: kvdr
 */

#include <amqpAnalyze/AmqpDissector.hpp>

#include <amqpAnalyze/amqp10/Frame.hpp>
#include <amqpAnalyze/amqp10/FrameBuffer.hpp>
#include <amqpAnalyze/amqp10/ProtocolHeader.hpp>
#include <amqpAnalyze/Error.hpp>
#include <deque>
#include <pcap.h>
#include <std/AnsiTermColors.hpp>

// debug
#include <iomanip>
#include <cmath>

namespace amqpAnalyze {

AmqpDissector::AmqpDissector(const WireDissector* parent,
                             uint64_t packetNum,
                             const struct pcap_pkthdr* pcapPacketHeaderPtr,
                             const uint8_t* packetPtr,
                             uint32_t packetOffs,
                             std::deque<WireDissector*>& protocolList,
                             std::size_t amqpDataSize):
		WireDissector(parent, packetNum, pcapPacketHeaderPtr, packetPtr, packetOffs, DISSECTOR_AMQP, protocolList),
		_amqpBlockList()
{
    // TODO: use a flag or switch to turn this on/off - it consumes memory!
    // TODO: use FrameBuffer for this
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


    try {
        amqp10::FrameBuffer frameBuffer(packetNum, packetPtr + packetOffs, amqpDataSize);
        while (!frameBuffer.empty()) {
            if (std::string((const char*)frameBuffer.getDataPtr(), 4).compare("AMQP") == 0) {
                amqp10::ProtocolHeader* php(new amqp10::ProtocolHeader(frameBuffer));
                php->validate(); // TODO - control validation with a command-line switch
                _amqpBlockList.push_back(php);
            } else {
                amqp10::Frame* fp(new amqp10::Frame(frameBuffer));
                fp->validate(); // TODO - control validation with a command-line switch
                _amqpBlockList.push_back(fp);
            }
        }
    } catch (const Error& e) {
        std::cout << std::fgnd_b_red << "Error: " << e.what() << std::res << std::endl;
    }
}

AmqpDissector::~AmqpDissector() {
    for (amqp10::amqp_block_list_itr_t i=_amqpBlockList.begin(); i!=_amqpBlockList.end(); ++i) {
        delete (*i);
    }
    _amqpBlockList.clear();
}

void AmqpDissector::appendString(std::ostringstream& oss, size_t margin) const {
    oss << "\n" << _debugHexFrameData; // TODO: use frameBuffer for this
    oss << "\n" << std::string(margin, ' ') << std::fgnd_green << "AMQP" << std::res << ": ";
    for (amqp10::amqp_block_list_citr_t i=_amqpBlockList.begin(); i!=_amqpBlockList.end(); ++i) {
        (*i)->appendString(oss, margin + 6, i == _amqpBlockList.begin());
    }
}

} /* namespace amqpAnalyze */
