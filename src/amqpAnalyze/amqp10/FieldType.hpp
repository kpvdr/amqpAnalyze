/*
 * FieldType.hpp
 *
 *  Created on: Apr 14, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_AMQP10_FIELDTYPE_HPP_
#define SRC_AMQPANALYZE_AMQP10_FIELDTYPE_HPP_

#include <amqpAnalyze/amqp10/Type.hpp>

namespace amqpAnalyze
{
    namespace amqp10
    {

        class FieldType
        {
        public:
            FieldType(const char* fieldName,
                      AmqpPrimitiveType_t primitiveType,
                      bool mandatoryFlag,
                      bool multipleFlag,
                      std::initializer_list<AmqpRequiresProvides_t> _requiresInit = {});
            FieldType(const char* fieldName,
                      const char* type,
                      bool mandatoryFlag,
                      bool multipleFlag,
                      std::initializer_list<AmqpRequiresProvides_t> _requiresInit = {});
            virtual ~FieldType();
            enum class FieldType_t {PRIMITIVE, WILDCARD};
            const char* _fieldName;
            FieldType_t _unionType;
            union ctype {
                const AmqpPrimitiveType_t _primitiveType;
                const char* _type;
                ctype(AmqpPrimitiveType_t primitiveType): _primitiveType(primitiveType) {}
                ctype(const char* type): _type(type) {}
            } _types;
            const bool _mandatoryFlag;
            const bool _multipleFlag;
            AmqpProvidesRequiresList_t _requiresList;
        };

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQP10_FIELDTYPE_HPP_ */
