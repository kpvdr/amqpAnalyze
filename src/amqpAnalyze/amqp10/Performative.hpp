/*
 * Performative.hpp
 *
 *  Created on: Apr 6, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_AMQP10_PERFORMATIVE_HPP_
#define SRC_AMQPANALYZE_AMQP10_PERFORMATIVE_HPP_

#include <amqpAnalyze/amqp10/Buffer.hpp>

#include <cstdint>

namespace amqpAnalyze
{
    namespace amqp10
    {

        enum performatve_e:uint64_t {OPEN=0x10,
                                     BEGIN,
                                     ATTACH,
                                     FLOW,
                                     TRANSFER,
                                     DISPOSITION,
                                     DETACH,
                                     END,
                                     CLOSE};

        class Performative
        {
        protected:
            AmqpList* _fieldList;
        public:
            Performative(AmqpList* fieldList);
            virtual ~Performative();
            virtual performatve_e descriptor() const = 0;
            const char* name() const;
            static Performative* decode(const char* dataPtr, std::size_t dataLength);
            static std::map<performatve_e, const char*> s_performativeName;
        };


        class Open: public Performative {
        public:
            Open(AmqpList* fieldList);
            virtual ~Open();
            inline performatve_e descriptor() const { return OPEN; }
        };


        class Begin: public Performative {
        public:
            Begin(AmqpList* fieldList);
            virtual ~Begin();
            inline performatve_e descriptor() const { return BEGIN; }
        };


        class Attach: public Performative {
        public:
            Attach(AmqpList* fieldList);
            virtual ~Attach();
            inline performatve_e descriptor() const { return ATTACH; }
        };


        class Flow: public Performative {
        public:
            Flow(AmqpList* fieldList);
            virtual ~Flow();
            inline performatve_e descriptor() const { return FLOW; }
        };


        class Transfer: public Performative {
        public:
            Transfer(AmqpList* fieldList);
            virtual ~Transfer();
            inline performatve_e descriptor() const { return TRANSFER; }
        };


        class Disposition: public Performative {
        public:
            Disposition(AmqpList* fieldList);
            virtual ~Disposition();
            inline performatve_e descriptor() const { return DISPOSITION; }
        };


        class Detach: public Performative {
        public:
            Detach(AmqpList* fieldList);
            virtual ~Detach();
            inline performatve_e descriptor() const { return DETACH; }
        };


        class End: public Performative {
        public:
            End(AmqpList* fieldList);
            virtual ~End();
            inline performatve_e descriptor() const { return END; }
        };


        class Close: public Performative {
        public:
            Close(AmqpList* fieldList);
            virtual ~Close();
            inline performatve_e descriptor() const { return CLOSE; }
        };

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQP10_PERFORMATIVE_HPP_ */
