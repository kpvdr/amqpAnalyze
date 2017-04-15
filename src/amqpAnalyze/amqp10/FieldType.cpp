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
                                     amqpPrimitiveType_t primitiveType,
                                     bool mandatoryFlag,
                                     bool multipleFlag,
                                     std::initializer_list<amqpRequiresProvides_t> _requiresInitList):
            _fieldName(fieldName),
            _types(primitiveType),
            _mandatoryFlag(mandatoryFlag),
            _multipleFlag(multipleFlag),
            _requiresList(_requiresInitList)
        {}

        FieldType::FieldType(const char* fieldName,
                                     amqpCompositeType_t compositeType,
                                     bool mandatoryFlag,
                                     bool multipleFlag,
                                     std::initializer_list<amqpRequiresProvides_t> _requiresInitList):
            _fieldName(fieldName),
            _types(compositeType),
            _mandatoryFlag(mandatoryFlag),
            _multipleFlag(multipleFlag),
            _requiresList(_requiresInitList)
        {}

        FieldType::FieldType(const char* fieldName,
                                     char wildcard,
                                     bool mandatoryFlag,
                                     bool multipleFlag,
                                     std::initializer_list<amqpRequiresProvides_t> _requiresInitList):
            _fieldName(fieldName),
            _types(wildcard),
            _mandatoryFlag(mandatoryFlag),
            _multipleFlag(multipleFlag),
            _requiresList(_requiresInitList)
        {}

        FieldType::~FieldType() {}

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */
