/*
 * IpConnectionMap.cpp
 *
 *  Created on: Apr 2, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/IpConnectionMap.hpp>

#include <amqpAnalyze/Packet.hpp>

namespace amqpAnalyze {

IpConnectionMap::IpConnectionMap(): _connectionMap() {}

IpConnectionMap::~IpConnectionMap() {
    for (std::map<std::size_t, std::deque<const Packet*>*>::iterator i=_connectionMap.begin(); i!=_connectionMap.end(); ++i) {
        delete i->second;
    }
    _connectionMap.clear();
    _connectionIndex.clear();
}

void IpConnectionMap::addPacket(const Packet* packetPtr) {
    std::string connectionString = packetPtr->connectionIndex();
    std::size_t connectionHash = IpConnectionMap::hash(connectionString);
    std::deque<const Packet*>* listPtr = nullptr;
    try {
        listPtr = _connectionMap.at(connectionHash);
    } catch (const std::out_of_range& e) {
        listPtr = new std::deque<const Packet*>();
        _connectionMap[connectionHash] = listPtr;
        _connectionIndex[connectionString] = connectionHash;
    }
    listPtr->push_back(packetPtr);
}

uint32_t IpConnectionMap::getIpHashList(std::deque<std::size_t>& list) const {
    uint32_t count = 0;
    for (std::map<std::size_t, std::deque<const Packet*>*>::const_iterator i=_connectionMap.cbegin(); i!=_connectionMap.cend(); ++i) {
        list.push_back(i->first);
        ++count;
    }
    return count;
}

uint32_t IpConnectionMap::getIpConnectionIndexList(std::deque<std::pair<std::string, std::size_t>>& list) const {
    uint32_t count = 0;
    for (std::map<std::string, std::size_t>::const_iterator i=_connectionIndex.cbegin(); i!=_connectionIndex.cend(); ++i) {
        list.push_back(*i);
        ++count;
    }
    return count;
}

const std::deque<const Packet*>* IpConnectionMap::getPacketList(size_t hash) const {
	return _connectionMap.at(hash);
}

// static
std::size_t IpConnectionMap::hash(std::string& connectionString) {
    std::hash<std::string> hashFn;
    return hashFn(connectionString);
}

} /* namespace amqpAnalyze */
