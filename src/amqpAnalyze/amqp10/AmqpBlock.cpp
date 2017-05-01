/*
 * AmqpBlock.cpp
 *
 *  Created on: Apr 18, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/amqp10/AmqpBlock.hpp>

#include <amqpAnalyze/Options.hpp>

namespace amqpAnalyze
{
    namespace amqp10
    {

        AmqpBlock::AmqpBlock(uint64_t packetNum, std::size_t dataOffset):
            _packetNum(packetNum),
            _dataOffset(dataOffset),
            _errorPtrList()
        {}

        AmqpBlock::~AmqpBlock() {
            for (ErrorPtrListItr_t i=_errorPtrList.begin(); i!=_errorPtrList.end(); ++i) {
                delete (*i);
            }
            _errorPtrList.clear();
        }

        void AmqpBlock::addError(const amqpAnalyze::Error* errorPtr) {
            _errorPtrList.push_back(errorPtr);
        }

        std::ostringstream& AmqpBlock::appendStringEpilog(std::ostringstream& oss, std::size_t margin) const {
            for (ErrorPtrListCitr_t i=_errorPtrList.cbegin(); i!=_errorPtrList.cend(); ++i) {
                oss << "\n" << std::string(margin, ' ') << (*i)->formattedMessage(g_optionsPtr->s_colorFlag);
            }
            return oss;
        }

        std::size_t AmqpBlock::dataOffset() const {
            return _dataOffset;
        }

        const ErrorPtrList_t AmqpBlock::errorPtrList() const {
            return _errorPtrList;
        }

        bool AmqpBlock::hasErrors() const {
            return !_errorPtrList.empty();
        }

        const char* AmqpBlock::name() const {
            return s_AmqpBlockTypeNames[blockType()];
        }

        uint64_t AmqpBlock::packetNum() const {
            return _packetNum;
        }

        void AmqpBlock::setStateStr(const std::string& stateStr) {
            _stateStr.assign(stateStr);
        }

        // static
        std::map<AmqpBlockType_t, const char*> AmqpBlock::s_AmqpBlockTypeNames = {
            {AmqpBlockType_t::PROTOCOL_HEADER, "ProtocolHeader"},
            {AmqpBlockType_t::FRAME, "Frame"},
            {AmqpBlockType_t::FRAME_ERROR, "FrameError"},
            {AmqpBlockType_t::PERFORMATIVE, "Performative"},
            {AmqpBlockType_t::SECTION,  "Section"}
        };


    } /* namespace amqp10 */
} /* namespace amqpAnalyze */
