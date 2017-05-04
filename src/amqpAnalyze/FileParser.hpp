/*
 * FileParser.hpp
 *
 *  Created on: Mar 30, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_FILEPARSER_HPP_
#define SRC_AMQPANALYZE_FILEPARSER_HPP_

#include <amqpAnalyze/amqp10/ConnectionHandler.hpp>
#include <amqpAnalyze/TcpConnectionMap.hpp>
#include <pcap.h>
#include <vector>

namespace amqpAnalyze
{
    class Packet;

    class FileParser {
    public:
        FileParser();
        virtual ~FileParser();
        void parse(std::ostream& os);
        void packetHandler(u_char *userData, const struct ::pcap_pkthdr* pkthdr, const u_char* packet);
        void print(std::ostream& os) const;
    protected:
        struct timeval _firstPakcetTimestamp;
        uint64_t _packetNumber;
        std::vector<Packet*> _packetList;
    };

} /* namespace amqpAnalyze */

// Global FileParser instance
extern amqpAnalyze::FileParser* g_fileParserPtr;

// Global function wrapper
void packetHandler(u_char *userData, const struct ::pcap_pkthdr* pkthdr, const u_char* packet);

#endif /* SRC_AMQPANALYZE_FILEPARSER_HPP_ */
