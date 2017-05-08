/*
 * Options.cpp
 *
 *  Created on: Apr 21, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/Options.hpp>

#include <amqpAnalyze/Error.hpp>
#include <cstring>
#include <iostream>
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
        char* fromVal = nullptr;
        char* numPacketsVal = nullptr;
        char* toVal = nullptr;
        while (true) {
            int option_index = 0;
            opterr = 0; // disable printing of error message from within getopt_long()
            int c = ::getopt_long(argc, argv, "acC:df:hn:Nst:v", s_longOptions, &option_index);
            if (c == -1) break;
            switch(c) {
            case 'a':
                s_amqpFlag = true;
                break;
            case 'c':
                s_colorFlag = true;
                break;
            case 'C':
                try { handleConnectionIndex(optarg); }
                catch (const std::invalid_argument& e) {
                    handleException(basename(argv[0]), MSG("Error: invalid -C/--connection-index option value: \"" << optarg << "\""));
                }
                break;
            case 'd':
                s_showAmqpDataFlag = true;
                break;
            case 'f':
                fromVal = optarg;
                break;
            case 'h':
                handleException(basename(argv[0]), "");
            case 'n':
                numPacketsVal = optarg;
                break;
            case 'N':
                s_showNullArgs = true;
                break;
            case 's':
                s_showStateFlag = true;
                break;
            case 't':
                toVal = optarg;
                break;
            case 'v':
                s_validateFlag = true;
                break;
            default:
                printHelp(basename(argv[0]));
                if (optopt != 0) handleException(basename(argv[0]), MSG("Error: Invalid option \'" << (char)optopt << "\'"));
                handleException(basename(argv[0]), MSG("Error: Invalid option \"" << argv[optind-1] << "\""));
            }
        }
        // Check file name is present and valid
        if (argc == optind) { // No file name
            handleException(basename(argv[0]), MSG("Error: No pcapng file specified"));
        }
        if (argc - optind > 1) { // More than one file name
            std::ostringstream oss;
            oss << "Error: Only one file may be specified. The following were found:\n";
            while (optind < argc)
                oss << "  " << argv[optind++] << "\n";
            handleException(basename(argv[0]), oss.str());
        }
        struct stat st;
        int res = stat(argv[optind], &st);
        if (res) {
            handleException(basename(argv[0]),
                            MSG("Error reading file \"" << argv[optind] << "\": " << std::strerror(errno)));
        }
        if (S_ISREG(st.st_mode) == 0) handleException(basename(argv[0]), MSG("Error: File \"" << argv[optind] << ": not regular file"));
        s_fileName = argv[optind];

        // Check from/num/to packet combinations. Process values.
        if (numPacketsVal != nullptr && toVal != nullptr) {
            handleException(basename(argv[0]),
                            MSG("Error: Both -n/--num-packets and -t/--to-packet options used together but are mutually exclusive"));
        }
        try {
            if (fromVal != nullptr) s_fromPacket = std::stoull(fromVal);
        } catch (const std::invalid_argument& e) {
            handleException(basename(argv[0]),
                            MSG("Error: -f/--from-packet option invalid: \"" << fromVal << "\""));
        }
        try {
            if (numPacketsVal != nullptr) {
                const uint32_t numPackets(std::stoull(numPacketsVal));
                if (numPackets == 0) {
                    handleException(basename(argv[0]),
                                    MSG("Error: -n/--num-packets option must have value > 0"));
                }
                s_toPacket = s_fromPacket + std::stoull(numPacketsVal) - 1;
            }
        } catch (const std::invalid_argument& e) {
            handleException(basename(argv[0]),
                            MSG("Error: -n/--num-packets option invalid: \"" << numPacketsVal << "\""));
        }
        try {
            if (toVal != nullptr) s_toPacket = std::stoull(toVal);
        } catch (const std::invalid_argument& e) {
            handleException(basename(argv[0]),
                            MSG("Error: -t/--to-packet option invalid: \"" << toVal << "\""));
        }
        if (s_fromPacket > s_toPacket) {
            handleException(basename(argv[0]),
                            MSG("Error: -f/--from-packet option > -t/--to-packet option (from=" << s_fromPacket << " > to=" << s_toPacket << ")"));
        }
    }

    Options::~Options() {}

    void Options::handleConnectionIndex(const char* optarg) {
        uint32_t n(std::stoul(optarg));
        s_connectionIndexSet.emplace(n);
    }

    void Options::handleException(const char* baseName, const std::string& errMsg) {
        printHelp(baseName);
        throw amqpAnalyze::Error(errMsg);
    }

    void Options::printHelp(const char* baseName) {
        std::cout << "Usage: " << baseName << " [options] FILE\n"
                  << "options: -h --help               Print help\n"
                  << "         -a --amqp               Show only AMQP packets\n"
                  << "         -c --color              Use ANSI color\n"
                  << "         -C --connection-index N Show packets from connection index N. May be used multiple\n"
                  << "                                 times to select multiple connections\n"
                  << "         -d --show-amqp-data     Show hex dump of AMQP data for each packet\n"
                  << "         -f --from-packet N      Show packets starting at N\n"
                  << "         -n --num-packets N      Show N packets. Can be used with option -f/--from-packet.\n"
                  << "                                 Cannot be used together with option -t/--to-packet.\n"
                  << "         -N --show-null-args     Show null parameters and headers"
                  << "         -s --show-state         Show connection, session and link state\n"
                  << "         -t --to-packet N        Show up to packet N. Can be used with option\n"
                  << "                                 -f/--from-packet. Cannot be used with option -n/--num-packets\n"
                  << "         -v --validate           Validate AMQP data to AMQP 1.0 specification\n"
                  << "   FILE: pcapng file to be analyzed" << std::endl;
    }


    // static
    bool Options::s_amqpFlag = false;
    bool Options::s_colorFlag = false;
    std::set<uint32_t> Options::s_connectionIndexSet;
    std::string Options::s_fileName;
    uint64_t Options::s_fromPacket = 0;
    uint32_t Options::s_maxDisplaySize = 50;
    bool Options::s_showAmqpDataFlag = false;
    bool Options::s_showStateFlag = false;
    bool Options::s_showNullArgs = false;
    uint64_t Options::s_toPacket = UINT64_MAX;
    bool Options::s_validateFlag = false;

    struct option Options::s_longOptions[] = {
        {"help",             no_argument,       0, 'h'},
        {"amqp",             no_argument,       0, 'a'},
        {"color",            no_argument,       0, 'c'},
        {"connection-index", required_argument, 0, 'C'},
        {"show-amqp-data",   no_argument,       0, 'd'},
        {"from-packet",      required_argument, 0, 'f'},
        {"num-packets",      required_argument, 0, 'n'},
        {"show-state",       no_argument,       0, 's'},
        {"suppress-nulls",   no_argument,       0, 'N'},
        {"to-packet",        required_argument, 0, 't'},
        {"validate",         no_argument,       0, 'v'},
        {0, 0, 0, 0}
    };

} /* namespace amqpAnalyze */
