/*
 * ConnectionHandler.cpp
 *
 *  Created on: Apr 24, 2017
 *      Author: kpvdr
 */

#include "ConnectionHandler.hpp"

#include <amqpAnalyze/amqp10/Connection.hpp>
#include <amqpAnalyze/amqp10/Frame.hpp>
#include <amqpAnalyze/amqp10/IllegalStateError.hpp>
#include <amqpAnalyze/amqp10/Performative.hpp>
#include <amqpAnalyze/amqp10/ProtocolHeader.hpp>
#include <amqpAnalyze/TcpConnection.hpp>
#include <amqpAnalyze/TcpDissector.hpp>
#include <cstring>
#include <typeinfo>

// Global AMQP connection handler
amqpAnalyze::amqp10::ConnectionHandler g_amqpConnectionHandler;

namespace amqpAnalyze
{
    namespace amqp10
    {

        ConnectionHandler::ConnectionHandler() {}

        ConnectionHandler::~ConnectionHandler() {
            for (std::map<uint64_t, Connection*>::iterator i=_connectionMap.begin(); i!=_connectionMap.end(); ++i) {
                delete i->second;
            }
            _connectionMap.clear();
        }

        void ConnectionHandler::handleFrame(const TcpConnection* tcpConnectionPtr, bool replyFlag, AmqpBlock* blockPtr) {
            if (std::strcmp(blockPtr->name(), "ProtocolHeader") == 0) {
                ProtocolHeader* protocolHeaderPtr = dynamic_cast<ProtocolHeader*>(blockPtr);
                if (protocolHeaderPtr == nullptr)  {
                    throw amqpAnalyze::Error(MSG("ConnectionHandler::handleFrame(): Error in class ProtocolHeader downcast"));
                }
                try { insertIfNotPresent(tcpConnectionPtr)->handleProtocolHeader(tcpConnectionPtr, replyFlag, protocolHeaderPtr); }
                catch (const IllegalStateError* ePtr) {
                    protocolHeaderPtr->addError(ePtr);
                }
            } else  if (std::strcmp(blockPtr->name(), "Frame") == 0) {
                Frame* framePtr = dynamic_cast<Frame*>(blockPtr);
                if (framePtr == nullptr) {
                    throw amqpAnalyze::Error(MSG("ConnectionHandler::handleFrame: Error in class Frame downcast"));
                }
                try { _connectionMap.at(tcpConnectionPtr->hash())->handleFrame(tcpConnectionPtr, replyFlag, framePtr); }
                catch (const std::out_of_range& e) {
                    framePtr->addError(new amqpAnalyze::Error(MSG("ConnectionHandler::handleFrame: AMQP connection map: address not seen, possible missing previous AMQP frames. ("
                                    << tcpConnectionPtr << ")")));
                }
                catch (const IllegalStateError* ePtr) {
                    framePtr->addError(ePtr);
                }
            } else {
                throw amqpAnalyze::Error(MSG("ConnectionHandler::handleFrame: Unexpected block type: \"" << blockPtr->name() << "\""));
            }
        }

        void ConnectionHandler::tcpClose(TcpDissector* tcpDissectorPtr) {
            const TcpConnection* tcpConnectionPtr = tcpDissectorPtr->tcpConnection();
            Connection* connectionPtr = nullptr;
            try { connectionPtr = _connectionMap.at(tcpConnectionPtr->hash()); }
            catch (const std::out_of_range& e) {
                tcpDissectorPtr->addError(new amqpAnalyze::Error(MSG("ConnectionHandler::tcpClose: AMQP connection map: TCP close for address not seen, possible missing previous frames. ("
                                         << tcpConnectionPtr << ")")));
                return;
            }
            if (connectionPtr->handleTcpClose(tcpConnectionPtr, tcpDissectorPtr->replyFlag())) {
                delete connectionPtr;
                const std::size_t numErased = _connectionMap.erase(tcpConnectionPtr->hash());
                if (numErased != 1) {
                    tcpDissectorPtr->addError(new amqpAnalyze::Error(MSG("ConnectionHandler::tcpClose: AMQP connection map: address not seen, possible missing previous AMQP frames. ("
                                             << tcpConnectionPtr << ")")));
                }
            }
        }

        Connection*  ConnectionHandler::insertIfNotPresent(const TcpConnection* tcpConnectionPtr) {
            // Efficient check-before-insert:
            // See http://stackoverflow.com/questions/97050/stdmap-insert-or-stdmap-find for the following:
            std::map<uint64_t, Connection*>::iterator itr = _connectionMap.lower_bound(tcpConnectionPtr->hash());
            if (itr != _connectionMap.end() && !(_connectionMap.key_comp()(tcpConnectionPtr->hash(), itr->first))) {
                return itr->second;
            } else {
                Connection* cPtr = new Connection(tcpConnectionPtr);
                _connectionMap.emplace(tcpConnectionPtr->hash(), cPtr);
                return cPtr;
            }
        }

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */
