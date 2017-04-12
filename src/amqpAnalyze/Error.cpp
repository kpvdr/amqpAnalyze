/*
 * Error.cpp
 *
 *  Created on: Mar 30, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/Error.hpp>

namespace amqpAnalyze
{

    Message::Message(): oss() {}
    Message::Message(const Message& m): oss(m.toString()) {}
    std::string Message::toString() const {return oss.str();}
    Message::operator std::string() const {return toString();}
    std::ostream& operator<<(std::ostream& out, const Message& m) {return out << m.toString();}

    Error::Error(const std::string& err_msg): std::runtime_error(err_msg) {}
    Error::~Error() {}

} /* namespace amqpAnalyze */
