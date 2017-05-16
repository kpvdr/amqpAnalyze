/*
 * ConnectionError.cpp
 *
 *  Created on: May 9, 2017
 *      Author: kpvdr
 */

#include "ConnectionError.hpp"

namespace amqpAnalyze
{
    namespace amqp10
    {

        ConnectionError::ConnectionError(const std::string& err_msg):
            amqpAnalyze::Error(err_msg)
        {}

        ConnectionError::ConnectionError(ErrorSeverity_t errorSeverity, const std::string& err_msg):
            amqpAnalyze::Error(errorSeverity, err_msg)
        {}

        ConnectionError::~ConnectionError() {}

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */
