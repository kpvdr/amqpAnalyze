/*
 * FieldType.hpp
 *
 *  Created on: Apr 14, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_AMQP10_FIELDTYPE_HPP_
#define SRC_AMQPANALYZE_AMQP10_FIELDTYPE_HPP_

#include <amqpAnalyze/amqp10/ProvidesRequires.hpp>
#include <amqpAnalyze/amqp10/Type.hpp>

namespace amqpAnalyze
{
    namespace amqp10
    {

        class FieldType
        {
        public:
            FieldType(const char* fieldName,
                      amqpPrimitiveType_t primitiveType,
                      bool mandatoryFlag,
                      bool multipleFlag,
                      std::initializer_list<amqpRequiresProvides_t> _requiresInit = {});
            FieldType(const char* fieldName,
                      const char* type,
                      bool mandatoryFlag,
                      bool multipleFlag,
                      std::initializer_list<amqpRequiresProvides_t> _requiresInit = {});
            virtual ~FieldType();
            typedef enum class type {PRIMITIVE, /*COMPOSITE,*/ WILDCARD} type_t;
            const char* _fieldName;
            type_t _unionType;
            union ctype {
                const amqpPrimitiveType_t _primitiveType;
                //const amqpCompositeType_t _compositeType;
                const char* _type;
                ctype(amqpPrimitiveType_t primitiveType): _primitiveType(primitiveType) {}
                //ctype(amqpCompositeType_t compositeType): _compositeType(compositeType) {}
                ctype(const char* type): _type(type) {}
            } _types;
            const bool _mandatoryFlag;
            const bool _multipleFlag;
            amqp_provides_requires_list_t _requiresList;
        };

        typedef std::vector<FieldType> fieldTypeList_t;
        typedef fieldTypeList_t::const_iterator fieldTypeList_citr_t;

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQP10_FIELDTYPE_HPP_ */
