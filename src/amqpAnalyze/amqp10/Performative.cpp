/*
 * Performative.cpp
 *
 *  Created on: Apr 6, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/amqp10/Performative.hpp>

#include <amqpAnalyze/Error.hpp>

// debug
//#include <iomanip>
//#include <iostream>
//#include <sstream>

namespace amqpAnalyze
{
    namespace amqp10
    {

        //static
        std::map<performatve_e, const char*> Performative::s_performativeName = {{OPEN, "open"},
                                                                                 {BEGIN, "begin"},
                                                                                 {ATTACH, "attach"},
                                                                                 {FLOW, "flow"},
                                                                                 {TRANSFER, "transfer"},
                                                                                 {DISPOSITION, "disposition"},
                                                                                 {DETACH, "detach"},
                                                                                 {END, "end"},
                                                                                 {CLOSE, "close"}};

        Performative::Performative(AmqpList* fieldList):
            _fieldList(fieldList)
        {}
        Performative::~Performative() {
            if (_fieldList != nullptr) {
                delete _fieldList;
                _fieldList = nullptr;
            }
        }
        const char* Performative::name() const {
            return s_performativeName[descriptor()];
        }
        // static
        Performative* Performative::decode(const char* dataPtr, std::size_t dataLength) {
            Buffer buffer(dataPtr, dataLength);
            //std::cout << "decode() coffs=" << buffer.getOffset() << std::endl;
            uint8_t domain(buffer.getUint8());
            if (domain != 0) { // TODO: figure out what these could be and handle them appropriately
                throw amqpAnalyze::Error(MSG("Non-zero domain: 0x" << std::hex << (int)domain));
            }
            //std::cout << "domain coffs=" << buffer.getOffset() << std::endl;
            // Read the code, then the field list
            AmqpYType* performative = AmqpYType::decode(buffer);
            checkType(performative, ULONG_TYPE);
            //std::cout << "performative " << performative->toString() << " coffs=" << buffer.getOffset() << std::endl;
            AmqpYType* fieldList = AmqpYType::decode(buffer);
            checkType(fieldList, LIST_TYPE);

            switch (((AmqpUlong*)performative)->value()) {
            case OPEN: return new Open((AmqpList*)fieldList);
            case BEGIN: return new Begin((AmqpList*)fieldList);
            case ATTACH: return new Attach((AmqpList*)fieldList);
            case FLOW: return new Flow((AmqpList*)fieldList);
            case TRANSFER: return new Transfer((AmqpList*)fieldList);
            case DISPOSITION: return new Disposition((AmqpList*)fieldList);
            case DETACH: return new Detach((AmqpList*)fieldList);
            case END: return new End((AmqpList*)fieldList);
            case CLOSE: return new Close((AmqpList*)fieldList);
            default: throw amqpAnalyze::Error(MSG("Unknown performative descriptor: 0x" << ((AmqpUlong*)performative)->value()));
            }
        }


        Open::Open(AmqpList* fieldList): Performative(fieldList) {}
        Open::~Open() {}


        Begin::Begin(AmqpList* fieldList): Performative(fieldList) {}
        Begin::~Begin() {}


        Attach::Attach(AmqpList* fieldList): Performative(fieldList) {}
        Attach::~Attach() {}


        Flow::Flow(AmqpList* fieldList): Performative(fieldList) {}
        Flow::~Flow() {}


        Transfer::Transfer(AmqpList* fieldList): Performative(fieldList) {}
        Transfer::~Transfer() {}


        Disposition::Disposition(AmqpList* fieldList): Performative(fieldList) {}
        Disposition::~Disposition() {}


        Detach::Detach(AmqpList* fieldList): Performative(fieldList) {}
        Detach::~Detach() {}


        End::End(AmqpList* fieldList): Performative(fieldList) {}
        End::~End() {}


        Close::Close(AmqpList* fieldList): Performative(fieldList) {}
        Close::~Close() {}


    } /* namespace amqp10 */
} /* namespace amqpAnalyze */
