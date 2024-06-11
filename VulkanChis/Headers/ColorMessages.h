//
// Created by loochis on 2/3/24.
//

#ifndef VOXCHIS_COLORMESSAGES_H
#define VOXCHIS_COLORMESSAGES_H

#include <string>
#include <iostream>

/*
            FG  BG
    black   30  40
    red     31  41
    green   32  42
    yellow  33  43
    blue    34  44
    magenta 35  45
    cyan    36  46
    white   37  47
 */

enum message_color {
    BLACK = 30,
    RED = 31,
    GREEN = 32,
    YELLOW = 33,
    BLUE = 34,
    MAGENTA = 35,
    CYAN = 36,
    WHITE = 37
};

static inline void print_colored(const std::string& message, message_color color) {
    std::cout << "\033[" << std::to_string(color) << "m" << message << "\033[0m\n";
}


#endif //VOXCHIS_COLORMESSAGES_H
