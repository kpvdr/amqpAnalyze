/*
 * Error.hpp
 *
 *  Created on: Mar 30, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_ERROR_HPP_
#define SRC_AMQPANALYZE_ERROR_HPP_

#include <sstream>

namespace amqpAnalyze
{

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

    class Error: public std::runtime_error {
    public:
        explicit Error(const std::string& err_msg = std::string());
        virtual ~Error();
    };

} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_ERROR_HPP_ */
