/*
 * FieldType.cpp
 *
 *  Created on: Apr 14, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/amqp10/FieldType.hpp>

namespace amqpAnalyze
{
    namespace amqp10
    {

        FieldType::FieldType(const char* fieldName,
                             AmqpPrimitiveType_t primitiveType,
                             bool mandatoryFlag,
                             bool multipleFlag,
                             std::initializer_list<AmqpRequiresProvides_t> _requiresInitList):
            _fieldName(fieldName),
            _unionType(FieldType_t::PRIMITIVE),
            _types(primitiveType),
            _mandatoryFlag(mandatoryFlag),
            _multipleFlag(multipleFlag),
            _requiresList(_requiresInitList)
        {}

        FieldType::FieldType(const char* fieldName,
                             const char* type,
                             bool mandatoryFlag,
                             bool multipleFlag,
                             std::initializer_list<AmqpRequiresProvides_t> _requiresInitList):
            _fieldName(fieldName),
            _unionType(FieldType_t::WILDCARD),
            _types(type),
            _mandatoryFlag(mandatoryFlag),
            _multipleFlag(multipleFlag),
            _requiresList(_requiresInitList)
        {}

        FieldType::~FieldType() {}

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */
