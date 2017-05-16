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
        class SessionHanlder;

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

            void handleFrame(const TcpConnection* tcpConnectionPtr, bool replyFlag, Frame* framePtr);
            void handleProtocolHeader(const TcpConnection* tcpConnectionPtr, bool replyFlag, ProtocolHeader* protocolHeaderPtr);
            bool handleTcpClose(const TcpConnection* tcpConnectionPtr, bool replyFlag);

        protected:
            std::vector<amqpAnalyze::Error*> _errorList;
            std::string _closeError;
            //bool _frameErrorFlag;

            std::string _initiatorAddrStr;
            //std::string _initiatorCloseError;
            std::map<uint16_t, SessionHanlder*> _initiatorEndpoints;
            //bool _initiatorHeaderSent;
            const AmqpOpen* _initiatorOpen;
            ConnectionState _initiatorState;
            bool _initiatorTcpClosedFlag;

            std::string _responderAddrStr;
            //std::string _responderCloseError;
            std::map<uint16_t, SessionHanlder*> _responderEndpoints;
            //bool _responderHeaderSent;
            const AmqpOpen* _responderOpen;
            ConnectionState _responderState;
            bool _responderTcpClosedFlag;

            LastActorType_t _lastActor;

            void checkChannel(const Frame* framePtr);
            bool checkError(const AmqpClose* amqpClosePtr);
            bool isInitiator(const TcpConnection* tcpConnectionPtr, bool replyFlag) const;
            bool isResponder(const TcpConnection* tcpConnectionPtr, bool replyFlag) const;
        };

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQP10_CONNECTION_HPP_ */
