/*
 * IllegalStateError.hpp
 *
 *  Created on: Apr 24, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_AMQP10_TRANSPORT_ILLEGALSTATEERROR_HPP_
#define SRC_AMQPANALYZE_AMQP10_TRANSPORT_ILLEGALSTATEERROR_HPP_

#include <amqpAnalyze/Error.hpp>

namespace amqpAnalyze
{
    namespace amqp10
    {

        class IllegalStateError: public Error
        {
        public:
            IllegalStateError(const char* type, const std::string& name, const char* action, const char* current_state);
            IllegalStateError(ErrorSeverity_t errorSeverity, const char* type, const std::string& name, const char* action, const char* current_state);
            virtual ~IllegalStateError();
        };

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQP10_TRANSPORT_ILLEGALSTATEERROR_HPP_ */
