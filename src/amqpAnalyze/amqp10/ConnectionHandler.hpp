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
    class TcpConnection;
    class TcpDissector;

    namespace amqp10
    {
        class Connection;

        class ConnectionHandler
        {
        public:
            ConnectionHandler();
            virtual ~ConnectionHandler();

            void handleFrame(const TcpConnection* tcpConnectionPtr, bool replyFlag, AmqpBlock* blockPtr);
            void handleTcpClose(TcpDissector* tcpDissectorPtr);

        protected:
            std::map<uint64_t, Connection*> _connectionMap;

            Connection* insertIfNotPresent(const TcpConnection* tcpConnectionPtr);
        };

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

// Global AMQP connection handler
extern amqpAnalyze::amqp10::ConnectionHandler g_amqpConnectionHandler;

#endif /* SRC_AMQPANALYZE_AMQP10_CONNECTIONHANDLER_HPP_ */
