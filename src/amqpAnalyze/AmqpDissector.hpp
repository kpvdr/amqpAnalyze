/*
 * AmqpDissector.hpp
 *
 *  Created on: Apr 2, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_AMQPDISSECTOR_HPP_
#define SRC_AMQPANALYZE_AMQPDISSECTOR_HPP_

#include <amqpAnalyze/amqp10/AmqpBlock.hpp>
#include <amqpAnalyze/Dissector.hpp>

struct pcap_pkthdr;

namespace amqpAnalyze
{
    class TcpDissector;

    enum class AmqpVersions_t {
        NONE = 0,
        UNKNOWN,
        AMQP_0_8,
        AMQP_0_9,
        AMQP_0_9_1,
        AMQP_0_10,
        AMQP_1_0
    };
    class AmqpDissector: public Dissector {
    public:
        AmqpDissector(Packet* packetPtr, uint32_t dataOffs, Dissector* parent, std::size_t amqpDataSize);
        virtual ~AmqpDissector();

        void appendString(std::ostringstream& oss, size_t margin) const override;
        inline DissectorType_t dissectorType() const override { return DissectorType_t::DISSECTOR_AMQP; }
        static const char* amqpVersionStr(AmqpVersions_t amqpVersion);

    protected:
        std::size_t _amqpDataSize;
        TcpDissector* _tcpDissectorPtr;
        std::string _debugHexFrameData;
        amqp10::AmqpBlockList_t _amqpBlockList;
        static std::map<AmqpVersions_t, const char*> s_amqpVerionStr;

        void handleAmqp0_8();
        void handleAmqp0_9();
        void handleAmqp0_9_1();
        void handleAmqp0_10();
        void handleAmqp1_0();
        AmqpVersions_t protocolHeaderVersion();
    };

} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQPDISSECTOR_HPP_ */
