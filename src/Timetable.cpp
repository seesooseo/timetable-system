#include "Timetable.hpp"
#include <sstream>
#include <stdexcept>

Timetable::Timetable() : weekNumber(0) {}

Timetable::Timetable(int week) : weekNumber(week) {}

int Timetable::getWeekNumber() const {
    return weekNumber;
}

void Timetable::addSession(const std::string& sessionDetail) {
    // 1) parse into a Session
    Session s = parseSession(sessionDetail);

    // 2) validate time range
    validateTimes(s.startTime, s.endTime);

    // 3) check for clashes
    for (const auto& other : sessions) {
        bool sameDay = (other.day == s.day);
        bool sameRoom = (other.roomId == s.roomId);
        bool overlap = !(s.endTime <= other.startTime || other.endTime <= s.startTime);
        if (sameDay && sameRoom && overlap) {
            std::cerr << "⚠ warning: room clash detected\n";
            // but continue—don't throw
        }
    }

    // 4) store it
    sessions.push_back(s);
}

void Timetable::displayTimetable() const {
    std::cout << "Timetable for week " << weekNumber << ":\n";
    if (sessions.empty()) {
        std::cout << "  (no sessions scheduled)\n";
    }
    else {
        for (const auto& s : sessions) {
            std::string start = minutesToHHMM12(s.startTime);
            std::string end = minutesToHHMM12(s.endTime);
            std::cout
                << "  " << s.courseName << " " << s.sessionType
                << " on " << s.day
                << " from " << start
                << " to " << end
                << " in room " << s.roomId
                << "\n";
        }
    }
}
