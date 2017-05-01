/*
 * Endpoint.hpp
 *
 *  Created on: Apr 27, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_AMQP10_ENDPOINT_HPP_
#define SRC_AMQPANALYZE_AMQP10_ENDPOINT_HPP_

#include <cstdint>

namespace amqpAnalyze
{
    namespace amqp10
    {

        enum class EndpointType_t:bool {
            INCOMING = false,
            OUTGOING = true
        };

        class Endpoint
        {
        public:
            Endpoint(EndpointType_t type, uint16_t channel);
            virtual ~Endpoint();

            inline EndpointType_t type() const { return _type; }
            inline uint16_t channel() const { return _channel; }

        protected:
            EndpointType_t _type;
            uint16_t _channel;
        };

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQP10_ENDPOINT_HPP_ */
