/*
 * FramePlaceholder.hpp
 *
 *  Created on: Apr 19, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_AMQP10_FRAMEPLACEHOLDER_HPP_
#define SRC_AMQPANALYZE_AMQP10_FRAMEPLACEHOLDER_HPP_

#include <amqpAnalyze/amqp10/AmqpBlock.hpp>

namespace amqpAnalyze
{
    class Error;

    namespace amqp10
    {

        class FramePlaceholder: public AmqpBlock
        {
        public:
            FramePlaceholder(uint64_t packetNum, std::size_t dataOffset, const amqpAnalyze::Error& error);
            virtual ~FramePlaceholder();

            std::ostringstream& appendString(std::ostringstream& oss, std::size_t margin, bool ignoreFirstMargin) const override;
            inline AmqpBlockType_t blockType() const override { return AmqpBlockType_t::FRAME_ERROR; }
            void validate() override;

        protected:
            std::string _errMsg;
        };

    } /* namespace amqp10 */
} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_AMQP10_FRAMEPLACEHOLDER_HPP_ */
