/*
 * ProvidesRequires.hpp
 *
 *  Created on: Apr 17, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_AMQP10_PROVIDESREQUIRES_HPP_
#define SRC_AMQPANALYZE_AMQP10_PROVIDESREQUIRES_HPP_

#include <map>
#include <vector>

namespace amqpAnalyze
{
    namespace amqp10
    {

        enum class amqpRequiresProvides_t {
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
        typedef std::vector<amqpRequiresProvides_t> amqp_provides_requires_list_t;
        typedef amqp_provides_requires_list_t::const_iterator amqp_provides_requires_list_citr_t;

        extern std::map<amqpRequiresProvides_t, const char*> ProvidesRequiresNames;


    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQP10_PROVIDESREQUIRES_HPP_ */
