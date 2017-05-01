/*
 * Color.cpp
 *
 *  Created on: May 1, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/Color.hpp>

#include <sstream>

namespace amqpAnalyze {

    // static
    std::map<DisplayColorType_t, const char*> Color::s_displayColor = {
                    {DisplayColorType_t::RESET, RST},
                    {DisplayColorType_t::DISSECTOR_NAME, FGND_GRN},
                    {DisplayColorType_t::TCP_PORT, FGND_BLU},
                    {DisplayColorType_t::TCP_RST_FLAG, FGND_BRED},
                    {DisplayColorType_t::AMQP_PROTOCOL_HEADER, FGND_BGRN},
                    {DisplayColorType_t::AMQP_FRAME, FGND_BCYN},
                    {DisplayColorType_t::AMQP_PERFORMATIVE, FGND_BYLW},
                    {DisplayColorType_t::AMQP_SECTION, FGND_YLW},
                    {DisplayColorType_t::AMQP_TYPE_NULL, FGND_BWHT},
                    {DisplayColorType_t::AMQP_TYPE_BOOLEAN, FGND_MGT},
                    {DisplayColorType_t::AMQP_TYPE_NUMBER, FGND_RED},
                    {DisplayColorType_t::AMQP_TYPE_BINARY, FGND_BWHT},
                    {DisplayColorType_t::AMQP_TYPE_CHAR, FGND_BBLU},
                    {DisplayColorType_t::AMQP_TYPE_STRING, FGND_BBLU},
                    {DisplayColorType_t::AMQP_TYPE_SYMBOL, FGND_BMGT},
                    {DisplayColorType_t::AMQP_TYPE_TIMESTAMP, FGND_BWHT},
                    {DisplayColorType_t::AMQP_TYPE_UUID, FGND_BMGT},
                    {DisplayColorType_t::AMQP_RESTRICTED_LIST, FGND_BWHT},
                    {DisplayColorType_t::AMQP_STATE_CONNECTION, FGND_YLW},
                    {DisplayColorType_t::AMQP_STATE_SESSION, FGND_YLW},
                    {DisplayColorType_t::AMQP_STATE_LINK, FGND_YLW},
                    {DisplayColorType_t::MSG_ERROR, FGND_BRED},
                    {DisplayColorType_t::MSG_WARNING, FGND_BYLW},
                    {DisplayColorType_t::MSG_INFO, FGND_GRN}
    };


    //static
    std::string Color::color(DisplayColorType_t colorType, const std::string& str) {
        std::ostringstream oss;
        oss << getColor(colorType) << str << resetColor();
        return oss.str();
    }

} /* namespace amqpAnalyze */
