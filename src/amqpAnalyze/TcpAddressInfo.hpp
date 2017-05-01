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
        TcpAddressInfo(const TcpDissector* tcpDissectorPtr);
        TcpAddressInfo(const std::string& srcAddrStr, const std::string& destAddrStr, std::size_t hash);
        void setAddress(const TcpDissector* tcpDissectorPtr);
    };

    std::ostream& operator<<(std::ostream& o, const TcpAddressInfo& t);
    std::ostream& operator<<(std::ostream& o, const TcpAddressInfo* t);

} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_TCPADDRESSINFO_HPP_ */
