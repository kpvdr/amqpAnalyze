/*
 * ConnectionHandler.cpp
 *
 *  Created on: Apr 24, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/amqp10/ConnectionHandler.hpp>

#include <amqpAnalyze/amqp10/Connection.hpp>
#include <amqpAnalyze/amqp10/Frame.hpp>
#include <amqpAnalyze/amqp10/IllegalStateError.hpp>
#include <amqpAnalyze/amqp10/Performative.hpp>
#include <amqpAnalyze/amqp10/ProtocolHeader.hpp>
#include <amqpAnalyze/TcpAddressInfo.hpp>
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

        void ConnectionHandler::handleFrame(const struct TcpAddressInfo& tcpAddrInfo, AmqpBlock* blockPtr) {
            if (std::strcmp(blockPtr->name(), "ProtocolHeader") == 0) {
                // TODO: Create a helper fn or #define for this downcast and check
                ProtocolHeader* protocolHeaderPtr = dynamic_cast<ProtocolHeader*>(blockPtr);
                if (protocolHeaderPtr == nullptr)  {
                    throw amqpAnalyze::Error(MSG("ConnectionHandler::handleFrame(): Error in class ProtocolHeader downcast"));
                }
                insertIfNotPresent(tcpAddrInfo)->handleProtocolHeader(tcpAddrInfo, protocolHeaderPtr);
            } else  if (std::strcmp(blockPtr->name(), "Frame") == 0) {
                Frame* framePtr = dynamic_cast<Frame*>(blockPtr);
                if (framePtr == nullptr) {
                    throw amqpAnalyze::Error(MSG("ConnectionHandler::handleFrame(): Error in class Frame downcast"));
                }
                try { _connectionMap.at(tcpAddrInfo._hash)->handleFrame(tcpAddrInfo, framePtr); }
                catch (const std::out_of_range& e) {
                    framePtr->addError(new amqpAnalyze::Error(MSG("ConnectionHandler::handleFrame(): Address " << tcpAddrInfo << " not in _connectionMap")));
                }
                catch (const IllegalStateError* ePtr) {
                    framePtr->addError(ePtr);
                }
            } else {
                throw amqpAnalyze::Error(MSG("ConnectionHandler::handleFrame(): Unexpected block type: \"" << blockPtr->name() << "\""));
            }
        }

        void ConnectionHandler::tcpClose(const struct TcpAddressInfo& tcpAddrInfo) {
            Connection* connectioPtr = nullptr;
            try { connectioPtr = _connectionMap.at(tcpAddrInfo._hash); }
            catch (const std::out_of_range& e) {
                throw amqpAnalyze::Error(MSG("ConnectionHandler::tcpClose(): Address " << tcpAddrInfo << " not in _connectionMap"));
            }
            if (connectioPtr->handleTcpClose(tcpAddrInfo)) {
                delete connectioPtr;
                const std::size_t numErased = _connectionMap.erase(tcpAddrInfo._hash);
                if (numErased != 1) {
                    throw amqpAnalyze::Error(MSG("ConnectionHandler::tcpClose(): map error: _connectionMap.erase() returned "
                                             << numErased << " (src=" << tcpAddrInfo._srcAddrStr << " dest=" << tcpAddrInfo._destAddrStr
                                             << ")"));
                }
            }
        }

        Connection*  ConnectionHandler::insertIfNotPresent(const struct TcpAddressInfo& tcpAddrInfo) {
            // Efficient check-before-insert:
            // See http://stackoverflow.com/questions/97050/stdmap-insert-or-stdmap-find for the following:
            std::map<uint64_t, Connection*>::iterator itr = _connectionMap.lower_bound(tcpAddrInfo._hash);
            if (itr != _connectionMap.end() && !(_connectionMap.key_comp()(tcpAddrInfo._hash, itr->first))) {
                return itr->second;
            } else {
                Connection* cPtr = new Connection(tcpAddrInfo);
                _connectionMap.emplace(tcpAddrInfo._hash, cPtr);
                return cPtr;
            }
        }

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */
