/*
 * Connection.cpp
 *
 *  Created on: Apr 24, 2017
 *      Author: kpvdr
 */

#include "Connection.hpp"

#include <amqpAnalyze/amqp10/ConnectionError.hpp>
#include <amqpAnalyze/amqp10/Endpoint.hpp>
#include <amqpAnalyze/amqp10/Frame.hpp>
#include <amqpAnalyze/amqp10/Performative.hpp>
//#include <amqpAnalyze/amqp10/Session.hpp>
#include <amqpAnalyze/amqp10/SessionHandler.hpp>
#include <amqpAnalyze/amqp10/Type.hpp>
#include <amqpAnalyze/Error.hpp>
#include <amqpAnalyze/TcpConnection.hpp>

namespace amqpAnalyze
{
    namespace amqp10
    {

        Connection::Connection(const TcpConnection* tcpConnectionPtr):
            _errorList(),
            _closeError(),
            //_frameErrorFlag(false),
            _initiatorAddrStr(tcpConnectionPtr->srcAddress()),
            //_initiatorCloseError(),
            _initiatorEndpoints(),
            //_initiatorHeaderSent(false),
            _initiatorOpen(nullptr),
            _initiatorState("initiator"),
            _initiatorTcpClosedFlag(false),
            _responderAddrStr(tcpConnectionPtr->destAddress()),
            //_responderCloseError(),
            _responderEndpoints(),
            //_responderHeaderSent(false),
            _responderOpen(nullptr),
            _responderState("responder"),
            _responderTcpClosedFlag(false),
            _lastActor(LastActorType_t::NONE)
        {}

        Connection::~Connection() {
/*
            for (std::map<uint16_t, Endpoint*>::iterator i=_initiatorEndpoints.begin(); i!= _initiatorEndpoints.end(); ++i) {
                delete i->second;
            }
            _initiatorEndpoints.clear();
*/
/*
            for (std::map<uint16_t, Endpoint*>::iterator i=_responderEndpoints.begin(); i!= _responderEndpoints.end(); ++i) {
                delete i->second;
            }
            _responderEndpoints.clear();
*/
            for (std::vector<amqpAnalyze::Error*>::iterator i=_errorList.begin(); i!=_errorList.end(); ++i) {
                delete *i;
            }
            _errorList.clear();
        }

        void Connection::handleFrame(const TcpConnection* tcpConnectionPtr, bool replyFlag, Frame* framePtr) {
            checkChannel(framePtr);
            if (framePtr->isEmpty()) return;  // ignore heartbeats
//            if (framePtr->hasErrors()) {
//                _frameErrorFlag = true;
//            }
            Performative* performativePtr = framePtr->performative();
            if (performativePtr == nullptr) {
                throw amqpAnalyze::Error(MSG("Connection::handleFrame(): Unexpected nullptr for Frame::performative()"));
            }
            std::ostringstream oss;
            switch (framePtr->performative()->type()) {
            case PerformativeType_t::OPEN:
                {
                    AmqpOpen* amqpOpenPtr(dynamic_cast<AmqpOpen*>(performativePtr));
                    if (isInitiator(tcpConnectionPtr, replyFlag)) {
                        _initiatorOpen = amqpOpenPtr;
                        _initiatorState.sentOpen();
                        _responderState.receivedOpen(_lastActor == LastActorType_t::INITIATOR);
                        _lastActor = LastActorType_t::INITIATOR;
                    } else if (isResponder(tcpConnectionPtr, replyFlag)) {
                        _responderOpen = amqpOpenPtr;
                        _responderState.sentOpen();
                        _initiatorState.receivedOpen(false);
                        _lastActor = LastActorType_t::RECEIVER;
                    } else {
                        throw amqpAnalyze::Error(MSG("Connection::handleFrame(): TCP address \"" << tcpConnectionPtr
                                        << "\" matches neither initiator nor responder"));
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
                    } else if (isResponder(tcpConnectionPtr, replyFlag)) {
                        _responderState.sentClose(closeErrorFlag);
                        _initiatorState.receivedClose(false);
                        _lastActor = LastActorType_t::RECEIVER;
                    } else {
                        throw amqpAnalyze::Error(MSG("Connection::handleFrame(): TCP address \"" << tcpConnectionPtr
                                        << "\" matches neither initiator nor responder"));
                    }
                    oss << _initiatorState.stateStr() << ":" <<  _responderState.stateStr();
                    amqpClosePtr->setStateStr(oss.str());
                }
                break;
            case PerformativeType_t::BEGIN:
                {
                    AmqpBegin* amqpBeginPtr(dynamic_cast<AmqpBegin*>(performativePtr));
                    if (amqpBeginPtr == nullptr) {
                        throw amqpAnalyze::Error(MSG("Connection::handleFrame(): Error downcasting class \"Performative\" to class \"AmqpBegin\": found=\""
                                        << performativePtr->name() << "\""));
                    }
/*
                    Session* sessionPtr = nullptr;
                    std::map<uint16_t, SessionHanlder*>::iterator itr=_initiatorEndpoints.lower_bound(framePtr->typeSpecific());
                    if (itr != _initiatorEndpoints.end() && !(_initiatorEndpoints.key_comp()(framePtr->typeSpecific(), itr->first))) {
                        sessionPtr = itr->second; ???
                    } else {
                        sessionPtr = new Session();
                    }
*/
                }
                break;
            case PerformativeType_t::END:
                {
                    AmqpEnd* amqpEndPtr(dynamic_cast<AmqpEnd*>(performativePtr));
                    if (amqpEndPtr == nullptr) {
                        throw amqpAnalyze::Error(MSG("Connection::handleFrame(): Error downcasting class \"Performative\" to class \"AmqpEnd\": found=\""
                                        << performativePtr->name() << "\""));
                    }
                }
                break;
            default:;
                SessionHandler::handleFrame(tcpConnectionPtr, framePtr);
            }
        }

        void Connection::handleProtocolHeader(const TcpConnection* tcpConnectionPtr, bool replyFlag, ProtocolHeader* protocolHeaderPtr) {
            if (isInitiator(tcpConnectionPtr, replyFlag)) {
                _initiatorState.sentProtocolHeader(protocolHeaderPtr->version());
                _responderState.receivedProtocolHeader(protocolHeaderPtr->version());
                _lastActor = LastActorType_t::INITIATOR;
            } else if (isResponder(tcpConnectionPtr, replyFlag)) {
                _responderState.sentProtocolHeader(protocolHeaderPtr->version());
                _initiatorState.receivedProtocolHeader(protocolHeaderPtr->version());
                _lastActor = LastActorType_t::RECEIVER;
            } else {
                throw amqpAnalyze::Error(MSG("Connection::handleProtocolHeader(): TCP address \"" << tcpConnectionPtr
                                << "\" matches neither initiator nor responder"));
            }
            std::ostringstream oss;
            oss << _initiatorState.stateStr() << ":" <<  _responderState.stateStr();
            protocolHeaderPtr->setStateStr(oss.str());
        }

        bool Connection::handleTcpClose(const TcpConnection* tcpConnectionPtr, bool replyFlag) {
            if (isInitiator(tcpConnectionPtr, replyFlag)) {
                if (_responderTcpClosedFlag) return true;
                _initiatorTcpClosedFlag = true;
            } else if (isResponder(tcpConnectionPtr, replyFlag)) {
                if (_initiatorTcpClosedFlag) return true;
                _responderTcpClosedFlag = true;
            } else {
                throw amqpAnalyze::Error(MSG("Connection::handleTcpClose(): TCP address \"" << tcpConnectionPtr
                                << "\" matches neither initiator nor responder"));
            }
            return false;
        }

        void Connection::checkChannel(const Frame* framePtr) {
            const uint16_t channel(framePtr->typeSpecific());
            if ((_initiatorOpen == nullptr || _responderOpen == nullptr) && channel != 0) {
                _errorList.push_back(new ConnectionError(MSG("ConnectionError: Channel number > 0 before open complete, found channel=0x" << std::hex << channel)));
            }
        }

        bool Connection::checkError(const AmqpClose* amqpClosePtr) {
            const AmqpErrorRecord* errorRecordPtr = amqpClosePtr->error(false);
            if (errorRecordPtr == nullptr) return false;
            std::ostringstream oss;
            errorRecordPtr->appendString(oss, 4, false);
            _closeError.assign(oss.str());
            return true;
/*
            const AmqpList* fieldList = amqpClosePtr->fieldList();
            if (fieldList != nullptr && fieldList->value().size() > 0) {
                if (fieldList->value().size() > 1) {
                    throw amqpAnalyze::Error(MSG("Too many fields in AMQP close: expected 1, found " << fieldList->value().size()));
                }
                const Type* typePtr = fieldList->value().front();
                if (!typePtr->isNull()) { // ignore null errors
                    const AmqpString* errorMsgPtr(dynamic_cast<const AmqpString*>(typePtr));
                    if (errorMsgPtr == nullptr) {
                        throw amqpAnalyze::Error(MSG("Connection::handleFrame(): Error downcasting class \"Type\" to class \"AmqpString\": found=\""
                                        << typePtr->typeStr() << "\""));
                    }
                    _closeError.assign(errorMsgPtr->value());
                    return true;
                }
            }
            return false;
*/
        }

        bool Connection::isInitiator(const TcpConnection* tcpConnectionPtr, bool replyFlag) const {
            // TODO: Comparing strings like this could be an inefficient way to determine initiator vs. responder
            if (replyFlag) {
                return tcpConnectionPtr->destAddress().compare(_initiatorAddrStr) == 0;
            }
            return tcpConnectionPtr->srcAddress().compare(_initiatorAddrStr) == 0;
        }

        bool Connection::isResponder(const TcpConnection* tcpConnectionPtr, bool replyFlag) const {
            // TODO: Comparing strings like this could be an inefficient way to determine initiator vs. responder
            if (replyFlag) {
                return tcpConnectionPtr->destAddress().compare(_responderAddrStr) == 0;
            }
            return tcpConnectionPtr->srcAddress().compare(_responderAddrStr) == 0;
        }

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */
