/*
 * Connection.cpp
 *
 *  Created on: Apr 24, 2017
 *      Author: kpvdr
 */

#include "Connection.hpp"

#include <amqpAnalyze/amqp10/ConnectionError.hpp>
#include <amqpAnalyze/amqp10/Frame.hpp>
#include <amqpAnalyze/amqp10/Performative.hpp>
#include <amqpAnalyze/amqp10/Session.hpp>
#include <amqpAnalyze/amqp10/Type.hpp>
#include <amqpAnalyze/TcpConnection.hpp>

namespace amqpAnalyze
{
    namespace amqp10
    {

        Connection::Connection(const TcpConnection* tcpConnectionPtr):
            _errorList(),
            _closeError(),
            _initiatorAddrStr(tcpConnectionPtr->srcAddress()),
            _initiatorSessionMap(),
            _initiatorOpen(nullptr),
            _initiatorState("initiator"),
            _initiatorTcpClosedFlag(false),
            _responderAddrStr(tcpConnectionPtr->destAddress()),
            _responderSessionMap(),
            _responderOpen(nullptr),
            _responderState("responder"),
            _responderTcpClosedFlag(false),
            _lastActor(LastActorType_t::NONE)
        {}

        Connection::~Connection() {
            for (std::map<uint16_t, Session*>::iterator i=_initiatorSessionMap.begin(); i!= _initiatorSessionMap.end(); ++i) {
                delete i->second;
            }
            _initiatorSessionMap.clear();
            for (std::map<uint16_t, Session*>::iterator i=_responderSessionMap.begin(); i!= _responderSessionMap.end(); ++i) {
                delete i->second;
            }
            _responderSessionMap.clear();
            for (std::vector<amqpAnalyze::Error*>::iterator i=_errorList.begin(); i!=_errorList.end(); ++i) {
                delete *i;
            }
            _errorList.clear();
        }

        void Connection::handleFrame(TcpConnection* tcpConnectionPtr, bool replyFlag, Frame* framePtr) {
            const uint16_t channel = checkChannel(framePtr);
            if (framePtr->isEmpty()) return;  // ignore heartbeats
            Performative* performativePtr = framePtr->performative();
            if (performativePtr == nullptr) {
                throw amqpAnalyze::Error(MSG("Connection::handleFrame(): Unexpected nullptr for Frame::performative()"));
            }
            std::ostringstream oss;
            oss << "Connection: ";
            switch (framePtr->performative()->type()) {
            case PerformativeType_t::OPEN:
                {
                    AmqpOpen* amqpOpenPtr(dynamic_cast<AmqpOpen*>(performativePtr));
                    if (isInitiator(tcpConnectionPtr, replyFlag)) {
                        _initiatorOpen = amqpOpenPtr;
                        _initiatorState.sentOpen();
                        _responderState.receivedOpen(_lastActor == LastActorType_t::INITIATOR);
                        _lastActor = LastActorType_t::INITIATOR;
                    } else {
                        _responderOpen = amqpOpenPtr;
                        _responderState.sentOpen();
                        _initiatorState.receivedOpen(false);
                        _lastActor = LastActorType_t::RECEIVER;
                    }
                    oss << _initiatorState.stateStr() << ":" <<  _responderState.stateStr();
                    amqpOpenPtr->setStateStr(oss.str());
                }
                break;
            case PerformativeType_t::CLOSE:
                {
                    AmqpClose* amqpClosePtr(dynamic_cast<AmqpClose*>(performativePtr));
                    if (amqpClosePtr == nullptr) {
                        throw amqpAnalyze::Error(MSG("Connection::handleFrame(): Error downcasting class \"Performative\" to class \"AmqpClose\": found=\""
                                        << performativePtr->name() << "\""));
                    }
                    const bool closeErrorFlag = checkError(amqpClosePtr);
                    if (isInitiator(tcpConnectionPtr, replyFlag)) {
                        _initiatorState.sentClose(closeErrorFlag);
                        _responderState.receivedClose(_lastActor == LastActorType_t::INITIATOR);
                        _lastActor = LastActorType_t::INITIATOR;
                    } else {
                        _responderState.sentClose(closeErrorFlag);
                        _initiatorState.receivedClose(false);
                        _lastActor = LastActorType_t::RECEIVER;
                    }
                    oss << _initiatorState.stateStr() << ":" <<  _responderState.stateStr();
                    amqpClosePtr->setStateStr(oss.str());
                }
                break;
            default:
                // All other frames are handled through Session objects
                const bool initiatorFlag = isInitiator(tcpConnectionPtr, replyFlag);
                insertIfNotPresent(_initiatorSessionMap, channel, initiatorFlag)->handleFrame(tcpConnectionPtr, framePtr, initiatorFlag);
                insertIfNotPresent(_responderSessionMap, channel, !initiatorFlag)->handleFrame(tcpConnectionPtr, framePtr, initiatorFlag);;
            }
        }

        void Connection::handleProtocolHeader(const TcpConnection* tcpConnectionPtr, bool replyFlag, ProtocolHeader* protocolHeaderPtr) {
            if (isInitiator(tcpConnectionPtr, replyFlag)) {
                _initiatorState.sentProtocolHeader(protocolHeaderPtr->version());
                _responderState.receivedProtocolHeader(protocolHeaderPtr->version());
                _lastActor = LastActorType_t::INITIATOR;
            } else {
                _responderState.sentProtocolHeader(protocolHeaderPtr->version());
                _initiatorState.receivedProtocolHeader(protocolHeaderPtr->version());
                _lastActor = LastActorType_t::RECEIVER;
            }
            std::ostringstream oss;
            oss << "Connection: " << _initiatorState.stateStr() << ":" <<  _responderState.stateStr();
            protocolHeaderPtr->setStateStr(oss.str());
        }

        bool Connection::handleTcpClose(const TcpConnection* tcpConnectionPtr, bool replyFlag) {
            if (isInitiator(tcpConnectionPtr, replyFlag)) {
                if (_responderTcpClosedFlag) return true;
                _initiatorTcpClosedFlag = true;
            } else {
                if (_initiatorTcpClosedFlag) return true;
                _responderTcpClosedFlag = true;
            }
            return false;
        }

        uint16_t Connection::checkChannel(const Frame* framePtr) {
            const uint16_t channel(framePtr->typeSpecific());
            if ((_initiatorOpen == nullptr || _responderOpen == nullptr) && channel != 0) {
                _errorList.push_back(new ConnectionError(MSG("ConnectionError: Channel number > 0 before open complete, found channel=0x" << std::hex << channel)));
            }
            return channel;
        }

        bool Connection::checkError(const AmqpClose* amqpClosePtr) {
            const AmqpErrorRecord* errorRecordPtr = amqpClosePtr->error(false);
            if (errorRecordPtr == nullptr) return false;
            std::ostringstream oss;
            errorRecordPtr->appendString(oss, 4, false);
            _closeError.assign(oss.str());
            return true;
        }

        bool Connection::isInitiator(const TcpConnection* tcpConnectionPtr, bool replyFlag) const {
            if (tcpConnectionPtr->srcAddress().compare(_initiatorAddrStr) == 0) {
                if (tcpConnectionPtr->destAddress().compare(_responderAddrStr) == 0) return !replyFlag;
                throw amqpAnalyze::Error(MSG("Connection::isInitiator(): TCP destination address \"" << tcpConnectionPtr
                                            << "\" does not match responder (" << _responderAddrStr << ")"));
            } else if (tcpConnectionPtr->srcAddress().compare(_responderAddrStr) == 0) {
                if (tcpConnectionPtr->destAddress().compare(_initiatorAddrStr) == 0)  return replyFlag;
                throw amqpAnalyze::Error(MSG("Connection::isInitiator(): TCP destination address \"" << tcpConnectionPtr
                                            << "\" does not match initiator (" << _initiatorAddrStr << ")"));
            }
            throw amqpAnalyze::Error(MSG("Connection::isInitiator(): TCP source address \"" << tcpConnectionPtr
                            << "\" matches neither initiator (" << _initiatorAddrStr << ") or responder ("
                            << _responderAddrStr << ")"));
        }

        // static
        Session* Connection::insertIfNotPresent(SessionMap_t& sessionMap, uint16_t channel, bool initiatorFlag) {
            SessionMapItr_t itr=sessionMap.lower_bound(channel);
            if (itr != sessionMap.end() && !(sessionMap.key_comp()(channel, itr->first))) {
                return itr->second;
            } else {
                Session* sessionPtr = new Session(channel, initiatorFlag);
                sessionMap.emplace(channel, sessionPtr);
                return sessionPtr;
            }
        }


    } /* namespace amqp10 */
} /* namespace amqpAnalyze */
