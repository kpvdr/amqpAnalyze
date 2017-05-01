/*
 * Colors.hpp
 *
 *  Created on: May 1, 2017
 *      Author: kpvdr
 */

#ifndef SRC_AMQPANALYZE_COLOR_HPP_
#define SRC_AMQPANALYZE_COLOR_HPP_

#include <amqpAnalyze/Options.hpp>
#include <map>
#include <vector>

#define FGND_BLK "\033[30m"
#define FGND_RED "\033[31m"
#define FGND_GRN "\033[32m"
#define FGND_YLW "\033[33m"
#define FGND_BLU "\033[34m"
#define FGND_MGT "\033[35m"
#define FGND_CYN "\033[36m"
#define FGND_WHT "\033[37m"
#define FGND_BBLK "\033[1;30m"
#define FGND_BRED "\033[1;31m"
#define FGND_BGRN "\033[1;32m"
#define FGND_BYLW "\033[1;33m"
#define FGND_BBLU "\033[1;34m"
#define FGND_BMGT "\033[1;35m"
#define FGND_BCYN "\033[1;36m"
#define FGND_BWHT "\033[1;37m"
#define BGND_BLK "\033[40m"
#define BGND_RED "\033[41m"
#define BGND_GRN "\033[42m"
#define BGND_YLW "\033[43m"
#define BGND_BLU "\033[44m"
#define BGND_MGT "\033[45m"
#define BGND_CYN "\033[46m"
#define BGND_WHT "\033[47m"
#define RST "\033[m"

/*
#define AC(f, c) (f ? c : "")

#define AC_F_BLK(f) AC(f, FGND_BLK)
#define AC_F_RED(f) AC(f, FGND_RED)
#define AC_F_GRN(f) AC(f, FGND_GRN)
#define AC_F_YLW(f) AC(f, FGND_YLW)
#define AC_F_BLU(f) AC(f, FGND_BLU)
#define AC_F_MGT(f) AC(f, FGND_MGT)
#define AC_F_CYN(f) AC(f, FGND_CYN)
#define AC_F_WHT(f) AC(f, FGND_WHT)
#define AC_F_BBLK(f) AC(f, FGND_BBLK)
#define AC_F_BRED(f) AC(f, FGND_BRED)
#define AC_F_BGRN(f) AC(f, FGND_BGRN)
#define AC_F_BYLW(f) AC(f, FGND_BYLW)
#define AC_F_BBLU(f) AC(f, FGND_BBLU)
#define AC_F_BMGT(f) AC(f, FGND_BMGT)
#define AC_F_BCYN(f) AC(f, FGND_BCYN)
#define AC_F_BWHT(f) AC(f, FGND_BWHT)
#define AC_B_BLK(f) AC(f, BGND_BLK)
#define AC_B_RED(f) AC(f, BGND_RED)
#define AC_B_GRN(f) AC(f, BGND_GRN)
#define AC_B_YLW(f) AC(f, BGND_YLW)
#define AC_B_BLU(f) AC(f, BGND_BLU)
#define AC_B_MGT(f) AC(f, BGND_MGT)
#define AC_B_CYN(f) AC(f, BGND_CYN)
#define AC_B_WHT(f) AC(f, BGND_WHT)
#define AC_RST(f) AC(f, RST)

#define COLOR(c, s, f) (f ? std::string(c) + s + RST : s) // TODO: find a better solution for this
*/

namespace amqpAnalyze {

    enum class DisplayColorType_t {
        RESET,
        DISSECTOR_NAME,
        TCP_PORT,
        TCP_RST_FLAG,
        AMQP_PROTOCOL_HEADER,
        AMQP_FRAME,
        AMQP_PERFORMATIVE,
        AMQP_SECTION,
        AMQP_TYPE_NULL,
        AMQP_TYPE_BOOLEAN,
        AMQP_TYPE_NUMBER,
        AMQP_TYPE_BINARY,
        AMQP_TYPE_CHAR,
        AMQP_TYPE_STRING,
        AMQP_TYPE_SYMBOL,
        AMQP_TYPE_TIMESTAMP,
        AMQP_TYPE_UUID,
        AMQP_RESTRICTED_LIST,
        AMQP_STATE_CONNECTION,
        AMQP_STATE_SESSION,
        AMQP_STATE_LINK,
        MSG_ERROR,
        MSG_WARNING,
        MSG_INFO
    };

    struct ColorDatum {
        std::size_t _offset;
        std::size_t _len;
        DisplayColorType_t _colorType;
        ColorDatum(std::size_t offset, std::size_t len, DisplayColorType_t colorType): _offset(offset), _len(len), _colorType(colorType) {}
    };

    typedef std::vector<const amqpAnalyze::ColorDatum*> ColorList_t;
    typedef ColorList_t::const_iterator ColorList_Citr_t;

    class Color {
    public:
        static std::string color(DisplayColorType_t colorType, const std::string& str);
        static inline const char* getColor(DisplayColorType_t colorType) { return g_optionsPtr->s_colorFlag ? s_displayColor[colorType] : ""; }
        static inline const char* resetColor() { return g_optionsPtr->s_colorFlag ? s_displayColor[DisplayColorType_t::RESET] : ""; }
    protected:
        static std::map<DisplayColorType_t, const char*> s_displayColor;
    };

} /* namespace amqpAnalyze */

#endif /* SRC_AMQPANALYZE_COLOR_HPP_ */
