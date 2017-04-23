/*
 * TermColors.hpp
 *
 *  Created on: Apr 17, 2017
 *      Author: kpvdr
 */

#include <amqpAnalyze/Error.hpp>

#ifndef SRC_STD_ANSITERMCOLORS_HPP_
#define SRC_STD_ANSITERMCOLORS_HPP_

#include <iostream>

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

namespace std
{

/*
    inline ostream& bgnd_black(ostream& os) {
        os << "\033[40m";
        return os;
    }

    inline ostream& bgnd_blue(ostream& os) {
        os << "\033[44m";
        return os;
    }

    inline ostream& bgnd_cyan(ostream& os) {
        os << "\033[46m";
        return os;
    }

    inline ostream& bgnd_green(ostream& os) {
        os << "\033[42m";
        return os;
    }

    inline ostream& bgnd_magenta(ostream& os) {
        os << "\033[45m";
        return os;
    }

    inline ostream& bgnd_red(ostream& os) {
        os << "\033[41m";
        return os;
    }

    inline ostream& bgnd_white(ostream& os) {
        os << "\033[47m";
        return os;
    }

    inline ostream& bgnd_yellow(ostream& os) {
        os << "\033[43m";
        return os;
    }

    inline ostream& fgnd_b_black(ostream& os) {
        os << "\033[1;30m";
        return os;
    }

    inline ostream& fgnd_b_blue(ostream& os) {
        os << "\033[1;34m";
        return os;
    }

    inline ostream& fgnd_b_cyan(ostream& os) {
        os << "\033[1;36m";
        return os;
    }

    inline ostream& fgnd_b_green(ostream& os) {
        os << "\033[1;32m";
        return os;
    }

    inline ostream& fgnd_b_magenta(ostream& os) {
        os << "\033[1;35m";
        return os;
    }

    inline ostream& fgnd_b_red(ostream& os) {
        os << "\033[1;31m";
        return os;
    }

    inline ostream& fgnd_b_white(ostream& os) {
        os << "\033[1;37m";
        return os;
    }

    inline ostream& fgnd_b_yellow(ostream& os) {
        os << "\033[1;33m";
        return os;
    }

    inline ostream& fgnd_black(ostream& os) {
        os << "\033[30m";
        return os;
    }

    inline ostream& fgnd_blue(ostream& os) {
        os << "\033[34m";
        return os;
    }

    inline ostream& fgnd_cyan(ostream& os) {
        os << "\033[36m";
        return os;
    }

    inline ostream& fgnd_green(ostream& os) {
        os << "\033[32m";
        return os;
    }

    inline ostream& fgnd_magenta(ostream& os) {
        os << "\033[35m";
        return os;
    }

    inline ostream& fgnd_red(ostream& os) {
        os << "\033[31m";
        return os;
    }

    inline ostream& fgnd_white(ostream& os) {
        os << "\033[37m";
        return os;
    }

    inline ostream& fgnd_yellow(ostream& os) {
        os << "\033[33m";
        return os;
    }

    inline ostream& res(ostream& os) {
        os << "\033[m";
        return os;
    }
*/

} /* namespace std */


#endif /* SRC_STD_ANSITERMCOLORS_HPP_ */
