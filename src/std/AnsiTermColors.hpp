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

    inline ostream& b_blue(ostream& os) {
        os << "\033[1;34m";
        return os;
    }

    inline ostream& b_cyan(ostream& os) {
        os << "\033[1;36m";
        return os;
    }

    inline ostream& b_green(ostream& os) {
        os << "\033[1;32m";
        return os;
    }

    inline ostream& b_magenta(ostream& os) {
        os << "\033[1;35m";
        return os;
    }

    inline ostream& b_red(ostream& os) {
        os << "\033[1;31m";
        return os;
    }

    inline ostream& b_white(ostream& os) {
        os << "\033[1;37m";
        return os;
    }

    inline ostream& b_yellow(ostream& os) {
        os << "\033[1;33m";
        return os;
    }

    inline ostream& blue(ostream& os) {
        os << "\033[34m";
        return os;
    }

    inline ostream& cyan(ostream& os) {
        os << "\033[36m";
        return os;
    }

    inline ostream& green(ostream& os) {
        os << "\033[32m";
        return os;
    }

    inline ostream& magenta(ostream& os) {
        os << "\033[35m";
        return os;
    }

    inline ostream& red(ostream& os) {
        os << "\033[31m";
        return os;
    }

    inline ostream& white(ostream& os) {
        os << "\033[37m";
        return os;
    }

    inline ostream& yellow(ostream& os) {
        os << "\033[33m";
        return os;
    }

    inline ostream& res(ostream& os) {
        os << "\033[m";
        return os;
    }

} /* namespace std */


#endif /* SRC_STD_ANSITERMCOLORS_HPP_ */
