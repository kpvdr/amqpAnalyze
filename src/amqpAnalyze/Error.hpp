/*
 * Error.hpp
 *
 *  Created on: Mar 30, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_ERROR_HPP_
#define SRC_AMQPANALYZE_ERROR_HPP_

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

    enum class ErrorSeverity_t:uint8_t {
        ERROR,
        WARNING,
        INFO
    };



    class Error: public std::runtime_error
    {
    public:
        explicit Error(const std::string& err_msg = std::string());
        explicit Error(ErrorSeverity_t errorSeverity, const std::string& err_msg = std::string());
        virtual ~Error();
        virtual std::string formattedMessage(bool colorFlag) const;
        virtual const char* typeStr() const;
        static std::map<ErrorSeverity_t, const char*> s_errorSeverityNames;
    protected:
        std::ostringstream& formattedSeverity(std::ostringstream& oss, bool colorFlag) const;
        ErrorSeverity_t _errorSeverity;
    };

} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_ERROR_HPP_ */
