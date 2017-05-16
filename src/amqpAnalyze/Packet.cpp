/*
 * Packet.cpp
 *
 *  Created on: Mar 31, 2017
 *      Author: kpvdr
 */

#include "Packet.hpp"

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

    Packet::Packet(uint64_t packetNum,
                   const struct pcap_pkthdr* pcapPacketHeaderPtr,
                   const uint8_t* dataPtr,
                   const struct timeval& relativeTimestamp):
        _packetNum(packetNum),
        _pcapPacketHeaderPtr(pcapPacketHeaderPtr),
        _dataPtr(dataPtr),
        _relativeTimestamp({relativeTimestamp.tv_sec, relativeTimestamp.tv_usec}),
        _captureLength(pcapPacketHeaderPtr->caplen),
        _packetLength(pcapPacketHeaderPtr->len),
		_dissectorList()
    {
    	// Ethernet header
        const struct ether_header* ethernetHeader = (struct ether_header*)dataPtr;
        switch (ntohs(ethernetHeader->ether_type)) {
        case ETHERTYPE_IP:
            addDissector(new Ip4Dissector(this, sizeof(struct ether_header)));
        	break;
        case ETHERTYPE_IPV6:
            addDissector(new Ip6Dissector(this, sizeof(struct ether_header)));
        	break;
        default:
        	throw Error(MSG("[" << _packetNum << "] Ethernet header: protocol not IP, found 0x" << std::hex << ethernetHeader->ether_type));
        }
    }

    Packet::~Packet() {
    	for (DissectorList_itr_t i=_dissectorList.begin(); i!=_dissectorList.end(); ++i) {
    		delete (*i);
    	}
    	_dissectorList.clear();
    }

    void Packet::addDissector(const Dissector* dissectorPtr) {
        _dissectorList.push_front(dissectorPtr);
    }

    std::string Packet::toString(std::size_t margin) const {
        std::ostringstream oss;
        // Packet number filter
        if (_packetNum >= g_optionsPtr->s_fromPacket && _packetNum <= g_optionsPtr->s_toPacket) {
            // Connection number filter
            if (g_optionsPtr->s_connectionIndexSet.empty() || g_optionsPtr->s_connectionIndexSet.find(connectionIndex()) != g_optionsPtr->s_connectionIndexSet.end()) {
                // AMQP Dissector filter
                if (!g_optionsPtr->s_amqpFlag || (_dissectorList.size() > 0 &&  _dissectorList.back()->dissectorType() == DissectorType_t::DISSECTOR_AMQP)) {
                    // Finally, ok to print this packet
                    std::string m(margin, ' ');
                    oss << m << "[" << _packetNum << "] " << _relativeTimestamp.tv_sec << "." << std::setfill('0') << std::setw(6)
                        << _relativeTimestamp.tv_usec << "s  " << _captureLength;
                    if (_captureLength != _packetLength) oss << "/" << _packetLength;
                    oss << " bytes";
                    for (DissectorList_citr_t i=_dissectorList.cbegin(); i!=_dissectorList.cend(); ++i) {
                        (*i)->appendString(oss, margin + 2);
                    }
                    oss << "\n";
                }
            }
        }
    	return oss.str();
    }

    uint32_t Packet::connectionIndex() const {
        if (_dissectorList.size() < 2) {
            throw Error(MSG("[" << _packetNum << "] Packet::connectionIndex: Insufficient protocol dissectors in dissectorList: size=" << _dissectorList.size()));
        }
        if (_dissectorList[1]->dissectorType() != DissectorType_t::DISSECTOR_TCP) {
            throw Error(MSG("[" << _packetNum << "] Packet::connectionIndex: Expected dissector type \"TcpDissector\" at dissectorList[1], found \"" << _dissectorList[1]->name() << "\""));
        }
        const TcpDissector* tcpDissectorPtr(dynamic_cast<const TcpDissector*>(_dissectorList[1]));
        if (tcpDissectorPtr == nullptr) {
            throw Error(MSG("[" << _packetNum << "] Packet::connectionIndex: Error downcasting from Dissector* to TcpDissector*"));
        }
        return tcpDissectorPtr->connectionIndex();
    }

} /* namespace amqpAnalyze */
