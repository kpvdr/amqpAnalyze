/*
 * SessionStateHandler.hpp
 *
 *  Created on: May 8, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_AMQP10_SESSIONHANDLER_HPP_
#define SRC_AMQPANALYZE_AMQP10_SESSIONHANDLER_HPP_

namespace amqpAnalyze
{
    class TcpAddressInfo;
    class TcpDissector;

    namespace amqp10
    {
        class AmqpBegin;
        class AmqpEnd;
        class Frame;

        enum class EndpointDir_t {
            INBOUND,
            OUTBOUND
        };
        class SessionHandler
        {
        public:
            SessionHandler();
            virtual ~SessionHandler();

            void handleBegin(const TcpAddressInfo& tcpAddrInfo, AmqpBegin* beginPtr, EndpointDir_t endpointDir);
            void handleEnd(const TcpAddressInfo& tcpAddrInfo, AmqpEnd* endPtr, EndpointDir_t endpointDir);
            static void handleFrame(const TcpAddressInfo& tcpAddrInfo, Frame* framePtr);
        };

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQP10_SESSIONHANDLER_HPP_ */
