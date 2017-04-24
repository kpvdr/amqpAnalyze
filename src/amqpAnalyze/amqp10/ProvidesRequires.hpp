/*
 * ProvidesRequires.hpp
 *
 *  Created on: Apr 17, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_AMQP10_PROVIDESREQUIRES_HPP_
#define SRC_AMQPANALYZE_AMQP10_PROVIDESREQUIRES_HPP_

#include <map>

namespace amqpAnalyze
{
    namespace amqp10
    {

        enum class AmqpRequiresProvides_t {
            ADDRESS = 1,
            DELIVERY_STATE,
            DISTRIBUTION_MODE,
            ERROR_CONDITION,
            GLOBAL_TX_ID,
            MESSAGE_ID,
            OUTCOME,
            SOURCE,
            TARGET,
            TXN_CAPABILITY,
            TXN_ID
        };

        extern std::map<AmqpRequiresProvides_t, const char*> providesRequiresNames;


    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQP10_PROVIDESREQUIRES_HPP_ */
