/*
 * ConnectionHandler.hpp
 *
 *  Created on: Apr 24, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_AMQP10_CONNECTIONHANDLER_HPP_
#define SRC_AMQPANALYZE_AMQP10_CONNECTIONHANDLER_HPP_

#include <amqpAnalyze/amqp10/FwdDecls.hpp>

namespace amqpAnalyze
{
    namespace amqp10
    {

        class ConnectionHandler
        {
        public:
            ConnectionHandler();
            virtual ~ConnectionHandler();

            void handleFrame(const struct TcpAddressInfo& tcpAddrInfo, AmqpBlock* blockPtr);
            void tcpClose(const struct TcpAddressInfo& tcpAddrInfo);

        protected:
            std::map<uint64_t, Connection*> _connectionMap;

            Connection* insertIfNotPresent(const struct TcpAddressInfo& tcpAddrInfo);
        };

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

// Global AMQP connection handler
extern amqpAnalyze::amqp10::ConnectionHandler g_amqpConnectionHandler;

#endif /* SRC_AMQPANALYZE_AMQP10_CONNECTIONHANDLER_HPP_ */
