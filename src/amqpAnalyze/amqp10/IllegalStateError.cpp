/*
 * IllegalStateError.cpp
 *
 *  Created on: Apr 24, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/amqp10/IllegalStateError.hpp>

namespace amqpAnalyze
{
    namespace amqp10
    {

        IllegalStateError::IllegalStateError(const char* type, const std::string& name, const char* action, const char* current_state):
            Error(MSG("Illegal state transition: " << type << ":" << name << ": action=" << action << " current_state=" << current_state))
        {}

        IllegalStateError::IllegalStateError(ErrorSeverity_t errorSeverity, const char* type, const std::string& name, const char* action, const char* current_state):
            Error(errorSeverity, MSG("Illegal state transition: " << type << ":" << name << ": action=" << action << " current_state=" << current_state))
        {}

        IllegalStateError::~IllegalStateError() {}

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */
