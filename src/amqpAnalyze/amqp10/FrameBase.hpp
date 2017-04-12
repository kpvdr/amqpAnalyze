/*
 * Frame.hpp
 *
 *  Created on: Apr 3, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_AMQP10_FRAMEBASE_HPP_
#define SRC_AMQPANALYZE_AMQP10_FRAMEBASE_HPP_

#include <sstream>

namespace amqpAnalyze
{
    namespace amqp10
    {

        class FrameBase
        {
        protected:
            std::size_t _frameOffset;
        public:
            FrameBase(std::size_t frameOffset);
            virtual ~FrameBase();
            virtual void appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const = 0;
            std::string toString(std::size_t margin, bool ignoreFirstMargin);
            virtual std::size_t decodeSize() const = 0;
        };

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQP10_FRAMEBASE_HPP_ */
