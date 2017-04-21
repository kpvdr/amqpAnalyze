/*
 * Error.hpp
 *
 *  Created on: Mar 30, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_ERROR_HPP_
#define SRC_AMQPANALYZE_ERROR_HPP_

#include <cstdint>
#include <map>
#include <sstream>
#include <vector>

namespace amqpAnalyze
{
    namespace amqp10
    {
        class FrameBuffer;
    }

    class Message {
    protected:
        std::ostringstream oss;
    public:
        Message();
        Message(const Message& m);
        std::string toString() const;
        operator std::string() const;
        template <class T> Message& operator<<(const T& t) {oss << t; return *this;}
    };
#define MSG(message) (::amqpAnalyze::Message() << message)

    enum error_severity_t:uint8_t {
        ERROR,
        WARNING,
        INFO
    };

    class Error: public std::runtime_error
    {
    public:
        explicit Error(const std::string& err_msg = std::string());
        explicit Error(error_severity_t errorSeverity, const std::string& err_msg = std::string());
        virtual ~Error();
        virtual std::string formattedMessage() const;
        virtual const char* typeStr() const;
        static std::map<error_severity_t, const char*> s_errorSeverityNames;
    protected:
        std::ostringstream& formattedSeverity(std::ostringstream& oss) const;
        error_severity_t _errorSeverity;
    };

    typedef std::vector<const Error*> error_ptr_list_t;
    typedef error_ptr_list_t::iterator error_ptr_list_itr_t;
    typedef error_ptr_list_t::const_iterator error_ptr_list_citr_t;



    class AmqpDecodeError: public Error
    {
    public:
        explicit AmqpDecodeError(const amqp10::FrameBuffer& frameBuffer, const std::string& errorMessage);
        explicit AmqpDecodeError(uint64_t packetNum, std::size_t amqpDataOffset, const std::string& errorMessage);
        explicit AmqpDecodeError(error_severity_t errorSeverity, const amqp10::FrameBuffer& frameBuffer, const std::string& errorMessage);
        explicit AmqpDecodeError(error_severity_t errorSeverity, uint64_t packetNum, std::size_t amqpDataOffset, const std::string& errorMessage);
        virtual ~AmqpDecodeError();
        std::size_t amqpDataOffset() const;
        uint64_t packetNum() const;
        virtual const char* typeStr() const override;
    protected:
        const uint64_t _packetNum;
        const std::size_t _amqpDataOffset;
        const std::string _msg;
    };



    class AmqpValidationError: public AmqpDecodeError
    {
    public:
        explicit AmqpValidationError(const amqp10::FrameBuffer& frameBuffer, const std::string& errorMessage);
        explicit AmqpValidationError(uint64_t packetNum, std::size_t amqpDataOffset, const std::string& errorMessage);
        explicit AmqpValidationError(error_severity_t errorSeverity, const amqp10::FrameBuffer& frameBuffer, const std::string& errorMessage);
        explicit AmqpValidationError(error_severity_t errorSeverity, uint64_t packetNum, std::size_t amqpDataOffset, const std::string& errorMessage);
        virtual ~AmqpValidationError();
        virtual const char* typeStr() const override;
    };

} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_ERROR_HPP_ */
