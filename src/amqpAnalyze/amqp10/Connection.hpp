/*
 * Connection.hpp
 *
 *  Created on: Apr 24, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_AMQP10_CONNECTION_HPP_
#define SRC_AMQPANALYZE_AMQP10_CONNECTION_HPP_

#include <amqpAnalyze/amqp10/ConnectionState.hpp>
#include <amqpAnalyze/amqp10/FwdDecls.hpp>

namespace amqpAnalyze
{
    class TcpConnection;

    namespace amqp10
    {
        class AmqpClose;
        class AmqpOpen;
        class Frame;
        class Session;

        typedef std::map<uint16_t, Session*> SessionMap_t;
        typedef SessionMap_t::iterator SessionMapItr_t;

        enum class LastActorType_t {
            NONE,
            INITIATOR,
            RECEIVER
        };

        class Connection
        {
        public:
            Connection(const TcpConnection* tcpConnectionPtr);
            virtual ~Connection();

            void handleFrame(TcpConnection* tcpConnectionPtr, bool replyFlag, Frame* framePtr);
            void handleProtocolHeader(const TcpConnection* tcpConnectionPtr, bool replyFlag, ProtocolHeader* protocolHeaderPtr);
            bool handleTcpClose(const TcpConnection* tcpConnectionPtr, bool replyFlag);

        protected:
            std::vector<amqpAnalyze::Error*> _errorList;
            std::string _closeError;

            std::string _initiatorAddrStr;
            const AmqpOpen* _initiatorOpen;
            ConnectionState _initiatorState;
            bool _initiatorTcpClosedFlag;
            SessionMap_t _initiatorSessionMap;

            std::string _responderAddrStr;
            const AmqpOpen* _responderOpen;
            ConnectionState _responderState;
            bool _responderTcpClosedFlag;
            SessionMap_t _responderSessionMap;

            LastActorType_t _lastActor;

            uint16_t checkChannel(const Frame* framePtr);
            bool checkError(const AmqpClose* amqpClosePtr);
            bool isInitiator(const TcpConnection* tcpConnectionPtr, bool replyFlag) const;
            static Session* insertIfNotPresent(SessionMap_t& sessionMap, uint16_t channel, bool initiatorFlag);
        };

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQP10_CONNECTION_HPP_ */
