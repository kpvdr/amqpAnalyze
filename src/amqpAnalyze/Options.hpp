/*
 * Options.hpp
 *
 *  Created on: Apr 21, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_OPTIONS_HPP_
#define SRC_AMQPANALYZE_OPTIONS_HPP_

#include <getopt.h>
#include <string>

namespace amqpAnalyze
{

    class Options
    {
    public:
        Options(int argc, char** argv);
        virtual ~Options();

        static void printHelp(const char* baseName);
        static struct option s_longOptions[];
        static bool s_colorFlag;
        static std::string s_fileName;
        static bool s_showAmqpDataFlag;
        static bool s_showStateFlag;
        static bool s_validateFlag;
    };

} /* namespace amqpAnalyze */

// Global Options ptr instance
extern amqpAnalyze::Options* g_optionsPtr;

#endif /* SRC_AMQPANALYZE_OPTIONS_HPP_ */
