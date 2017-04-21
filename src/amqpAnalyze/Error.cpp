/*
 * Error.cpp
 *
 *  Created on: Mar 30, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/Error.hpp>

#include <amqpAnalyze/amqp10/FrameBuffer.hpp>
#include <iomanip>
#include <std/AnsiTermColors.hpp>

namespace amqpAnalyze
{

    Message::Message(): oss() {}
    Message::Message(const Message& m): oss(m.toString()) {}
    std::string Message::toString() const {return oss.str();}
    Message::operator std::string() const {return toString();}
    std::ostream& operator<<(std::ostream& out, const Message& m) { return out << m.toString(); }



    //--- class Error ---

    Error::Error(const std::string& err_msg):
        std::runtime_error(err_msg),
        _errorSeverity(error_severity_t::ERROR)
    {}
    Error::Error(error_severity_t errorSeverity, const std::string& err_msg):
        std::runtime_error(err_msg),
        _errorSeverity(errorSeverity)
    {}
    Error::~Error() {}
    std::string Error::formattedMessage() const {
        std::ostringstream oss;
        formattedSeverity(oss);
        oss << typeStr() << what();
        return oss.str();
    }
    std::ostringstream& Error::formattedSeverity(std::ostringstream& oss) const {
        switch (_errorSeverity) {
        case error_severity_t::ERROR:
            oss << std::fgnd_b_red;
            break;
        case error_severity_t::WARNING:
            oss << std::fgnd_b_red;
            break;
        case error_severity_t::INFO:
            oss << std::fgnd_b_green;
            break;
        }
        oss << s_errorSeverityNames[_errorSeverity] << std::res << ": ";
        return oss;
    }
    const char* Error::typeStr() const {
        return "";
    }
    // static
    std::map<error_severity_t, const char*> Error::s_errorSeverityNames = {
        {error_severity_t::ERROR, "ERROR"},
        {error_severity_t::WARNING, "WARNING"},
        {error_severity_t::INFO, "INFO"}
    };



    //--- class AmqpDecodeError ---

    AmqpDecodeError::AmqpDecodeError(const amqp10::FrameBuffer& frameBuffer, const std::string& errorMessage):
        Error(errorMessage),
        _packetNum(frameBuffer.getPacketNum()),
        _amqpDataOffset(frameBuffer.getFrameOffsetSnapshot())
    {}
    AmqpDecodeError::AmqpDecodeError(uint64_t packetNum, std::size_t amqpDataOffset, const std::string& errorMessage):
        Error(errorMessage),
        _packetNum(packetNum),
        _amqpDataOffset(amqpDataOffset)
    {}
    AmqpDecodeError::AmqpDecodeError(error_severity_t errorSeverity, const amqp10::FrameBuffer& frameBuffer, const std::string& errorMessage):
        Error(errorSeverity, errorMessage),
        _packetNum(frameBuffer.getPacketNum()),
        _amqpDataOffset(frameBuffer.getFrameOffsetSnapshot())
    {}
    AmqpDecodeError::AmqpDecodeError(error_severity_t errorSeverity, uint64_t packetNum, std::size_t amqpDataOffset, const std::string& errorMessage):
        Error(errorSeverity, errorMessage),
        _packetNum(packetNum),
        _amqpDataOffset(amqpDataOffset)
    {}
    AmqpDecodeError::~AmqpDecodeError() {}
    uint64_t AmqpDecodeError::packetNum() const { return _packetNum; }
    std::size_t AmqpDecodeError::amqpDataOffset() const { return _amqpDataOffset; }
    const char* AmqpDecodeError::typeStr() const {
        return "decode: ";
    }



    //--- class AmqpValidationError ---

    AmqpValidationError::AmqpValidationError(const amqp10::FrameBuffer& frameBuffer, const std::string& errorMessage):
        AmqpDecodeError(error_severity_t::WARNING, frameBuffer, errorMessage)
    {}
    AmqpValidationError::AmqpValidationError(uint64_t packetNum, std::size_t amqpDataOffset, const std::string& errorMessage):
        AmqpDecodeError(error_severity_t::WARNING, packetNum, amqpDataOffset, errorMessage)
    {}
    AmqpValidationError::AmqpValidationError(error_severity_t errorSeverity, const amqp10::FrameBuffer& frameBuffer, const std::string& errorMessage):
        AmqpDecodeError(errorSeverity, frameBuffer, errorMessage)
    {}
    AmqpValidationError::AmqpValidationError(error_severity_t errorSeverity, uint64_t packetNum, std::size_t amqpDataOffset, const std::string& errorMessage):
        AmqpDecodeError(errorSeverity, packetNum, amqpDataOffset, errorMessage)
    {}
    AmqpValidationError::~AmqpValidationError() {}
    const char* AmqpValidationError::typeStr() const {
        return "validation: ";
    }

} /* namespace amqpAnalyze */
