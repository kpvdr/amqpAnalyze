/*
 * ValidationError.hpp
 *
 *  Created on: Mar 30, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_VALIDATIONERROR_HPP_
#define SRC_AMQPANALYZE_VALIDATIONERROR_HPP_

#include <amqpAnalyze/DecodeError.hpp>

namespace amqpAnalyze
{
    class FrameBuffer;

    class ValidationError: public DecodeError
    {
    public:
        explicit ValidationError(const FrameBuffer& frameBuffer, const std::string& errorMessage);
        explicit ValidationError(uint64_t packetNum, std::size_t amqpDataOffset, const std::string& errorMessage);
        explicit ValidationError(ErrorSeverity_t errorSeverity, const FrameBuffer& frameBuffer, const std::string& errorMessage);
        explicit ValidationError(ErrorSeverity_t errorSeverity, uint64_t packetNum, std::size_t amqpDataOffset, const std::string& errorMessage);
        virtual ~ValidationError();
        virtual const char* typeStr() const override;
    };

} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_VALIDATIONERROR_HPP_ */
