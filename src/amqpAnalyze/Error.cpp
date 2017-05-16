/*
 * Error.cpp
 *
 *  Created on: Mar 30, 2017
 *      Author: kpvdr
 */

#include "Error.hpp"

#include <amqpAnalyze/amqp10/FrameBuffer.hpp>

namespace amqpAnalyze
{

    Message::Message(): oss() {}

    Message::Message(const Message& m): oss(m.toString()) {}

    std::string Message::toString() const {return oss.str();}

    Message::operator std::string() const {return toString();}

    std::ostream& operator<<(std::ostream& out, const Message& m) { return out << m.toString(); }



    Error::Error(const std::string& err_msg):
        std::runtime_error(err_msg),
        _errorSeverity(ErrorSeverity_t::ERROR)
    {}

    Error::Error(ErrorSeverity_t errorSeverity, const std::string& err_msg):
        std::runtime_error(err_msg),
        _errorSeverity(errorSeverity)
    {}

    Error::~Error() {}

    std::string Error::formattedMessage(bool colorFlag) const {
        std::ostringstream oss;
        formattedSeverity(oss, colorFlag);
        oss << ": " << typeStr() << what();
        return oss.str();
    }

    std::ostringstream& Error::formattedSeverity(std::ostringstream& oss, bool colorFlag) const {
        switch (_errorSeverity) {
        case ErrorSeverity_t::ERROR:
            oss << Color::color(DisplayColorType_t::MSG_ERROR, s_errorSeverityNames[_errorSeverity]);
            break;
        case ErrorSeverity_t::WARNING:
            oss << Color::color(DisplayColorType_t::MSG_WARNING, s_errorSeverityNames[_errorSeverity]);
            break;
        case ErrorSeverity_t::INFO:
            oss << Color::color(DisplayColorType_t::MSG_INFO, s_errorSeverityNames[_errorSeverity]);
            break;
        }
        return oss;
    }

    const char* Error::typeStr() const {
        return "";
    }

    // static
    std::map<ErrorSeverity_t, const char*> Error::s_errorSeverityNames = {
        {ErrorSeverity_t::ERROR, "ERROR"},
        {ErrorSeverity_t::WARNING, "WARNING"},
        {ErrorSeverity_t::INFO, "INFO"}
    };

} /* namespace amqpAnalyze */
