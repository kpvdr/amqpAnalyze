/*
 * ProvidesRequires.cpp
 *
 *  Created on: Apr 17, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/amqp10/ProvidesRequires.hpp>

#include <sstream>

namespace amqpAnalyze
{
    namespace amqp10
    {

        std::map<amqpRequiresProvides_t, const char*> ProvidesRequiresNames = {
            {amqpRequiresProvides_t::ADDRESS, "address"},
            {amqpRequiresProvides_t::DELIVERY_STATE, "delivery-state"},
            {amqpRequiresProvides_t::DISTRIBUTION_MODE, "distribution-mode"},
            {amqpRequiresProvides_t::ERROR_CONDITION, "error-condition"},
            {amqpRequiresProvides_t::GLOBAL_TX_ID, "global-tx-id"},
            {amqpRequiresProvides_t::MESSAGE_ID, "message-id"},
            {amqpRequiresProvides_t::OUTCOME, "outcome"},
            {amqpRequiresProvides_t::SOURCE, "source"},
            {amqpRequiresProvides_t::TARGET, "target"},
            {amqpRequiresProvides_t::TXN_CAPABILITY, "txn-capability"},
            {amqpRequiresProvides_t::TXN_ID, "txn-id"}
        };

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */
