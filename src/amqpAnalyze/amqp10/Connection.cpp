/*
 * Connection.cpp
 *
 *  Created on: Apr 24, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/amqp10/Connection.hpp>

#include <amqpAnalyze/amqp10/Endpoint.hpp>
#include <amqpAnalyze/amqp10/Frame.hpp>
#include <amqpAnalyze/amqp10/Performative.hpp>
#include <amqpAnalyze/amqp10/Type.hpp>
#include <amqpAnalyze/Error.hpp>
#include <amqpAnalyze/TcpAddressInfo.hpp>

namespace amqpAnalyze
{
    namespace amqp10
    {

        Connection::Connection(const TcpAddressInfo& tcpAddrInfo):
            _errorList(),
            _closeError(),
            _frameErrorFlag(false),
            _initiatorAddrStr(tcpAddrInfo._srcAddrStr),
            _initiatorCloseError(),
            _initiatorEndpoints(),
            _initiatorHeaderSent(false),
            _initiatorOpen(nullptr),
            _initiatorState("initiator"),
            _initiatorTcpClosedFlag(false),
            _responderAddrStr(tcpAddrInfo._destAddrStr),
            _responderCloseError(),
            _responderEndpoints(),
            _responderHeaderSent(false),
            _responderOpen(nullptr),
            _responderState("responder"),
            _responderTcpClosedFlag(false),
            _lastActor(LastActorType_t::NONE)
        {}

        Connection::~Connection() {
            for (std::map<uint16_t, Endpoint*>::iterator i=_initiatorEndpoints.begin(); i!= _initiatorEndpoints.end(); ++i) {
                delete i->second;
            }
            _initiatorEndpoints.clear();
            for (std::map<uint16_t, Endpoint*>::iterator i=_responderEndpoints.begin(); i!= _responderEndpoints.end(); ++i) {
                delete i->second;
            }
            _responderEndpoints.clear();
            for (std::vector<amqpAnalyze::Error*>::iterator i=_errorList.begin(); i!=_errorList.end(); ++i) {
                delete *i;
            }
            _errorList.clear();
        }

        void Connection::handleFrame(const struct TcpAddressInfo& tcpAddrInfo, Frame* framePtr) {
            checkChannel(framePtr);
            if (!framePtr->isEmpty()) { // ignore heartbeats
                if (framePtr->hasErrors()) {
                    _frameErrorFlag = true;
                }
                Performative* performativePtr = framePtr->performative();
                if (performativePtr != nullptr) {
                    std::ostringstream oss;
                    switch (framePtr->performative()->type()) {
                    case PerformativeType_t::OPEN:
                        {
                            AmqpOpen* amqpOpenPtr(dynamic_cast<AmqpOpen*>(performativePtr));
                            if (isInitiator(tcpAddrInfo)) {
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
                            if (isInitiator(tcpAddrInfo)) {
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
                    default:;
                        // TODO: pass on these frames to Link handler
                    }
                }
            }
        }

        void Connection::handleProtocolHeader(const struct TcpAddressInfo& tcpAddrInfo, ProtocolHeader* protocolHeaderPtr) {
            if (isInitiator(tcpAddrInfo)) {
                _initiatorState.sentProtocolHeader(protocolHeaderPtr->version());
                _responderState.receivedProtocolHeader(protocolHeaderPtr->version());
                _lastActor = LastActorType_t::INITIATOR;
            } else {
                _responderState.sentProtocolHeader(protocolHeaderPtr->version());
                _initiatorState.receivedProtocolHeader(protocolHeaderPtr->version());
                _lastActor = LastActorType_t::RECEIVER;
            }
            std::ostringstream oss;
            oss << _initiatorState.stateStr() << ":" <<  _responderState.stateStr();
            protocolHeaderPtr->setStateStr(oss.str());
        }

        bool Connection::handleTcpClose(const struct TcpAddressInfo& tcpAddrInfo) {
            if (isInitiator(tcpAddrInfo)) {
                if (_responderTcpClosedFlag) return true;
                _initiatorTcpClosedFlag = true;
            } else {
                if (_initiatorTcpClosedFlag) return true;
                _responderTcpClosedFlag = true;
            }
            return false;
        }

        void Connection::checkChannel(const Frame* framePtr) {
            const uint16_t channel(framePtr->typeSpecific());
            if ((_initiatorOpen == nullptr || _responderOpen == nullptr) && channel != 0) {
                std::string errorMsg(MSG("Connection::handleFrame(): Channel number > 0 before open complete, found channel=0x" << std::hex << channel));
                _errorList.push_back(new amqpAnalyze::Error(errorMsg)); // TODO: create new error class for state analysis errors
                // std::cerr << errorMsg << "\n";
            }
        }

        bool Connection::checkError(const AmqpClose* amqpClosePtr) {
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
                }
            }
        }

        bool Connection::isInitiator(const TcpAddressInfo& tcpAddrInfo) const {
            return tcpAddrInfo._srcAddrStr.compare(_initiatorAddrStr) == 0; // TODO: this could be an inefficient way to determine initiator
        }

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */
