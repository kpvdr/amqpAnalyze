/*
 * DecodeError.hpp
 *
 *  Created on: Mar 30, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_AMQP10_DECODEERROR_HPP_
#define SRC_AMQPANALYZE_AMQP10_DECODEERROR_HPP_

#include <amqpAnalyze/Error.hpp>

namespace amqpAnalyze
{
    namespace amqp10
    {
        class FrameBuffer;

        class DecodeError: public Error
        {
        public:
            explicit DecodeError(const amqp10::FrameBuffer& frameBuffer, const std::string& errorMessage);
            explicit DecodeError(uint64_t packetNum, std::size_t amqpDataOffset, const std::string& errorMessage);
            explicit DecodeError(ErrorSeverity_t errorSeverity, const amqp10::FrameBuffer& frameBuffer, const std::string& errorMessage);
            explicit DecodeError(ErrorSeverity_t errorSeverity, uint64_t packetNum, std::size_t amqpDataOffset, const std::string& errorMessage);
            virtual ~DecodeError();
            std::size_t amqpDataOffset() const;
            uint64_t packetNum() const;
            virtual const char* typeStr() const override;

        protected:
            const uint64_t _packetNum;
            const std::size_t _amqpDataOffset;
            const std::string _msg;
        };

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQP10_DECODEERROR_HPP_ */
