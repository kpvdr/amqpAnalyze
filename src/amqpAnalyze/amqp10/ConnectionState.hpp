/*
 * ConnectionState.hpp
 *
 *  Created on: Apr 24, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_AMQP10_TRANSPORT_CONNECTIONSTATE_HPP_
#define SRC_AMQPANALYZE_AMQP10_TRANSPORT_CONNECTIONSTATE_HPP_

#include <amqpAnalyze/amqp10/ProtocolHeader.hpp>
#include <cstdint>
#include <deque>
#include <map>

namespace amqpAnalyze
{
    namespace amqp10
    {

        enum class ConnectionState_t {
            START,
            HDR_RCVD,
            HDR_SENT,
            HDR_EXCH,
            OPEN_PIPE,
            OC_PIPE,
            OPEN_RCVD,
            OPEN_SENT,
            CLOSE_PIPE,
            OPENED,
            CLOSE_RCVD,
            CLOSE_SENT,
            DISCARDING,
            END
        };
        enum class PipelinedAction_t {
            OPEN,
            CLOSE
        };
        enum class LastAction_t {
            NONE,
            SENT,
            RECEIVED
        };

        class ConnectionState
        {
        public:
            ConnectionState(const std::string& name);
            virtual ~ConnectionState();

            const struct AmqpVersion& amqpVersion() const;
            const std::string& name() const;
            ConnectionState_t state() const;
            const char* stateStr() const;

            //--- change of state actions ---

            void receivedClose(bool pipelinedFlag);
            void sentClose(bool errorFlag);
            void receivedOpen(bool pipelinedFlag);
            void sentOpen();
            void receivedProtocolHeader(const struct AmqpVersion& amqpVersion);
            void sentProtocolHeader(const struct AmqpVersion& amqpVersion);

        protected:
            const std::string _name;
            ConnectionState_t _state;
            struct AmqpVersion _amqpVersion;
            LastAction_t _lastAction;
            std::deque<PipelinedAction_t> _pipelinedActions;

            bool isVersionEqual(const struct AmqpVersion& amqpVersion);

            static std::map<ConnectionState_t, const char*> s_ConnectionStateNames;
        };


    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQP10_TRANSPORT_CONNECTIONSTATE_HPP_ */
