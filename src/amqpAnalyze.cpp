/*
 * amqp_analyze.cpp
 *
 *  Created on: Mar 30, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze.hpp>

#include <amqpAnalyze_config.hpp>
#include <amqpAnalyze/Error.hpp>
#include <amqpAnalyze/FileParser.hpp>

#include <cstdlib>
#include <iostream>
#include <libgen.h>

// Global FileParser instance
amqpAnalyze::FileParser* g_fileParserPtr;

// Global function wrapper
void packetHandler(u_char *userData, const struct ::pcap_pkthdr* pkthdr, const u_char* packet) {
    return g_fileParserPtr->packetHandler(userData, pkthdr, packet);
}

int main(int argc, char** argv) {
    std::cout << basename(argv[0]) << " v" << amqpAnalyze_VERSION_MAJOR << "." << amqpAnalyze_VERSION_MINOR << "\n";
    try {
        if (argc != 2) throw amqpAnalyze::Error(MSG("Incorrect number of arguments. Usage: amapAnalyze <pcapng_file>"));
        // Create global file parser
        g_fileParserPtr = new amqpAnalyze::FileParser;
        g_fileParserPtr->parse(argv[1]);
        delete g_fileParserPtr;
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}


