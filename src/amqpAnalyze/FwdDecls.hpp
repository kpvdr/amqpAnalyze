/*
 * FwdDecls.hpp
 *
 *  Created on: Apr 24, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_FWDDECLS_HPP_
#define SRC_AMQPANALYZE_FWDDECLS_HPP_

#include <cstdint>
#include <deque>
#include <string>
#include <vector>

namespace amqpAnalyze
{

    class Dissector;
    typedef std::deque<const Dissector*> DissectorList_t;
    typedef DissectorList_t::iterator DissectorList_itr_t;
    typedef DissectorList_t::const_iterator DissectorList_citr_t;

    class Error;
    typedef std::vector<const Error*> ErrorPtrList_t;
    typedef ErrorPtrList_t::iterator ErrorPtrListItr_t;
    typedef ErrorPtrList_t::const_iterator ErrorPtrListCitr_t;

    class Options;

    class Packet;

    struct TcpAddressInfo;

    class TcpDissector;

} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_FWDDECLS_HPP_ */
