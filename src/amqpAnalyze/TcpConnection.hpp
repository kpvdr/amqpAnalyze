/*
 * TcpConnection.hpp
 *
 *  Created on: May 6, 2017
 *      Author: kvdr
 */

#ifndef SRC_AMQPANALYZE_TCPCONNECTION_HPP_
#define SRC_AMQPANALYZE_TCPCONNECTION_HPP_

#include <cstdint>
#include <string>

namespace amqpAnalyze
{
    enum class AmqpVersions_t;
    class TcpDissector;

	class TcpConnection {
	public:
        TcpConnection(const TcpDissector* tcpDissectorPtr, uint32_t initSrcSequence, uint32_t connectionIndex, uint64_t packetNumber);
        virtual ~TcpConnection();

        inline const std::string& destAddress() const { return _destAddrStr; }
        inline std::size_t hash() const { return _hash; }
        inline const std::string& srcAddress() const { return _srcAddrStr; }
        inline uint32_t connectionIndex() const { return _connectionIndex; }
        inline uint64_t firstPacketNumber() const { return _firstPacketNumber; }
        inline uint32_t initDestSequence() const { return _initDestSequence; }
        inline uint32_t initSrcSequence() const { return _initSrcSequence; }
        inline uint64_t lastPacketNumber() const { return _lastPacketNumber; }
        inline void setDestFinFlag() { _destFinFlag = true; }
        inline void setInitDestSequence(uint32_t initDestSequence) { _initDestSequence = initDestSequence; }
        inline void setSrcFinFlag() { _srcFinFlag = true; }
        inline void setLastPacketNumber(uint64_t lastPacketNumber) { _lastPacketNumber = lastPacketNumber; }
        inline AmqpVersions_t amqpVersion() const { return _amqpVersion; }
        inline void setAmqpVersion(AmqpVersions_t amqpVersion) { _amqpVersion = amqpVersion; }

	protected:
        const std::string _destAddrStr;
        const std::size_t _hash;
        const std::string _srcAddrStr;
        uint32_t _initSrcSequence;
        uint32_t _initDestSequence;
        bool _srcFinFlag;
        bool _destFinFlag;
        uint32_t _connectionIndex;
        uint64_t _firstPacketNumber;
        uint64_t _lastPacketNumber;
        AmqpVersions_t _amqpVersion;
	};

} /* namespace amqpAnalyze */

std::ostream& operator<<(std::ostream& o, const amqpAnalyze::TcpConnection& t);
std::ostream& operator<<(std::ostream& o, const amqpAnalyze::TcpConnection* t);

#endif /* SRC_AMQPANALYZE_TCPCONNECTION_HPP_ */
