/*
 * amqp_analyze.cpp
 *
 *  Created on: Mar 30, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze.hpp>

#include <amqpAnalyze_config.hpp>
#include <amqpAnalyze/amqp10/ConnectionHandler.hpp>
#include <amqpAnalyze/FileParser.hpp>
#include <amqpAnalyze/Options.hpp>
#include <amqpAnalyze/TcpConnectionMap.hpp>
#include <iostream>
#include <libgen.h>

int main(int argc, char** argv) {
    std::cout << basename(argv[0]) << " v" << amqpAnalyze_VERSION_MAJOR << "." << amqpAnalyze_VERSION_MINOR << "\n";
    try {
        // Create global instances
        g_optionsPtr = new amqpAnalyze::Options(argc, argv);
        g_fileParserPtr = new amqpAnalyze::FileParser();

        g_fileParserPtr->parse(std::cout);
        g_tcpConnectionMap.print(std::cout);
        std::cout << "\n\n";
        g_fileParserPtr->print(std::cout);
        std::cout << "\ndone" << std::endl;

        // Delete global instances
        delete g_fileParserPtr;
        delete g_optionsPtr;
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}


