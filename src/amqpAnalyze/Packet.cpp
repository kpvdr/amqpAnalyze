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
#include <iomanip>
#include <net/ethernet.h>
#include <netinet/in.h>
#include <pcap.h>

namespace amqpAnalyze
{

    Packet::Packet(const struct pcap_pkthdr* pcapPacketHeaderPtr,
                   const uint8_t* packetPtr,
                   uint64_t packetNum,
                   const struct timeval& relativeTimestamp):
        _packetNum(packetNum),
        _relativeTimestamp({relativeTimestamp.tv_sec, relativeTimestamp.tv_usec}),
        _captureLength(pcapPacketHeaderPtr->caplen),
        _packetLength(pcapPacketHeaderPtr->len),
		_dissectorList()
    {
    	// Ethernet header
        const struct ether_header* ethernetHeader = (struct ether_header*)packetPtr;
        switch (::ntohs(ethernetHeader->ether_type)) {
        case ETHERTYPE_IP:
        	_dissectorList.push_front(new Ip4Dissector(_packetNum,
        	                                           pcapPacketHeaderPtr,
        	                                           packetPtr,
        	                                           sizeof(struct ether_header),
        	                                           _dissectorList));
        	break;
        case ETHERTYPE_IPV6:
        	_dissectorList.push_front(new Ip6Dissector(_packetNum,
        	                                           pcapPacketHeaderPtr,
        	                                           packetPtr,
        	                                           sizeof(struct ether_header),
        	                                           _dissectorList));
        	break;
        default:
        	throw Error(MSG("[" << _packetNum << "] Ethernet header: protocol not IP, found 0x" << std::hex << ethernetHeader->ether_type));
        }
    }

    Packet::~Packet() {
    	for (std::deque<Dissector*>::iterator i=_dissectorList.begin(); i!=_dissectorList.end(); ++i) {
    		delete (*i);
    	}
    	_dissectorList.clear();
    }

    std::string Packet::toString(std::size_t margin) const {
        std::ostringstream oss;
        // Packet number filter
        if (_packetNum >= g_optionsPtr->s_fromPacket && _packetNum <= g_optionsPtr->s_toPacket) {
            // AMQP Dissector filter
            if (!g_optionsPtr->s_amqpFlag || (_dissectorList.size() > 0 &&  _dissectorList.back()->dissectorType() == DissectorType_t::DISSECTOR_AMQP)) {
                std::string m(margin, ' ');
                oss << m << "[" << _packetNum << "] " << _relativeTimestamp.tv_sec << "." << std::setfill('0') << std::setw(6)
                    << _relativeTimestamp.tv_usec << "s  " << _captureLength;
                if (_captureLength != _packetLength) oss << "/" << _packetLength;
                oss << " bytes";
                for (std::deque<Dissector*>::const_iterator i=_dissectorList.cbegin(); i!=_dissectorList.cend(); ++i) {
                    (*i)->appendString(oss, margin + 2);
                }
                oss << "\n";
            }
        }
    	return oss.str();
    }

    std::string Packet::connectionIndex() const {
        if (_dissectorList.size() < 2) {
            throw Error(MSG("[" << _packetNum << "] Insufficient protocol dissectors in list"));
        }
        if (_dissectorList[1]->dissectorType() != DissectorType_t::DISSECTOR_TCP) {
            throw Error(MSG("[" << _packetNum << "] Unexpected dissector type at index 1: 0x" << std::hex << (int)_dissectorList[1]->dissectorType()));
        }
        TcpDissector* tcpDissector = (TcpDissector*)_dissectorList[1];
        return tcpDissector->getConnectionIndex();
    }

} /* namespace amqpAnalyze */
