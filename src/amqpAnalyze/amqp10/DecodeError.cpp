/*
 * DecodeError.cpp
 *
 *  Created on: Mar 30, 2017
 *      Author: kpvdr
 */

#include "DecodeError.hpp"

#include <amqpAnalyze/amqp10/FrameBuffer.hpp>

namespace amqpAnalyze
{
    namespace amqp10
    {

        DecodeError::DecodeError(const amqp10::FrameBuffer& frameBuffer, const std::string& errorMessage):
            Error(errorMessage),
            _packetNum(frameBuffer.getPacketNum()),
            _amqpDataOffset(frameBuffer.getFrameOffsetSnapshot())
        {}

        DecodeError::DecodeError(uint64_t packetNum, std::size_t amqpDataOffset, const std::string& errorMessage):
            Error(errorMessage),
            _packetNum(packetNum),
            _amqpDataOffset(amqpDataOffset)
        {}

        DecodeError::DecodeError(ErrorSeverity_t errorSeverity, const amqp10::FrameBuffer& frameBuffer, const std::string& errorMessage):
            Error(errorSeverity, errorMessage),
            _packetNum(frameBuffer.getPacketNum()),
            _amqpDataOffset(frameBuffer.getFrameOffsetSnapshot())
        {}

        DecodeError::DecodeError(ErrorSeverity_t errorSeverity, uint64_t packetNum, std::size_t amqpDataOffset, const std::string& errorMessage):
            Error(errorSeverity, errorMessage),
            _packetNum(packetNum),
            _amqpDataOffset(amqpDataOffset)
        {}

        DecodeError::~DecodeError() {}
        uint64_t DecodeError::packetNum() const { return _packetNum; }
        std::size_t DecodeError::amqpDataOffset() const { return _amqpDataOffset; }
        const char* DecodeError::typeStr() const {
            return "decode: ";
        }

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */
