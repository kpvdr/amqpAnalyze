/*
 * Options.hpp
 *
 *  Created on: Apr 21, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_OPTIONS_HPP_
#define SRC_AMQPANALYZE_OPTIONS_HPP_

#include <getopt.h>
#include <set>
#include <string>

namespace amqpAnalyze
{

    class Options
    {
    public:
        static struct option s_longOptions[];

        static bool s_amqpFlag;
        static bool s_colorFlag;
        static std::set<uint32_t> s_connectionIndexSet;
        static bool s_showAmqpDataFlag;
        static uint64_t s_fromPacket;
        static uint32_t s_maxDisplaySize;
        static bool s_showStateFlag;
        static bool s_showNullArgs;
        static uint64_t s_toPacket;
        static bool s_validateFlag;

        static std::string s_fileName;

        Options(int argc, char** argv);
        virtual ~Options();

        static void handleConnectionIndex(const char* optarg);
        static void handleException(const char* baseName, const std::string& errMsg);
        static void printHelp(const char* baseName);
    };

} /* namespace amqpAnalyze */

// Global Options ptr instance
extern amqpAnalyze::Options* g_optionsPtr;

#endif /* SRC_AMQPANALYZE_OPTIONS_HPP_ */
