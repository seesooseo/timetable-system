#ifndef TIME_UTILS_HPP
#define TIME_UTILS_HPP

//TimeUtility.hpp
#pragma once
#include <string>
#include <sstream>
#include <iomanip>

inline std::string minutesToHHMM12(int minutes) {
    int h = minutes / 60, m = minutes % 60;
    bool pm = (h >= 12);
    int disp = h % 12; if (!disp) disp = 12;
    std::ostringstream os;
    os << disp << ':'
        << std::setw(2) << std::setfill('0') << m
        << (pm ? "PM" : "AM");
    return os.str();
}


#endif // TIME_UTILS_HPP
