/*
 * FileParser.cpp
 *
 *  Created on: Mar 30, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/FileParser.hpp>

#include <amqpAnalyze.hpp>
#include <amqpAnalyze/Error.hpp>
#include <amqpAnalyze/Packet.hpp>

#include <iomanip>
#include <iostream>
#include <pcap.h>


namespace amqpAnalyze
{

    FileParser::FileParser(): _fileName(), _packetNumber(0LL), _packetList(), _ipConnectionMap() {
        _firstPakcetTimestamp.tv_sec = 0;
        _firstPakcetTimestamp.tv_usec = 0;
    }

    FileParser::~FileParser() {
        for (std::vector<Packet*>::iterator i=_packetList.begin(); i!=_packetList.end(); ++i) {
            delete (*i);
        }
        _packetList.clear();
    }

    void FileParser::parse(const char* fileName) {
        _fileName = fileName;
        char errbuf[PCAP_ERRBUF_SIZE];
        pcap_t* descr = ::pcap_open_offline(_fileName.c_str(), errbuf);
        if (descr == nullptr) {
            throw Error(MSG("Opening pcap file \"" << _fileName << "\" failed: " << errbuf));
        }
        if (::pcap_loop(descr, 0, ::packetHandler, nullptr) < 0) {
            throw Error(MSG("Parsing pcap file \"" << _fileName << "\" failed: " << pcap_geterr(descr)));
        }
        ::pcap_close(descr);

        // Print results
        for (std::vector<Packet*>::const_iterator i=_packetList.cbegin(); i!=_packetList.cend(); ++i) {
        	std::cout << (*i)->toString() << std::endl;
        }
        std::cout << std::endl;

/*
        // print hashList
        std::deque<std::size_t> hashList;
        uint32_t hashCount = _ipConnectionMap.getIpHashList(hashList);
        std::cout << std::endl << " Found " << hashCount << " connections:" << std::endl;
        for (std::deque<std::size_t>::const_iterator i=hashList.cbegin(); i!=hashList.cend(); ++i) {
            std::cout << std::endl << "Hash=0x" << std::setw(16) << std::setfill('0') << std::hex << *i << std::dec << std::endl << "-----------------------" << std::endl;
            const std::deque<const Packet*>* packetListPtr = _ipConnectionMap.getPacketList(*i);
            for (std::deque<const Packet*>::const_iterator j=packetListPtr->cbegin(); j!=packetListPtr->cend(); ++j) {
                std::cout << (*j)->toString() << std::endl;
            }
        }
*/
/*
        // print indexList
        std::deque<std::pair<std::string, std::size_t>> indexList;
        uint32_t count = _ipConnectionMap.getIpConnectionIndexList(indexList);
        std::cout << std::endl << " Found " << count << " connections:" << std::endl;
        for (std::deque<std::pair<std::string, std::size_t>>::const_iterator i=indexList.cbegin(); i!=indexList.cend(); ++i) {
            std::cout << std::endl << "Connection: [" << i->first << "]:" << std::endl << std::string(i->first.length() + 15, '-') << std::endl << std::endl;
            const std::deque<const Packet*>* packetListPtr = _ipConnectionMap.getPacketList(i->second);
            for (std::deque<const Packet*>::const_iterator j=packetListPtr->cbegin(); j!=packetListPtr->cend(); ++j) {
                std::cout << (*j)->toString() << std::endl;
            }
        }
*/

        std::cout << "done" << std::endl;
    }

    // static
    void FileParser::packetHandler(u_char *userData, const struct pcap_pkthdr* pkthdr, const u_char* packet) {
        if (_packetNumber == 0) {
            // copy first packet timesamp
            _firstPakcetTimestamp.tv_sec = pkthdr->ts.tv_sec;
            _firstPakcetTimestamp.tv_usec = pkthdr->ts.tv_usec;
        }
        struct timeval relativeTimestamp;
        timersub(&pkthdr->ts, &_firstPakcetTimestamp, &relativeTimestamp);
        Packet* packetPtr = new Packet(userData, pkthdr, packet, ++_packetNumber, relativeTimestamp);
        _packetList.push_back(packetPtr);
        _ipConnectionMap.addPacket(packetPtr);
    }

} /* namespace amqpAnalyze */
