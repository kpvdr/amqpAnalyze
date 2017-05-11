/*
 * SessionStateHandler.cpp
 *
 *  Created on: May 8, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/amqp10/SessionHandler.hpp>

namespace amqpAnalyze
{
    namespace amqp10
    {

        SessionHandler::SessionHandler() {}

        SessionHandler::~SessionHandler() {}

        void SessionHandler::handleBegin(const TcpAddressInfo& tcpAddrInfo, AmqpBegin* beginPtr, EndpointDir_t endpointDir) {}

        void SessionHandler::handleEnd(const TcpAddressInfo& tcpAddrInfo, AmqpEnd* endPtr, EndpointDir_t endpointDir) {}

        // static
        void SessionHandler::handleFrame(const TcpAddressInfo& tcpAddrInfo, Frame* framePtr) {}

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */
