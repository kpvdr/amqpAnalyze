/*
 * FrameHeader.cpp
 *
 *  Created on: Apr 3, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/amqp10/Buffer.hpp>
#include <amqpAnalyze/amqp10/Frame.hpp>
#include <amqpAnalyze/amqp10/Type.hpp>
#include <amqpAnalyze/Error.hpp>

#include <iomanip>
#include <netinet/in.h>

namespace amqpAnalyze
{
    namespace amqp10
    {

        // static
        std::map<frameType_e, const char*> Frame::s_frameTypeName = {
            {AMQP_FRAME, "AMQP"},
            {SASL_FRAME, "SASL"}
        };

        Frame::hdr::hdr(const hdr* hdrPtr):
                _frameSize(::ntohl(hdrPtr->_frameSize)),
                _dataOffset(hdrPtr->_dataOffset),
                _type(hdrPtr->_type),
                _typeSpecific(::ntohs(hdrPtr->_typeSpecific))
        {}

        Frame::Frame(std::size_t frameOffset, const struct Frame::hdr* hdrPtr):
                FrameBase(frameOffset),
                _hdr(hdrPtr),
                _dataOffsetBytes(_hdr._dataOffset*4),
                _extendedHeaderSize(_dataOffsetBytes - 8),
                _performative(nullptr),
                _sectionList()
        {
            if (_hdr._frameSize < 8) throw amqpAnalyze::Error(MSG("Invalid AMQP frame size: 0x" << std::hex << _hdr._frameSize));
            if (_hdr._dataOffset < 2) throw amqpAnalyze::Error(MSG("Invalid AMQP frame data offset (doff): 0x" << std::hex << _hdr._dataOffset));
            switch (_hdr._type) {
            case 0: { // AMQP frame
                    Buffer amqpBuffer(((const char*)hdrPtr + _dataOffsetBytes), (_hdr._frameSize - _dataOffsetBytes));
                    _performative = (CompositeType*)Type::decode(amqpBuffer);
//                    while (!amqpBuffer.isEmpty()) {
//                        _sectionList.push_back(Type::decode(amqpBuffer));
//                    }
                }
                break;
            case 1: // SASL frame
                break;
            default:
                throw amqpAnalyze::Error(MSG("Invalid AMQP frame type: 0x" << std::hex << _hdr._type));
            }
        }

        Frame::~Frame() {
            if (_performative != nullptr) {
                delete _performative;
                _performative = nullptr;
            }
            for (std::vector<Type*>::iterator i=_sectionList.begin(); i!=_sectionList.end(); ++i) {
                delete *i;
            }
            _sectionList.clear();
        }

        void Frame::appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const {
            if (margin > 0 && !ignoreFirstMargin) oss << std::endl << std::string(margin, ' ');
            oss << "[" << std::setw(4) << std::setfill('0') << std::hex << _frameOffset
                << "] AMQP frame: size=0x" << _hdr._frameSize << " doff=0x" << (int)_hdr._dataOffset
                << " type=0x" << (int)_hdr._type << " (" << s_frameTypeName[_hdr._type] << ")";
            if (_hdr._type == 0) { // AMQP frame
                oss << " chnl=0x" << _hdr._typeSpecific;
            }
            if (_extendedHeaderSize > 0) {
                oss << " extHdrSize=0x" << _extendedHeaderSize;
            }
            oss << ": ";
            if (_performative == nullptr) {
                oss << "heartbeat";
            } else {
                oss <<  _performative->toString(margin + 9);
                for (std::vector<Type*>::const_iterator i=_sectionList.cbegin(); i!=_sectionList.cend(); ++i) {
                    oss << "section: " << (*i)->Type::typeValueStr();
                }
            }
        }

        std::size_t Frame::frameSize() const {
            return _hdr._frameSize;;
        }

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */
