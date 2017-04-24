/*
 * FrameError.cpp
 *
 *  Created on: Apr 19, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/amqp10/FrameError.hpp>

#include <iomanip>
#include <std/AnsiTermColors.hpp>

namespace amqpAnalyze
{
    namespace amqp10
    {

        FrameError::FrameError(uint64_t packetNum, std::size_t dataOffset, const amqpAnalyze::Error& error):
            AmqpBlock(packetNum, dataOffset),
            _errMsg(error.what())
        {}

        FrameError::~FrameError() {}

        std::ostringstream& FrameError::appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin, bool colorFlag) const {
            if (!ignoreFirstMargin) oss << "\n" << std::string(margin, ' ');
            oss << "[" << std::setw(4) << std::setfill('0') << std::hex << _dataOffset  << "] e ";
            oss << COLOR(FGND_BRED, _errMsg, colorFlag);
            return appendStringEpilog(oss, margin, colorFlag);
        }

        void FrameError::validate() {}

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */
