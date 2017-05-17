/*
 * Decoder.hpp
 *
 *  Created on: Apr 18, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_AMQP10_DECODER_HPP_
#define SRC_AMQPANALYZE_AMQP10_DECODER_HPP_

#include <amqpAnalyze/amqp10/FwdDecls.hpp>

namespace amqpAnalyze
{
    namespace amqp10
    {
        class AmqpList;
        class CompositeType;
        class FrameBuffer10;
        class Performative;
        class Section;

        class Decoder
        {
        public:
            Decoder();
            virtual ~Decoder();

            // Decode AMQP type
            static Type* decode(FrameBuffer10& frameBuffer);
            static PrimitiveType* decodePrimitive(uint8_t code, FrameBuffer10& frameBuffer, const char* fieldName = nullptr);
            static CompositeType* decodeComposite(FrameBuffer10& frameBuffer, const char* fieldName = nullptr);

            // Decode AMQP performative
            static Performative* decodePerformative(FrameBuffer10& frameBuffer);

            // Decode AMQP section
            static Section* decodeSection(FrameBuffer10& frameBuffer);

        protected:
            static AmqpList* decodeFieldList(FrameBuffer10& frameBuffer, const FieldTypeList_t& fieldTypeList);
            static AmqpList* decodeFieldList(std::size_t size, std::size_t count, FrameBuffer10& frameBuffer, const FieldTypeList_t& fieldTypeList);
            static Type* decodeField(FrameBuffer10& frameBuffer, const FieldType& fieldType);
       };

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQP10_DECODER_HPP_ */
