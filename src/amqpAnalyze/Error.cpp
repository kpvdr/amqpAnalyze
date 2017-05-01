/*
 * Error.cpp
 *
 *  Created on: Mar 30, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/Error.hpp>

#include <amqpAnalyze/amqp10/FrameBuffer.hpp>

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
    AmqpDecodeError::AmqpDecodeError(ErrorSeverity_t errorSeverity, const amqp10::FrameBuffer& frameBuffer, const std::string& errorMessage):
        Error(errorSeverity, errorMessage),
        _packetNum(frameBuffer.getPacketNum()),
        _amqpDataOffset(frameBuffer.getFrameOffsetSnapshot())
    {}
    AmqpDecodeError::AmqpDecodeError(ErrorSeverity_t errorSeverity, uint64_t packetNum, std::size_t amqpDataOffset, const std::string& errorMessage):
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
        AmqpDecodeError(ErrorSeverity_t::WARNING, frameBuffer, errorMessage)
    {}
    AmqpValidationError::AmqpValidationError(uint64_t packetNum, std::size_t amqpDataOffset, const std::string& errorMessage):
        AmqpDecodeError(ErrorSeverity_t::WARNING, packetNum, amqpDataOffset, errorMessage)
    {}
    AmqpValidationError::AmqpValidationError(ErrorSeverity_t errorSeverity, const amqp10::FrameBuffer& frameBuffer, const std::string& errorMessage):
        AmqpDecodeError(errorSeverity, frameBuffer, errorMessage)
    {}
    AmqpValidationError::AmqpValidationError(ErrorSeverity_t errorSeverity, uint64_t packetNum, std::size_t amqpDataOffset, const std::string& errorMessage):
        AmqpDecodeError(errorSeverity, packetNum, amqpDataOffset, errorMessage)
    {}
    AmqpValidationError::~AmqpValidationError() {}
    const char* AmqpValidationError::typeStr() const {
        return "validation: ";
    }

} /* namespace amqpAnalyze */
