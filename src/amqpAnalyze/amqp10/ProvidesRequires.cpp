/*
 * ProvidesRequires.cpp
 *
 *  Created on: Apr 17, 2017
 *      Author: kpvdr
 */

#include "ProvidesRequires.hpp"

#include <sstream>

namespace amqpAnalyze
{
    namespace amqp10
    {

        std::map<AmqpRequiresProvides_t, const char*> providesRequiresNames = {
            {AmqpRequiresProvides_t::ADDRESS, "address"},
            {AmqpRequiresProvides_t::DELIVERY_STATE, "delivery-state"},
            {AmqpRequiresProvides_t::DISTRIBUTION_MODE, "distribution-mode"},
            {AmqpRequiresProvides_t::ERROR_CONDITION, "error-condition"},
            {AmqpRequiresProvides_t::GLOBAL_TX_ID, "global-tx-id"},
            {AmqpRequiresProvides_t::MESSAGE_ID, "message-id"},
            {AmqpRequiresProvides_t::OUTCOME, "outcome"},
            {AmqpRequiresProvides_t::SOURCE, "source"},
            {AmqpRequiresProvides_t::TARGET, "target"},
            {AmqpRequiresProvides_t::TXN_CAPABILITY, "txn-capability"},
            {AmqpRequiresProvides_t::TXN_ID, "txn-id"}
        };

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */
