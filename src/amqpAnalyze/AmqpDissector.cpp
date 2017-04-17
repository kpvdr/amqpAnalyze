/*
 * AmqpDissector.cpp
 *
 *  Created on: Apr 2, 2017
 *      Author: kvdr
 */

#include <amqpAnalyze/AmqpDissector.hpp>

#include <amqpAnalyze/amqp10/FrameBuffer.hpp>
#include <amqpAnalyze/amqp10/Performative.hpp>
#include <amqpAnalyze/amqp10/ProtocolHeader.hpp>
#include <amqpAnalyze/Error.hpp>

#include <deque>
#include <pcap.h>

// debug
#include <iomanip>
#include <iostream>
#include <cstring>
#include <cctype>
#include <cmath>

namespace amqpAnalyze {

AmqpDissector::AmqpDissector(uint64_t packetNum,
                             const struct pcap_pkthdr* pcapPacketHeaderPtr,
                             const uint8_t* packetPtr,
                             uint32_t packetOffs,
                             std::deque<WireDissector*>& protocolList,
                             std::size_t amqpDataSize,
                             const TcpDissector* tcpDissectorPtr):
		WireDissector(packetNum, pcapPacketHeaderPtr, packetPtr, packetOffs, DISSECTOR_AMQP, protocolList),
		_tcpDissectorPtr(tcpDissectorPtr),
		_amqpFrameList()
{
/*
    // TODO: use a flag or switch to turn this on/off - it consumes memory!
    std::ostringstream oss;
    for (std::size_t i=0; i<std::ceil(amqpDataSize/16.0); ++i) {
        std::size_t numChars = amqpDataSize >= ((i+1)*16) ? 16 : amqpDataSize - (i*16);
        oss << "  " << std::hex << std::setw(8) << std::setfill('0') << (i*16) << "  ";
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
        oss << "|" << std::endl;
    }
    _debugHexFrameData.assign(oss.str());
*/


    std::size_t amqpOffs = 0;
    try {
        while (amqpDataSize - amqpOffs >= 8) {
            std::string magic((const char*)(packetPtr+packetOffs+amqpOffs), 4);
            if (magic.compare("AMQP") == 0) {
                // AMQP header
                amqp10::ProtocolHeader* amqpHdrPtr = new amqp10::ProtocolHeader(amqpOffs, (const amqp10::ProtocolHeader::hdr*)(packetPtr+packetOffs+amqpOffs));
                amqpOffs += amqpHdrPtr->frameSize();
                _amqpFrameList.push_back(amqpHdrPtr);
            } else {
                // AMQP frame
                amqp10::FrameBuffer frameBuffer(packetNum, packetOffs+amqpOffs, packetPtr+packetOffs+amqpOffs);
                _amqpFrameList.push_back(amqp10::Performative::decode(frameBuffer));
                amqpOffs += frameBuffer.getSize();
            }
        }
    } catch (const Error& e) {std::cout << " Error: " << e.what() << std::endl;}
}

AmqpDissector::~AmqpDissector() {
    for (std::deque<amqp10::FrameBase*>::iterator i=_amqpFrameList.begin(); i!=_amqpFrameList.end(); ++i) {
        delete (*i);
    }
    _amqpFrameList.clear();
}

void AmqpDissector::appendString(std::ostringstream& oss, size_t margin) const {
    oss << std::endl << std::string(margin, ' ') << "AMQP: ";
    for (std::deque<amqp10::FrameBase*>::const_iterator i=_amqpFrameList.begin(); i!=_amqpFrameList.end(); ++i) {
        (*i)->appendString(oss, margin + 6, i == _amqpFrameList.begin());
     }
}

} /* namespace amqpAnalyze */
