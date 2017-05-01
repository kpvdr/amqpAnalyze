/*
 * FrameError.cpp
 *
 *  Created on: Apr 19, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/amqp10/FrameError.hpp>

#include <amqpAnalyze/Color.hpp>
#include <amqpAnalyze/Options.hpp>
#include <iomanip>

namespace amqpAnalyze
{
    namespace amqp10
    {

        FrameError::FrameError(uint64_t packetNum, std::size_t dataOffset, const amqpAnalyze::Error& error):
            AmqpBlock(packetNum, dataOffset),
            _errMsg(error.what())
        {}

        FrameError::~FrameError() {}

        std::ostringstream& FrameError::appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const {
            if (!ignoreFirstMargin) oss << "\n" << std::string(margin, ' ');
            oss << "[" << std::setw(4) << std::setfill('0') << std::hex << _dataOffset  << "] e ";
            oss << Color::color(DisplayColorType_t::MSG_ERROR, _errMsg);
            return appendStringEpilog(oss, margin);
        }

        void FrameError::validate() {}

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */
