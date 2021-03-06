/*
 * FileParser.cpp
 *
 *  Created on: Mar 30, 2017
 *      Author: kpvdr
 */

#include "FileParser.hpp"

#include <amqpAnalyze.hpp>
#include <amqpAnalyze/Error.hpp>
#include <amqpAnalyze/Options.hpp>
#include <amqpAnalyze/Packet.hpp>


// Global FileParser instance
amqpAnalyze::FileParser* g_fileParserPtr;

// Global function wrapper
void packetHandler(u_char *userData, const struct ::pcap_pkthdr* pkthdr, const u_char* packet) {
    return g_fileParserPtr->packetHandler(userData, pkthdr, packet);
}

namespace amqpAnalyze
{

    FileParser::FileParser():
        _firstPakcetTimestamp({0, 0}),
        _packetNumber(0LL),
        _packetList()
    {}

    FileParser::~FileParser() {
        for (std::vector<Packet*>::iterator i=_packetList.begin(); i!=_packetList.end(); ++i) {
            delete (*i);
        }
        _packetList.clear();
    }

    void FileParser::parse(std::ostream& os) {
        char errbuf[PCAP_ERRBUF_SIZE];
        pcap_t* descr = ::pcap_open_offline(g_optionsPtr->s_fileName.c_str(), errbuf);
        if (descr == nullptr) {
            throw Error(MSG("Opening pcap file \"" << g_optionsPtr->s_fileName << "\" failed: " << errbuf));
        }
        if (::pcap_loop(descr, 0, ::packetHandler, nullptr) < 0) {
            throw Error(MSG("Parsing pcap file \"" << g_optionsPtr->s_fileName << "\" failed: " << pcap_geterr(descr)));
        }
        ::pcap_close(descr);
        os << _packetList.size() << " packets found in file \"" << g_optionsPtr->s_fileName << "\"\n\n";
    }

    void FileParser::packetHandler(u_char *userData, const struct pcap_pkthdr* pkthdr, const u_char* packet) {
        if (_packetNumber == 0) {
            // copy first packet timesamp
            _firstPakcetTimestamp.tv_sec = pkthdr->ts.tv_sec;
            _firstPakcetTimestamp.tv_usec = pkthdr->ts.tv_usec;
        }
        struct timeval relativeTimestamp;
        timersub(&pkthdr->ts, &_firstPakcetTimestamp, &relativeTimestamp);
        Packet* packetPtr = new Packet(++_packetNumber, pkthdr, (const uint8_t*)packet, relativeTimestamp);
        _packetList.push_back(packetPtr);
    }

    void FileParser::print(std::ostream& os) const {
        for (std::vector<Packet*>::const_iterator i=_packetList.cbegin(); i!=_packetList.cend(); ++i) {
            os << (*i)->toString();
        }
    }

} /* namespace amqpAnalyze */
