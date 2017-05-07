/*
 * ValidationError.hpp
 *
 *  Created on: Mar 30, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_AMQP10_VALIDATIONERROR_HPP_
#define SRC_AMQPANALYZE_AMQP10_VALIDATIONERROR_HPP_

#include "DecodeError.hpp"

namespace amqpAnalyze
{
    namespace amqp10
    {
        class FrameBuffer;

        class ValidationError: public DecodeError
        {
        public:
            explicit ValidationError(const amqp10::FrameBuffer& frameBuffer, const std::string& errorMessage);
            explicit ValidationError(uint64_t packetNum, std::size_t amqpDataOffset, const std::string& errorMessage);
            explicit ValidationError(ErrorSeverity_t errorSeverity, const amqp10::FrameBuffer& frameBuffer, const std::string& errorMessage);
            explicit ValidationError(ErrorSeverity_t errorSeverity, uint64_t packetNum, std::size_t amqpDataOffset, const std::string& errorMessage);
            virtual ~ValidationError();
            virtual const char* typeStr() const override;
        };

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQP10_VALIDATIONERROR_HPP_ */
