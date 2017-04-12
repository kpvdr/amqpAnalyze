/*
 * Frame.cpp
 *
 *  Created on: Apr 3, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/amqp10/FrameBase.hpp>

#include <sstream>

namespace amqpAnalyze
{
    namespace amqp10
    {

        FrameBase::FrameBase(std::size_t frameOffset):
                _frameOffset(frameOffset)
        {}

        FrameBase::~FrameBase() {}

        std::string FrameBase::toString(std::size_t margin, bool ignoreFirstMargin) {
            std::ostringstream oss;
            appendString(oss, margin, ignoreFirstMargin);
            return oss.str();
        }

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */
