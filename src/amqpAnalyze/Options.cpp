/*
 * Options.cpp
 *
 *  Created on: Apr 21, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/Options.hpp>

#include <amqpAnalyze/Error.hpp>
#include <iostream>
#include <cstring>
#include <sys/stat.h>

// Global Options ptr instance
amqpAnalyze::Options* g_optionsPtr;

namespace amqpAnalyze
{

     /*
      * Adapted directly from example at
      * http://www.gnu.org/savannah-checkouts/gnu/libc/manual/html_node/Getopt-Long-Option-Example.html#Getopt-Long-Option-Example
      */


    Options::Options(int argc, char** argv)
    {
        bool done = false;
        int c;
        while (!done) {
            int option_index = 0;
            opterr = 0; // disable printing of error message from within getopt_long()
            c = ::getopt_long(argc, argv, "acdhsv", s_longOptions, &option_index);
            if (c == -1) break;
            switch(c) {
            case 'a':
                s_amqpFlag = true;
                break;
            case 'c':
                s_colorFlag = true;
                break;
            case 'd':
                s_showAmqpDataFlag = true;
                break;
            case 'h':
                printHelp(basename(argv[0]));
                throw amqpAnalyze::Error(""); // exit without an error message
            case 's':
                s_showStateFlag = true;
                break;
            case 'v':
                s_validateFlag = true;
                break;
            default:
                printHelp(basename(argv[0]));
                if (optopt != 0) throw amqpAnalyze::Error(MSG("Invalid option \'" << (char)optopt << "\'"));
                throw amqpAnalyze::Error(MSG("Invalid option \"" << argv[optind-1] << "\""));
            }
        }
        if (optind == argc) { // No file name
            printHelp(basename(argv[0]));
            throw amqpAnalyze::Error(MSG("No pcapng file specified"));
        } else if (optind - argc > 1) { // More than one file name
            std::ostringstream oss;
            oss << "Only one file:";
            while (optind < argc) oss << " " << argv[optind++];
            printHelp(basename(argv[0]));
            throw amqpAnalyze::Error(oss.str());
        } else {
            struct stat st;
            int res = stat(argv[optind], &st);
            if (res) throw amqpAnalyze::Error(MSG("Error reading file \"" << argv[optind] << "\": " << std::strerror(errno)));
            if (S_ISREG(st.st_mode) == 0) throw amqpAnalyze::Error(MSG("File \"" << argv[optind] << ": not regular file"));
            s_fileName = argv[optind];
        }
    }

    Options::~Options() {}

    void Options::printHelp(const char* baseName) {
        std::cout << "Usage: " << baseName << " [options] FILE\n"
                  << "options: -h --help           Print help\n"
                  << "         -a --amqp           Show only AMQP packets"
                  << "         -c --color          Use ANSI color\n"
                  << "         -d --show-amqp-data Show hex dump of AMQP data for each packet\n"
                  << "         -s --show_state     Show connection, session and link state\n"
                  << "         -v --validate       Validate AMQP data to AMQP 1.0 specification\n"
                  << "   FILE: pcapng file to be analyzed" << std::endl;
    }
    // static
    bool Options::s_amqpFlag = false;
    bool Options::s_colorFlag = false;
    std::string Options::s_fileName;
    bool Options::s_showAmqpDataFlag = false;
    bool Options::s_showStateFlag = false;
    bool Options::s_validateFlag = false;
    struct option Options::s_longOptions[] = {
        {"amqp",           no_argument, 0, 'a'},
        {"color",          no_argument, 0, 'c'},
        {"show-amqp-data", no_argument, 0, 'd'},
        {"help",           no_argument, 0, 'h'},
        {"show_state",     no_argument, 0, 's'},
        {"validate",       no_argument, 0, 'v'},
        {0, 0, 0, 0}
    };

} /* namespace amqpAnalyze */
