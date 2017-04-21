/*
 * FrameError.hpp
 *
 *  Created on: Apr 19, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_AMQP10_FRAMEERROR_HPP_
#define SRC_AMQPANALYZE_AMQP10_FRAMEERROR_HPP_

#include <amqpAnalyze/amqp10/AmqpBlock.hpp>
#include <sstream>

namespace amqpAnalyze
{
    class Error;

    namespace amqp10
    {

        class FrameError: public AmqpBlock
        {
        public:
            FrameError(uint64_t packetNum, std::size_t dataOffset, const amqpAnalyze::Error& error);
            virtual ~FrameError();

            std::ostringstream& appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const override;
            void validate() override;

        protected:
            std::string _errMsg;
        };

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQP10_FRAMEERROR_HPP_ */
