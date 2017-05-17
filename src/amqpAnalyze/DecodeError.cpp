/*
 * DecodeError.cpp
 *
 *  Created on: Mar 30, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/amqp10/FrameBuffer10.hpp>
#include <amqpAnalyze/DecodeError.hpp>

namespace amqpAnalyze
{

    DecodeError::DecodeError(const FrameBuffer& frameBuffer, const std::string& errorMessage):
        Error(errorMessage),
        _packetNum(frameBuffer.getPacketNum()),
        _amqpDataOffset(frameBuffer.getFrameOffsetSnapshot())
    {}

    DecodeError::DecodeError(uint64_t packetNum, std::size_t amqpDataOffset, const std::string& errorMessage):
        Error(errorMessage),
        _packetNum(packetNum),
        _amqpDataOffset(amqpDataOffset)
    {}

    DecodeError::DecodeError(ErrorSeverity_t errorSeverity, const FrameBuffer& frameBuffer, const std::string& errorMessage):
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

} /* namespace amqpAnalyze */
