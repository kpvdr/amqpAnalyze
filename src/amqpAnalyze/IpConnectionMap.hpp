/*
 * IpConnectionMap.hpp
 *
 *  Created on: Apr 2, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_IPCONNECTIONMAP_HPP_
#define SRC_AMQPANALYZE_IPCONNECTIONMAP_HPP_

#include <amqpAnalyze/FwdDecls.hpp>
#include <map>
#include <deque>

namespace amqpAnalyze
{

    class IpConnectionMap {
    protected:
        std::map<std::size_t, std::deque<const Packet*>*> _connectionMap;
        std::map<std::string, std::size_t> _connectionIndex;
    public:
        IpConnectionMap();
        virtual ~IpConnectionMap();
        void addPacket(const Packet* packetPtr);
        uint32_t getIpHashList(std::deque<std::size_t>& list) const;
        uint32_t getIpConnectionIndexList(std::deque<std::pair<std::string, std::size_t>>& list) const;
        const std::deque<const Packet*>* getPacketList(size_t hash) const;
        static std::size_t hash(std::string& connectionString);
    };

} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_IPCONNECTIONMAP_HPP_ */
