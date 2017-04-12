/*
 * amqpAnalyze.hpp
 *
 *  Created on: Mar 31, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_HPP_
#define SRC_AMQPANALYZE_HPP_

#include <amqpAnalyze/FileParser.hpp>


// Global FileParser instance
extern amqpAnalyze::FileParser* g_fileParserPtr;

// Global function wrapper
void packetHandler(u_char *userData, const struct ::pcap_pkthdr* pkthdr, const u_char* packet);

#endif /* SRC_AMQPANALYZE_HPP_ */
