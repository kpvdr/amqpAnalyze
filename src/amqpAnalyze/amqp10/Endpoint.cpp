/*
 * Endpoint.cpp
 *
 *  Created on: Apr 27, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/amqp10/Endpoint.hpp>

namespace amqpAnalyze
{
    namespace amqp10
    {

        Endpoint::Endpoint(EndpointType_t type, uint16_t channel):
            _type(type),
            _channel(channel)
        {}

        Endpoint::~Endpoint() {}

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */
