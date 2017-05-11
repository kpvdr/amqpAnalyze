/*
 * SessionState.cpp
 *
 *  Created on: May 8, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/amqp10/SessionState.hpp>

#include <amqpAnalyze/amqp10/IllegalStateError.hpp>

namespace amqpAnalyze
{
    namespace amqp10
    {

        SessionState::SessionState(const std::string& name):
            _name(name),
            _state(SessionState_t::UNMAPPED)
        {}

        SessionState::~SessionState() {}

        SessionState_t SessionState::state() const {
            return _state;
        }

        const char* SessionState::stateStr() const {
            return s_SessionStateNames[_state];
        }

        void SessionState::receivedBegin() {
            switch (_state) {
            case SessionState_t::UNMAPPED: _state = SessionState_t::BEGIN_RCVD; break;
            case SessionState_t::BEGIN_SENT: _state = SessionState_t::MAPPED; break;
            default:
                throw new IllegalStateError("SessionState", _name, "receivedBegin", s_SessionStateNames[_state]);
            }
        }

        void SessionState::receivedEnd() {
            switch (_state) {
            case SessionState_t::MAPPED: _state = SessionState_t::END_RCVD; break;
            case SessionState_t::END_SENT:
            case SessionState_t::DISCARDING:
                _state = SessionState_t::UNMAPPED;
                break;
            default:
                throw new IllegalStateError("SessionState", _name, "receivedEnd", s_SessionStateNames[_state]);
            }
        }

        void SessionState::sentBegin() {
            switch (_state) {
            case SessionState_t::UNMAPPED: _state = SessionState_t::BEGIN_SENT; break;
            case SessionState_t::BEGIN_RCVD: _state = SessionState_t::MAPPED; break;
            default:
                throw new IllegalStateError("SessionState", _name, "sentBegin", s_SessionStateNames[_state]);
            }
        }

        void SessionState::sentEnd(bool errorFlag) {
            switch (_state) {
            case SessionState_t::MAPPED: _state = errorFlag ? SessionState_t::DISCARDING : SessionState_t::END_SENT; break;
            case SessionState_t::END_RCVD:
            case SessionState_t::DISCARDING:
                _state = SessionState_t::UNMAPPED;
                break;
            default:
                throw new IllegalStateError("SessionState", _name, "sentEnd", s_SessionStateNames[_state]);
            }
        }

        // static
        std::map<SessionState_t, const char*> SessionState::s_SessionStateNames = {
                        {SessionState_t::UNMAPPED, "UNMAPPED"},
                        {SessionState_t::BEGIN_SENT, "BEGIN_SENT"},
                        {SessionState_t::BEGIN_RCVD, "BEGIN_RCVD"},
                        {SessionState_t::MAPPED, "MAPPED"},
                        {SessionState_t::END_SENT, "END_SENT"},
                        {SessionState_t::END_RCVD, "END_RCVD"},
                        {SessionState_t::DISCARDING, "DISCARDING"}
        };

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */
