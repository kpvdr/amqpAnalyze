/*
 * Decoder.hpp
 *
 *  Created on: Apr 18, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_AMQP10_DECODER_HPP_
#define SRC_AMQPANALYZE_AMQP10_DECODER_HPP_

#include <cstdint>
#include <vector>

namespace amqpAnalyze
{
    namespace amqp10
    {
        class AmqpList;
        class CompositeType;
        class FieldType;
        typedef std::vector<FieldType> fieldTypeList_t;
        class FrameBuffer;
        class Type;
        class Performative;
        class PrimitiveType;
        class Section;

        class Decoder
        {
        public:
            Decoder();
            virtual ~Decoder();

            // Decode AMQP type
            static Type* decode(FrameBuffer& frameBuffer);
            static PrimitiveType* decodePrimitive(uint8_t code, FrameBuffer& frameBuffer, const char* fieldName = nullptr);
            static CompositeType* decodeComposite(FrameBuffer& frameBuffer, const char* fieldName = nullptr);

            // Decode AMQP performative
            static Performative* decodePerformative(FrameBuffer& frameBuffer);

            // Decode AMQP section
            static Section* decodeSection(FrameBuffer& frameBuffer);

        protected:
            static AmqpList* decodeFieldList(FrameBuffer& frameBuffer, fieldTypeList_t& fieldTypeList);
            static AmqpList* decodeFieldList(std::size_t size, std::size_t count, FrameBuffer& frameBuffer, fieldTypeList_t& fieldTypeList);
            static Type* decodeField(FrameBuffer& frameBuffer, const FieldType& fieldType);
       };

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQP10_DECODER_HPP_ */
