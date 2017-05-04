/*
 * TcpConnectionMap.hpp
 *
 *  Created on: May 3, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_TCPCONNECTIONMAP_HPP_
#define SRC_AMQPANALYZE_TCPCONNECTIONMAP_HPP_

#include <amqpAnalyze/TcpAddressInfo.hpp>
#include <map>
#include <vector>

namespace amqpAnalyze
{
    struct TcpConnection;

    typedef std::map<std::size_t, struct TcpConnection*> ConnectionMap_t;
    typedef ConnectionMap_t::iterator ConnectionMap_itr_t;

    typedef std::vector<std::size_t> ConnectionHashList_t;
    typedef ConnectionHashList_t::const_iterator ConnectionHashList_citr_t;

    class TcpConnectionMap
    {
    public:
        TcpConnectionMap();
        virtual ~TcpConnectionMap();

        void handleTcpSyn(const TcpAddressInfo& tcpAddressInfo, uint32_t sequence);
        void handleTcpFin(const TcpAddressInfo& tcpAddressInfo);
        bool hasConnection(std::size_t hash) const;
        void print(std::ostream& os) const;

    protected:
        ConnectionMap_t _connectionMap;
        ConnectionHashList_t _connectionList;
    };

} /* namespace amqpAnalyze */

// Global TCP connection map
extern amqpAnalyze::TcpConnectionMap g_tcpConnectionMap;

#endif /* SRC_AMQPANALYZE_TCPCONNECTIONMAP_HPP_ */
