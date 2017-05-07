/*
 * ValidationError.cpp
 *
 *  Created on: Mar 30, 2017
 *      Author: kpvdr
 */

#include "ValidationError.hpp"

#include <amqpAnalyze/amqp10/FrameBuffer.hpp>

namespace amqpAnalyze
{
    namespace amqp10
    {

        ValidationError::ValidationError(const amqp10::FrameBuffer& frameBuffer, const std::string& errorMessage):
            DecodeError(ErrorSeverity_t::WARNING, frameBuffer, errorMessage)
        {}
        ValidationError::ValidationError(uint64_t packetNum, std::size_t amqpDataOffset, const std::string& errorMessage):
            DecodeError(ErrorSeverity_t::WARNING, packetNum, amqpDataOffset, errorMessage)
        {}
        ValidationError::ValidationError(ErrorSeverity_t errorSeverity, const amqp10::FrameBuffer& frameBuffer, const std::string& errorMessage):
            DecodeError(errorSeverity, frameBuffer, errorMessage)
        {}
        ValidationError::ValidationError(ErrorSeverity_t errorSeverity, uint64_t packetNum, std::size_t amqpDataOffset, const std::string& errorMessage):
            DecodeError(errorSeverity, packetNum, amqpDataOffset, errorMessage)
        {}
        ValidationError::~ValidationError() {}
        const char* ValidationError::typeStr() const {
            return "validation: ";
        }

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */
