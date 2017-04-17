/*
 * Packet.cpp
 *
 *  Created on: Mar 31, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/Packet.hpp>

#include <amqpAnalyze/Error.hpp>
#include <amqpAnalyze/Ip4Dissector.hpp>
#include <amqpAnalyze/Ip6Dissector.hpp>
#include <amqpAnalyze/TcpDissector.hpp>
#include <amqpAnalyze/WireDissector.hpp>

#include <iomanip>
#include <net/ethernet.h>
#include <netinet/in.h>
#include <pcap.h>

// debug
//#include <iostream>

namespace amqpAnalyze
{

    Packet::Packet(uint8_t* userDataPtr,
                   const struct pcap_pkthdr* pcapPacketHeaderPtr,
                   const uint8_t* packetPtr,
                   uint64_t packetNum,
                   const struct timeval& relativeTimestamp):
        _packetNum(packetNum),
        _relativeTimestamp({relativeTimestamp.tv_sec, relativeTimestamp.tv_usec}),
        _captureLength(pcapPacketHeaderPtr->caplen),
        _packetLength(pcapPacketHeaderPtr->len),
		_protocolList()
    {
    	// Ethernet header
        const struct ether_header* ethernetHeader = (struct ether_header*)packetPtr;
        switch (::ntohs(ethernetHeader->ether_type)) {
        case ETHERTYPE_IP:
        	_protocolList.push_front(new Ip4Dissector(_packetNum, pcapPacketHeaderPtr, packetPtr, sizeof(struct ether_header), _protocolList));
        	break;
        case ETHERTYPE_IPV6:
        	_protocolList.push_front(new Ip6Dissector(_packetNum, pcapPacketHeaderPtr, packetPtr, sizeof(struct ether_header), _protocolList));
        	break;
        default:
        	throw Error(MSG("[" << _packetNum << "] Ethernet header: protocol not IP, found 0x" << std::hex << ethernetHeader->ether_type));
        }
    }

    Packet::~Packet() {
    	for (std::deque<WireDissector*>::iterator i=_protocolList.begin(); i!=_protocolList.end(); ++i) {
    		delete (*i);
    	}
    	_protocolList.clear();
    }

    std::string Packet::toString(std::size_t margin) const {
        std::string m(margin, ' ');
        std::ostringstream oss;
    	oss << m << "[" << _packetNum << "] " << _relativeTimestamp.tv_sec << "." << std::setfill('0') << std::setw(6)
    	    << _relativeTimestamp.tv_usec << "s  " << _captureLength;
    	if (_captureLength != _packetLength) oss << "/" << _packetLength;
    	oss << " bytes";
    	for (std::deque<WireDissector*>::const_iterator i=_protocolList.cbegin(); i!=_protocolList.cend(); ++i) {
    		(*i)->appendString(oss, margin + 2);
    	}
    	return oss.str();
    }

    std::string Packet::connectionIndex() const {
        if (_protocolList.size() < 2) {
            throw Error(MSG("[" << _packetNum << "] Insufficient protocol dissectors in list"));
        }
        if (_protocolList[1]->dissectorType() != DISSECTOR_TCP) {
            throw Error(MSG("[" << _packetNum << "] Unexpected dissector type at index 1: 0x" << std::hex << (int)_protocolList[1]->dissectorType()));
        }
        TcpDissector* tcpDissector = (TcpDissector*)_protocolList[1];
        return tcpDissector->getConnectionIndex();
    }

} /* namespace amqpAnalyze */
