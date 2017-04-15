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
                          amqpPrimitiveType_t primitiveType,
                          bool mandatoryFlag,
                          bool multipleFlag,
                          std::initializer_list<amqpRequiresProvides_t> _requiresInit = {});
            FieldType(const char* fieldName,
                          amqpCompositeType_t compositeType,
                          bool mandatoryFlag,
                          bool multipleFlag,
                          std::initializer_list<amqpRequiresProvides_t> _requiresInit = {});
            FieldType(const char* fieldName,
                          char wildcard,
                          bool mandatoryFlag,
                          bool multipleFlag,
                          std::initializer_list<amqpRequiresProvides_t> _requiresInit);
            virtual ~FieldType();
        protected:
            const char* _fieldName;
            union ctype {
                const amqpPrimitiveType_t _primitiveType;
                const amqpCompositeType_t _compositeType;
                const char _wildcard;
                ctype(amqpPrimitiveType_t primitiveType): _primitiveType(primitiveType) {}
                ctype(amqpCompositeType_t compositeType): _compositeType(compositeType) {}
                ctype(char wildcard): _wildcard(wildcard) {}
            } _types;
            const bool _mandatoryFlag;
            const bool _multipleFlag;
            std::vector<amqpRequiresProvides_t> _requiresList;
        };

        typedef std::vector<FieldType> fieldTypeList_t;
        typedef fieldTypeList_t::const_iterator fieldTypeList_citr_t;

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQP10_FIELDTYPE_HPP_ */
