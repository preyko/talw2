#pragma once

#include <sstream>
inline std::string intToStr(int num) {
    std::stringstream c;
    c << num;
    return c.str();
}
