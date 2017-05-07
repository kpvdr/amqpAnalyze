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
    class TcpDissector;

    class TcpAddressInfo
    {
    public:
        TcpAddressInfo();
        TcpAddressInfo(const TcpAddressInfo& other);
        virtual ~TcpAddressInfo();

        inline const std::string& destAddress() const { return _destAddrStr; }
        inline const std::size_t hash() const { return _hash; }
        inline const std::string& srcAddress() const { return _srcAddrStr; }
        void setAddress(const TcpDissector* tcpDissectorPtr);
    protected:
        std::string _destAddrStr;
        std::size_t _hash;
        std::string _srcAddrStr;
   };

} /* namespace amqpAnalyze */

std::ostream& operator<<(std::ostream& o, const amqpAnalyze::TcpAddressInfo& t);
std::ostream& operator<<(std::ostream& o, const amqpAnalyze::TcpAddressInfo* t);

#endif /* SRC_AMQPANALYZE_TCPADDRESSINFO_HPP_ */
