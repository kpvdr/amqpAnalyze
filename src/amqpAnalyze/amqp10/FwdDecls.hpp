/*
 * FwdDecls.hpp
 *
 *  Created on: Apr 24, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_AMQP10_FWDDECLS_HPP_
#define SRC_AMQPANALYZE_AMQP10_FWDDECLS_HPP_

//#include <amqpAnalyze/FwdDecls.hpp>
#include <array>
#include <map>
#include <vector>

namespace amqpAnalyze
{
    class Error;
    class TcpConnectionInfo;

    namespace amqp10
    {

        class AmqpBlock;
        typedef std::vector<AmqpBlock*> AmqpBlockList_t;
        typedef AmqpBlockList_t::iterator AmqpBlockListItr_t;
        typedef AmqpBlockList_t::const_iterator AmqpBlockListCitr_t;

        // Function pointer to error handler AmqpBlock::addError
        typedef void (AmqpBlock::*addErrorFp)(const amqpAnalyze::Error*);

        enum class AmqpRequiresProvides_t;
        typedef std::vector<AmqpRequiresProvides_t> AmqpProvidesRequiresList_t;
        typedef AmqpProvidesRequiresList_t::const_iterator AmqpProvidesRequiresListCitr_t;

        class ConnectionHandler;
        typedef void (ConnectionHandler::*tcpCloseCallbackFp)(const TcpConnectionInfo*);

        class FieldType;
        typedef std::vector<FieldType> FieldTypeList_t;

        //--- AMQP primitive types ---

        typedef std::vector<uint8_t> AmqpBinary_t;
        typedef std::array<uint8_t, 4> AmqpDecimal32_t;
        typedef std::array<uint8_t, 8> AmqpDecimal64_t;
        typedef std::array<uint8_t, 16> AmqpDecimal128_t;
        typedef std::array<uint8_t, 16> AmqpUuid_t;


        //--- AMQP compound types ---

        class Type;
        typedef std::vector<Type*> AmqpList_t;
        typedef AmqpList_t::iterator AmqpListItr_t;
        typedef AmqpList_t::const_iterator AmqpListCitr_t;

        class PrimitiveType;
        typedef std::map<PrimitiveType*, PrimitiveType*> AmqpMap_t;
        typedef AmqpMap_t::iterator AmqpMapItr_t;
        typedef AmqpMap_t::const_iterator AmqpMapCitr_t;

        typedef std::vector<PrimitiveType*> AmqpArray_t;
        typedef AmqpArray_t::iterator AmqpArrayItr_t;
        typedef AmqpArray_t::const_iterator AmqpArrayCitr_t;


        //--- AMQP restricted types ---

        typedef std::vector<std::string> AmqpConnectionErrorList_t;
        typedef AmqpConnectionErrorList_t::const_iterator AmqpConnectionErrorListCitr_t;

        typedef std::vector<std::string> AmqpErrorList_t;
        typedef AmqpErrorList_t::const_iterator AmqpErrorListCitr_t;

        typedef std::vector<std::string> AmqpLinkErrorList_t;
        typedef AmqpLinkErrorList_t::const_iterator AmqpLinkErrorListCitr_t;

        typedef std::vector<std::string> AmqpSessionErrorList_t;
        typedef AmqpSessionErrorList_t::const_iterator AmqpSessionErrorListCitr_t;

        typedef std::vector<std::string> AmqpStdDistModeList_t;
        typedef AmqpStdDistModeList_t::const_iterator AmqpStdDistModeListCitr_t;

        typedef std::vector<std::string> AmqpTerminusExpiryPolicyList_t;
        typedef AmqpTerminusExpiryPolicyList_t::const_iterator AmqpTerminusExpiryPolicyListCitr_t;

        typedef std::vector<std::string> AmqpTransactionCapabilityList_t;
        typedef AmqpTransactionCapabilityList_t::const_iterator AmqpTransactionCapabilityListCitr_t;

        typedef std::vector<std::string> AmqpTransactionErrorList_t;
        typedef AmqpTransactionErrorList_t::const_iterator AmqpTransactionErrorListCitr_t;

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQP10_FWDDECLS_HPP_ */
