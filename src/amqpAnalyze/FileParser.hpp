/*
 * FileParser.hpp
 *
 *  Created on: Mar 30, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_FILEPARSER_HPP_
#define SRC_AMQPANALYZE_FILEPARSER_HPP_

#include <amqpAnalyze/IpConnectionMap.hpp>
#include <pcap.h>
#include <vector>

namespace amqpAnalyze
{
    class Options;
    class Packet;

    class FileParser {
    public:
        FileParser(const Options* optionsPtr);
        virtual ~FileParser();
        void parse();
        void packetHandler(u_char *userData, const struct ::pcap_pkthdr* pkthdr, const u_char* packet);
    protected:
        const Options* _optionsPtr;
        struct timeval _firstPakcetTimestamp;
        uint64_t _packetNumber;
        std::vector<Packet*> _packetList;
        IpConnectionMap _ipConnectionMap;
    };

} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_FILEPARSER_HPP_ */
