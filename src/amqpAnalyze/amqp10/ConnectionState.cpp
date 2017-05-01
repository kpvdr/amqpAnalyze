/*
 * ConnectionState.cpp
 *
 *  Created on: Apr 24, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/amqp10/ConnectionState.hpp>

#include <amqpAnalyze/amqp10/IllegalStateError.hpp>

namespace amqpAnalyze
{
    namespace amqp10
    {

        ConnectionState::ConnectionState(const std::string& name):
            _name(name),
            _state(ConnectionState_t::START),
            _amqpVersion(),
            _lastAction(LastAction_t::NONE),
            _pipelinedActions()
        {}

        ConnectionState::~ConnectionState() {}

        ConnectionState_t ConnectionState::state() const {
            return _state;
        }

        const struct AmqpVersion& ConnectionState::amqpVersion() const {
            return _amqpVersion;
        }

        const std::string& ConnectionState::name() const {
            return _name;
        }

        const char* ConnectionState::stateStr() const {
            return s_ConnectionStateNames[_state];
        }

        void ConnectionState::receivedClose(bool pipelinedFlag) {
            if (pipelinedFlag) {
                _pipelinedActions.push_back(PipelinedAction_t::CLOSE);
                return;
            }
            switch (_state) {
            case ConnectionState_t::OPENED: _state = ConnectionState_t::CLOSE_RCVD; break;
            case ConnectionState_t::CLOSE_SENT:
            case ConnectionState_t::DISCARDING:
                _state = ConnectionState_t::END; break;
            default:
                throw new IllegalStateError("ConnectionState", _name, "receivedClose", s_ConnectionStateNames[_state]);
            }
        }

        void ConnectionState::sentClose(bool errorFlag) {
            if (errorFlag) {
                switch (_state) {
                case ConnectionState_t::OPENED: _state = ConnectionState_t::DISCARDING; break;
                default:
                    throw IllegalStateError("ConnectionState", _name, "sentCloseWithError", s_ConnectionStateNames[_state]);
                }
            } else {
                switch (_state) {
                case ConnectionState_t::OPEN_SENT: _state = ConnectionState_t::CLOSE_PIPE; break;
                case ConnectionState_t::OPENED: _state = ConnectionState_t::CLOSE_SENT; break;
                case ConnectionState_t::OPEN_PIPE: _state = ConnectionState_t::OC_PIPE; break;
                case ConnectionState_t::CLOSE_RCVD: _state = ConnectionState_t::END; break;
                default:
                    throw new IllegalStateError("ConnectionState", _name, "sentClose", s_ConnectionStateNames[_state]);
                }
            }
        }

        void ConnectionState::receivedOpen(bool pipelinedFlag) {
            if (pipelinedFlag) {
                _pipelinedActions.push_back(PipelinedAction_t::OPEN);
                return;
            }
            switch (_state) {
            case ConnectionState_t::HDR_EXCH: _state = ConnectionState_t::OPEN_RCVD; break;
            case ConnectionState_t::OPEN_SENT: _state = ConnectionState_t::OPENED; break;
            case ConnectionState_t::CLOSE_PIPE: _state = ConnectionState_t::CLOSE_SENT; break;
            default:
                throw new IllegalStateError("ConnectionState", _name, "receivedOpen", s_ConnectionStateNames[_state]);
            }
        }

        void ConnectionState::sentOpen() {
            switch (_state) {
            case ConnectionState_t::HDR_EXCH: _state = ConnectionState_t::OPEN_SENT; break;
            case ConnectionState_t::HDR_SENT: _state = ConnectionState_t::OPEN_PIPE; break;
            case ConnectionState_t::OPEN_RCVD: _state = ConnectionState_t::OPENED; break;
            default:
                throw new IllegalStateError("ConnectionState", _name, "sentOpen", s_ConnectionStateNames[_state]);
            }
            if (!_pipelinedActions.empty()) {
                if (_pipelinedActions.front() == PipelinedAction_t::CLOSE) {
                    receivedClose(false);
                } else {
                    throw amqpAnalyze::Error(MSG("ConnectionState::sentOpen(): Unexpected pipelined action: " << (int)_pipelinedActions.front()));
                }
                _pipelinedActions.pop_front();
            }
        }

        void ConnectionState::receivedProtocolHeader(const struct AmqpVersion& amqpVersion) {
            bool equal(isVersionEqual(amqpVersion));
            switch (_state) {
            case ConnectionState_t::START: _state = ConnectionState_t::HDR_RCVD; break;
            case ConnectionState_t::HDR_SENT: _state = equal ? ConnectionState_t::HDR_EXCH : ConnectionState_t::END; break;
            case ConnectionState_t::OPEN_PIPE: _state = equal ? ConnectionState_t::OPEN_SENT : ConnectionState_t::END; break;
            case ConnectionState_t::OC_PIPE: _state = equal ? ConnectionState_t::CLOSE_PIPE : ConnectionState_t::END; break;
            default:
                throw new IllegalStateError("ConnectionState", _name, "receivedProtocolHeader", s_ConnectionStateNames[_state]);
            }
        }

        void ConnectionState::sentProtocolHeader(const struct AmqpVersion& amqpVersion) {
            bool equal(isVersionEqual(amqpVersion));
            switch (_state) {
            case ConnectionState_t::START: _state = ConnectionState_t::HDR_SENT; break;
            case ConnectionState_t::HDR_RCVD: _state = equal ? ConnectionState_t::HDR_EXCH : ConnectionState_t::END; break;
            default:
                throw new IllegalStateError("ConnectionState", _name, "sentProtocolHeader", s_ConnectionStateNames[_state]);
            }
            if (!_pipelinedActions.empty()) {
                if (_pipelinedActions.front() == PipelinedAction_t::OPEN) {
                    receivedOpen(false);
                } else {
                    throw amqpAnalyze::Error(MSG("ConnectionState::sentProtocolHeader(): Unexpected pipelined action: " << (int)_pipelinedActions.front()));
                }
                _pipelinedActions.pop_front();
            }
        }

        bool ConnectionState::isVersionEqual(const struct AmqpVersion& amqpVersion) {
            if (_amqpVersion.isZero()) { // not previously set
                _amqpVersion.set(amqpVersion);
                return true;
            }
            return _amqpVersion.isEqual(amqpVersion);
        }

        //static
        std::map<ConnectionState_t, const char*> ConnectionState::s_ConnectionStateNames {
            {ConnectionState_t::START, "START"},
            {ConnectionState_t::HDR_RCVD, "HDR_RCVD"},
            {ConnectionState_t::HDR_SENT, "HDR_SENT"},
            {ConnectionState_t::HDR_EXCH, "HDR_EXCH"},
            {ConnectionState_t::OPEN_PIPE, "OPEN_PIPE"},
            {ConnectionState_t::OC_PIPE, "OC_PIPE"},
            {ConnectionState_t::OPEN_RCVD, "OPEN_RCVD"},
            {ConnectionState_t::OPEN_SENT, "OPEN_SENT"},
            {ConnectionState_t::CLOSE_PIPE, "CLOSE_PIPE"},
            {ConnectionState_t::OPENED, "OPENED"},
            {ConnectionState_t::CLOSE_RCVD, "CLOSE_RCVD"},
            {ConnectionState_t::CLOSE_SENT, "CLOSE_SENT"},
            {ConnectionState_t::DISCARDING, "DISCARDING"},
            {ConnectionState_t::END, "END"}
        };

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */
