/*
 * SessionState.hpp
 *
 *  Created on: May 8, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_AMQP10_SESSIONSTATE_HPP_
#define SRC_AMQPANALYZE_AMQP10_SESSIONSTATE_HPP_

#include <map>
#include <string>

namespace amqpAnalyze
{
    namespace amqp10
    {

        enum class SessionState_t {
            UNMAPPED,
            BEGIN_SENT,
            BEGIN_RCVD,
            MAPPED,
            END_SENT,
            END_RCVD,
            DISCARDING
        };

        class SessionState
        {
        public:
            SessionState(const std::string& name);
            virtual ~SessionState();
            SessionState_t state() const;
            const char* stateStr() const;

            void receivedBegin();
            void receivedEnd();
            void sentBegin();
            void sentEnd(bool errorFlag);

        protected:
            const std::string _name;
            SessionState_t _state;
            static std::map<SessionState_t, const char*> s_SessionStateNames;
        };

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQP10_SESSIONSTATE_HPP_ */
