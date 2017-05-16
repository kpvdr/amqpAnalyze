/*
 * SessionStateHandler.cpp
 *
 *  Created on: May 8, 2017
 *      Author: kpvdr
 */

#include "SessionHandler.hpp"

namespace amqpAnalyze
{
    namespace amqp10
    {

        SessionHandler::SessionHandler() {}

        SessionHandler::~SessionHandler() {}

        void SessionHandler::handleBegin(const TcpConnection* tcpConnectionPtr, AmqpBegin* beginPtr, EndpointDir_t endpointDir) {}

        void SessionHandler::handleEnd(const TcpConnection* tcpConnectionPtr, AmqpEnd* endPtr, EndpointDir_t endpointDir) {}

        // static
        void SessionHandler::handleFrame(const TcpConnection* tcpConnectionPtr, Frame* framePtr) {}

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */
