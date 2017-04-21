/*
 * TermColors.hpp
 *
 *  Created on: Apr 17, 2017
 *      Author: kpvdr
 */

#ifndef SRC_STD_ANSITERMCOLORS_HPP_
#define SRC_STD_ANSITERMCOLORS_HPP_

#include <iostream>


namespace std
{

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

} /* namespace std */


#endif /* SRC_STD_ANSITERMCOLORS_HPP_ */
