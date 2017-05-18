/*
 * Session.hpp
 *
 *  Created on: May 11, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_AMQP10_SESSION_HPP_
#define SRC_AMQPANALYZE_AMQP10_SESSION_HPP_

#include <amqpAnalyze/amqp10/SessionState.hpp>
#include <cstdint>

namespace amqpAnalyze
{
    class TcpConnection;

    namespace amqp10
    {
        class AmqpEnd;
        class Frame;

        class Session
        {
        public:
            Session(uint16_t channel, bool initiatorFlag);
            virtual ~Session();

            void handleFrame(TcpConnection* tcpConnectionPtr, Frame* framePtr, bool initiatorFlag);

        protected:
            const bool _initiatorFlag;
            const uint16_t _channel;
            SessionState _sessionState;
            std::string _endError;
            bool checkError(const AmqpEnd* amqpEndPtr);
        };

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQP10_SESSION_HPP_ */
