/*
 * TcpAddressInfo.hpp
 *
 *  Created on: Apr 26, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_TCPADDRESSINFO_HPP_
#define SRC_AMQPANALYZE_TCPADDRESSINFO_HPP_

#include <amqpAnalyze/FwdDecls.hpp>

namespace amqpAnalyze
{

    struct TcpAddressInfo
    {
        std::string _srcAddrStr;
        std::string _destAddrStr;
        std::size_t _hash;
        TcpAddressInfo();
        TcpAddressInfo(const TcpAddressInfo& other);
        void setAddress(const TcpDissector* tcpDissectorPtr);
    };

    std::ostream& operator<<(std::ostream& o, const TcpAddressInfo& t);
    std::ostream& operator<<(std::ostream& o, const TcpAddressInfo* t);



    struct TcpConnection: public TcpAddressInfo
    {
        uint32_t _initSrcSequence;
        uint32_t _initDestSequence;
        bool _srcFinFlag;
        bool _destFinFlag;
        TcpConnection(const TcpAddressInfo& tcpAddressInfo, uint32_t initSrcSequence);
        void setInitDestSequence(uint32_t initDestSequence);
        inline uint32_t getRelSrcSequence(uint32_t currSrcSequence) { return currSrcSequence - _initSrcSequence; }
        inline uint32_t getRelDestSequence(uint32_t currDestSequence) { return currDestSequence - _initDestSequence; }
    };

    std::ostream& operator<<(std::ostream& o, const TcpConnection& t);
    std::ostream& operator<<(std::ostream& o, const TcpConnection* t);

} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_TCPADDRESSINFO_HPP_ */
