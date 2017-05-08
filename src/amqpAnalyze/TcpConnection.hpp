/*
 * TcpConnection.hpp
 *
 *  Created on: May 6, 2017
 *      Author: kvdr
 */

#ifndef SRC_AMQPANALYZE_TCPCONNECTION_HPP_
#define SRC_AMQPANALYZE_TCPCONNECTION_HPP_

#include "TcpAddressInfo.hpp"

namespace amqpAnalyze
{

	class TcpConnection: public TcpAddressInfo {
	public:
        TcpConnection(const TcpAddressInfo& tcpAddressInfo, uint32_t initSrcSequence, uint32_t connectionIndex, uint64_t packetNumber);
        virtual ~TcpConnection();

        inline uint32_t connectionIndex() const { return _connectionIndex; }
        inline uint64_t firstPacketNumber() const { return _firstPacketNumber; }
        inline uint32_t initDestSequence() const { return _initDestSequence; }
        inline uint32_t initSrcSequence() const { return _initSrcSequence; }
        inline uint64_t lastPacketNumber() const { return _lastPacketNumber; }
        inline void setDestFinFlag() { _destFinFlag = true; }
        inline void setInitDestSequence(uint32_t initDestSequence) { _initDestSequence = initDestSequence; }
        inline void setSrcFinFlag() { _srcFinFlag = true; }
        inline void setLastPacketNumber(uint64_t lastPacketNumber) { _lastPacketNumber = lastPacketNumber; }

	protected:
        uint32_t _initSrcSequence;
        uint32_t _initDestSequence;
        bool _srcFinFlag;
        bool _destFinFlag;
        uint32_t _connectionIndex;
        uint64_t _firstPacketNumber;
        uint64_t _lastPacketNumber;
	};

} /* namespace amqpAnalyze */

std::ostream& operator<<(std::ostream& o, const amqpAnalyze::TcpConnection& t);
std::ostream& operator<<(std::ostream& o, const amqpAnalyze::TcpConnection* t);

#endif /* SRC_AMQPANALYZE_TCPCONNECTION_HPP_ */
