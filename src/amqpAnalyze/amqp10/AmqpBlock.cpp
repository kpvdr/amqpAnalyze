/*
 * AmqpBlock.cpp
 *
 *  Created on: Apr 18, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/amqp10/AmqpBlock.hpp>

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

        std::ostringstream& AmqpBlock::appendStringEpilog(std::ostringstream& oss, std::size_t margin, bool colorFlag) const {
            for (ErrorPtrListCitr_t i=_errorPtrList.cbegin(); i!=_errorPtrList.cend(); ++i) {
                oss << "\n" << std::string(margin, ' ') << (*i)->formattedMessage(colorFlag);
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

        uint64_t AmqpBlock::packetNum() const {
            return _packetNum;
        }


    } /* namespace amqp10 */
} /* namespace amqpAnalyze */
