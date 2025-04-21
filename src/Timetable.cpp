#include "Timetable.hpp"
#include "TimeUtility.hpp"
#include "Session.hpp"
#include <iostream>
#include <sstream>
#include "TimeValidation.hpp"    // for validateTimes()
#include "Session.hpp"           // for parseSession()
#include <stdexcept>
#include <algorithm>

Timetable::Timetable() : weekNumber(0) {}

Timetable::Timetable(int week) : weekNumber(week) {}

int Timetable::getWeekNumber() const {
    return weekNumber;
}

void Timetable::addSession(const std::string& sessionStr) {
    // 1) Parse into fields
    Session s = parseSession(sessionStr);

    // 2) Validate that start < end and within 0–1439
    validateTimes(s.startTime, s.endTime);

    // 3) Check existing sessions for a room/day clash
    for (const auto& existing : sessions) {
        Session other = parseSession(existing);
        bool sameDay = (other.day == s.day);
        bool sameRoom = (other.roomId == s.roomId);
        bool overlap = !(s.endTime <= other.startTime || other.endTime <= s.startTime);
        if (sameDay && sameRoom && overlap) {
            std::cerr << "⚠ warning: room clash detected\n";
            // DO NOT throw – just record it
        }
    }

    // 4) If all good, store it
    sessions.push_back(sessionStr);
}

void Timetable::displayTimetable() const {
    std::cout << "Timetable for week " << weekNumber << ":" << std::endl;
    if (sessions.empty()) {
        std::cout << "No sessions scheduled." << std::endl;
    }
    else {
        for (const auto& sessionStr : sessions) {
            // Use the parseSession helper to extract structured data.
            Session s = parseSession(sessionStr);
            // Reformat start and end times using the utility function.
            std::string startFormatted = minutesToHHMM12(s.startTime);
            std::string endFormatted = minutesToHHMM12(s.endTime);
            std::cout << s.courseName << " " << s.sessionType
                << " on " << s.day
                << " from " << startFormatted
                << " to " << endFormatted
                << " in room " << s.roomId << std::endl;
        }
    }
}

const std::vector<std::string>& Timetable::getSessions() const {
    return sessions;
}
