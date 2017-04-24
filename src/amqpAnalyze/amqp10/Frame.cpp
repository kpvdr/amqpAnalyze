/*
 * Frame.cpp
 *
 *  Created on: Apr 3, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/amqp10/Frame.hpp>

#include <amqpAnalyze/amqp10/Decoder.hpp>
#include <amqpAnalyze/amqp10/FrameBuffer.hpp>
#include <amqpAnalyze/amqp10/FrameError.hpp>
#include <amqpAnalyze/amqp10/Performative.hpp>
#include <amqpAnalyze/amqp10/Section.hpp>
#include <iomanip>
#include <netinet/in.h>
#include <std/AnsiTermColors.hpp>

namespace amqpAnalyze
{
    namespace amqp10
    {

        Frame::hdr::hdr(const struct hdr* hdrPtr) :
            _frameSize(::ntohl(hdrPtr->_frameSize)),
            _doff(hdrPtr->_doff),
            _type(hdrPtr->_type),
            _typeSpecific(::ntohl(hdrPtr->_typeSpecific))
        {}

        Frame::Frame(FrameBuffer& frameBuffer):
            AmqpBlock(frameBuffer.getPacketNum(), frameBuffer.pushFrameOffsetSnapshot()),
            _hdr((hdr*)frameBuffer.getStructPtr(sizeof(hdr))),
            _extendedHeaderSize((_hdr._doff * 4) - 8),
            _extendedHeader((char*)frameBuffer.getStructPtr(_extendedHeaderSize)),
            _performative(nullptr),
            _sectionPtrList()
        {
            if (!frameBuffer.empty()) {
                try {
                    _performative = Decoder::decodePerformative(frameBuffer);
                    while (frameBuffer.getOffset() - _dataOffset < _hdr._frameSize) {
                        try {
                            _sectionPtrList.push_back(Decoder::decodeSection(frameBuffer));
                        } catch (const amqpAnalyze::Error& e) {
                            _sectionPtrList.push_back(new FrameError(frameBuffer.getPacketNum(), frameBuffer.getFrameOffsetSnapshot(), e));
                            frameBuffer.popFrameOffsetSnapshot();
                            frameBuffer.ignore(_hdr._frameSize - (frameBuffer.getOffset() - _dataOffset)); // ignore rest of frame
                            break;
                        }
                    }
                } catch (const amqpAnalyze::Error& e) {
                    _performative = new FrameError(frameBuffer.getPacketNum(), frameBuffer.getFrameOffsetSnapshot(), e);
                    frameBuffer.popFrameOffsetSnapshot();
                    frameBuffer.ignore(_hdr._frameSize - (frameBuffer.getOffset() - _dataOffset)); // ignore rest of frame
                }
            }
            frameBuffer.popFrameOffsetSnapshot();
        }

        Frame::~Frame() {
            if (_performative != nullptr) {
                delete _performative;
                _performative = nullptr;
            }
            for (AmqpBlockListCitr_t i=_sectionPtrList.begin(); i!=_sectionPtrList.end(); ++i) {
                delete *i;
            }
            _sectionPtrList.clear();
        }

        std::ostringstream& Frame::appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin, bool colorFlag) const {
            std::string m(margin, ' ');
            if (margin > 0 && !ignoreFirstMargin) oss << "\n" << m;
            oss << "[" << std::setw(4) << std::setfill('0') << std::hex << _dataOffset  << "] f ";
            oss << COLOR(FGND_BCYN, "AMQP frame", colorFlag) << ": size=0x" << _hdr._frameSize << " doff=0x" << (int)_hdr._doff;
            oss << " type=0x" << (int)_hdr._type << " (" << s_frameTypeName[_hdr._type] << ")";
            oss << (_hdr._type == 0 ? " chnl=0x" : " typeSpecific=0x") << _hdr._typeSpecific;
            if (_extendedHeaderSize > 0) {
                oss << " extHdrSize=0x" << _extendedHeaderSize;
            }
            oss << ":";
            if (_performative == nullptr) oss << " heartbeat";
            for (ErrorPtrListCitr_t i=_errorPtrList.cbegin(); i!=_errorPtrList.cend(); ++i) {
                oss << "\n" << m << (*i)->formattedMessage(colorFlag);
            }
            if (_performative != nullptr) {
                _performative->appendString(oss, margin, false, colorFlag);
                for (AmqpBlockListCitr_t i=_sectionPtrList.cbegin(); i!=_sectionPtrList.cend(); ++i) {
                    (*i)->appendString(oss, margin, false, colorFlag);
                }
            }
            return appendStringEpilog(oss, margin + 9, colorFlag);
        }

        uint8_t Frame::doff() const {
            return _hdr._doff;
        }

        uint32_t Frame::doffBytes() const {
            return _hdr._doff * 4;
        }

        const std::string& Frame::extendedHeader() const {
            return _extendedHeader;
        }

        std::size_t Frame::extendedHeaderSize() const {
            return _extendedHeaderSize;
        }

        uint32_t Frame::frameSize() const {
            return _hdr._frameSize;
        }

        FrameType_t Frame::frameType() const {
            return _hdr._type;
        }

        const AmqpBlockList_t& Frame::sectionPtrList() const {
            return _sectionPtrList;
        }

        uint16_t Frame::typeSpecific() const {
            return _hdr._typeSpecific;
        }

        void Frame::validate() {
            if (_extendedHeaderSize > 0) {
                addError(new amqpAnalyze::AmqpValidationError(_packetNum, _dataOffset, MSG("Extended header of size 0x"
                                << std::hex << _extendedHeaderSize << " found: reserved for future use, should be size 0")));
            }
            if (_performative != nullptr) {
                _performative->validate();
            }
            for (AmqpBlockListItr_t i=_sectionPtrList.begin(); i!=_sectionPtrList.end(); ++i) {
                (*i)->validate();
            }
        }

        // static
        std::map<FrameType_t, const char*> Frame::s_frameTypeName = {
            {AMQP_FRAME, "AMQP"},
            {SASL_FRAME, "SASL"}
        };

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */
