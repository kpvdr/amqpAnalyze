/*
 * Session.cpp
 *
 *  Created on: May 11, 2017
 *      Author: kpvdr
 */

#include "Session.hpp"

#include <amqpAnalyze/amqp10/Frame.hpp>
#include <amqpAnalyze/amqp10/Performative.hpp>
#include <amqpAnalyze/amqp10/Type.hpp>
#include <amqpAnalyze/Error.hpp>
#include <amqpAnalyze/TcpConnection.hpp>

namespace amqpAnalyze
{
    namespace amqp10
    {

        Session::Session(uint16_t channel, bool initiatorFlag):
            _initiatorFlag(initiatorFlag),
            _channel(channel),
            _sessionState(MSG((initiatorFlag ? "initiator ch" : "responder ch") << channel)),
            _endError()
        {}

        Session::~Session() {}

        void Session::handleFrame(TcpConnection* tcpConnectionPtr, Frame* framePtr, bool initiatorFlag) {
            Performative* performativePtr = framePtr->performative();
            if (performativePtr == nullptr) {
                throw amqpAnalyze::Error(MSG("Connection::handleFrame(): Unexpected nullptr for Frame::performative()"));
            }
            std::ostringstream oss;
            oss << (performativePtr->stateStr().empty() ? "Session:" : "");
            oss << (_initiatorFlag ? " I" : " R") << _channel << ":";
            switch (framePtr->performative()->type()) {
            case PerformativeType_t::BEGIN:
                {
                    AmqpBegin* amqpBeginPtr(dynamic_cast<AmqpBegin*>(performativePtr));
                    if (amqpBeginPtr == nullptr) {
                        throw amqpAnalyze::Error(MSG("Connection::handleFrame(): Error downcasting class \"Performative\" to class \"AmqpBegin\": found=\""
                                        << performativePtr->name() << "\""));
                    }
                    _initiatorFlag == initiatorFlag ? _sessionState.sentBegin() : _sessionState.receivedBegin();
                    oss << _sessionState.stateStr();
                    amqpBeginPtr->appendStateStr(oss.str());
                    if (_initiatorFlag) tcpConnectionPtr->incrNumSessions(); // only count connections from initiator
                }
                break;
            case PerformativeType_t::END:
                {
                    AmqpEnd* amqpEndPtr(dynamic_cast<AmqpEnd*>(performativePtr));
                    if (amqpEndPtr == nullptr) {
                        throw amqpAnalyze::Error(MSG("Connection::handleFrame(): Error downcasting class \"Performative\" to class \"AmqpEnd\": found=\""
                                        << performativePtr->name() << "\""));
                    }
                    _initiatorFlag == initiatorFlag ? _sessionState.sentEnd(checkError(amqpEndPtr)) : _sessionState.receivedEnd();
                    oss << _sessionState.stateStr();
                    amqpEndPtr->appendStateStr(oss.str());
                }
                break;
            default:
                // All other frames are handled through Link objects
                // TODO: Add link and link state processing here
                ;
            }
        }

        bool Session::checkError(const AmqpEnd* amqpEndPtr) {
            const AmqpErrorRecord* errorRecordPtr = amqpEndPtr->error(false);
            if (errorRecordPtr == nullptr) return false;
            std::ostringstream oss;
            errorRecordPtr->appendString(oss, 4, false);
            _endError.assign(oss.str());
            return true;
        }

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */
