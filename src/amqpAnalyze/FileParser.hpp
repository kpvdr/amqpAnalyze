/*
 * FileParser.hpp
 *
 *  Created on: Mar 30, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_FILEPARSER_HPP_
#define SRC_AMQPANALYZE_FILEPARSER_HPP_

#include <amqpAnalyze/IpConnectionMap.hpp>
#include <string>
#include <vector>

struct ether_header;
struct pcap_pkthdr;

namespace amqpAnalyze
{
    class Packet;

    class FileParser {
    protected:
        std::string _fileName;
        struct timeval _firstPakcetTimestamp;
        uint64_t _packetNumber;
        std::vector<Packet*> _packetList;
        IpConnectionMap _ipConnectionMap;
    public:
        FileParser();
        virtual ~FileParser();
        void parse(const char* fileName);
        void packetHandler(u_char *userData, const struct ::pcap_pkthdr* pkthdr, const u_char* packet);
    };

} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_FILEPARSER_HPP_ */
