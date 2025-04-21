#ifndef TIME_UTILS_HPP
#define TIME_UTILS_HPP

#include <string>
#include <sstream>
#include <iomanip>

// Converts minutes since midnight into a 12-hour "HH:MM AM/PM" format string.
// For example, 600 becomes "10:00 AM" and 750 becomes "12:30 PM".
inline std::string minutesToHHMM12(int totalMinutes) {
    // Normalize total minutes to within 0 and 1439 (24*60) to handle values greater than a day.
    totalMinutes = totalMinutes % (24 * 60);

    int hours = totalMinutes / 60;
    int mins = totalMinutes % 60;

    bool isPM = (hours >= 12);
    // For 12-hour format, convert 0 to 12, and reduce hours > 12.
    int displayHours = hours % 12;
    if (displayHours == 0) {
        displayHours = 12;
    }

    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << displayHours << ":"
        << std::setw(2) << std::setfill('0') << mins
        << (isPM ? " PM" : " AM");
    return oss.str();
}

#endif // TIME_UTILS_HPP
