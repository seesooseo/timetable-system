#ifndef TIMEVALIDATION_HPP
#define TIMEVALIDATION_HPP
#include <stdexcept>

inline void validateTimes(int start, int end) {
    if (start < 0 || end > 24 * 60 || start >= end)
        throw std::runtime_error("Invalid time range (must be 0‑1439 & start<end)");
}
#endif
