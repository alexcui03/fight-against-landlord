/**
 * ui.h
 */

#ifndef _UI_H_
#define _UI_H_

#include <iostream>
#include <cctype>
#include <limits>

namespace fal {

template <class T>
bool input_with_check(T &res) {
    std::cin >> res;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }
    return true;
}

template <class T>
bool input_until_endl(T &res) {
    char ch;
    while (std::isspace(ch = std::cin.peek())) {
        std::cin.ignore();
        if (ch == '\n') return false;
    }
    input_with_check(res);
    return true;
}

inline void clear_until_endl() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

}

#endif
