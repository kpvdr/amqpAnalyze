/*
 * ConnectionError.hpp
 *
 *  Created on: May 9, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_AMQP10_CONNECTIONERROR_HPP_
#define SRC_AMQPANALYZE_AMQP10_CONNECTIONERROR_HPP_

#include <amqpAnalyze/Error.hpp>

namespace amqpAnalyze
{
    namespace amqp10
    {

        class ConnectionError: public Error
        {
        public:
            ConnectionError(const std::string& err_msg = std::string());
            ConnectionError(ErrorSeverity_t errorSeverity, const std::string& err_msg = std::string());
            virtual ~ConnectionError();
        };

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQP10_CONNECTIONERROR_HPP_ */
