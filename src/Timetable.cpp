// src/Timetable.cpp
#include "Timetable.hpp"
#include "TimeValidation.hpp"   // getValidTimeInput(...)
#include "TimeUtility.hpp"      // minutesToHHMM12(int)
#include <iostream>

// default ctor
Timetable::Timetable() : weekNumber(0) {}

// ctor with week
Timetable::Timetable(int week) : weekNumber(week) {}

// getter
int Timetable::getWeekNumber() const {
    return weekNumber;
}

// parse a space‑delimited string
void Timetable::addSession(const std::string& sessionDetail) {
    Session s = parseSession(sessionDetail);
    addSession(s);
}

// core overload: keep prompting until no clash
void Timetable::addSession(const Session& sessionTemplate) {
    Session s = sessionTemplate;
    while (true) {
        // 1) prompt for times (you already have getValidTimeInput)
        getValidTimeInput(s.startTime, s.endTime);

        // 2) check clash
        bool clash = false;
        for (auto const& other : sessions) {
            bool sameDay = (other.day == s.day);
            bool sameRoom = (other.roomId == s.roomId);
            bool overlap = !(s.endTime <= other.startTime ||
                other.endTime <= s.startTime);
            if (sameDay && sameRoom && overlap) {
                std::cout << "Warning: room clash detected, please re‑enter times.\n";
                clash = true;
                break;
            }
        }

        // 3) if no clash, store and break; else loop
        if (!clash) {
            sessions.push_back(s);
            return;
        }
    }
}

// bypass all validation
void Timetable::forceAddSession(const Session& s) {
    sessions.push_back(s);
}

const std::vector<Session>& Timetable::getSessions() const {
    return sessions;
}

void Timetable::displayTimetable() const {
    std::cout << "Timetable for week " << weekNumber << ":\n";
    for (auto const& s : sessions) {
        std::cout
            << s.courseName << " "
            << s.sessionType << " "
            << s.day << " "
            << minutesToHHMM12(s.startTime)
            << " - "
            << minutesToHHMM12(s.endTime)
            << "  Room: " << s.roomId
            << "  Lecturer: " << s.lecturerId
            << "  Group: " << s.groupId
            << "\n";
    }
}

void Timetable::displayForGroup(const std::string& groupId) const {
    std::cout << "Timetable for group " << groupId << ":\n";
    for (auto const& s : sessions) {
        if (s.groupId == groupId) {
            std::cout
                << s.courseName << " "
                << s.sessionType << " "
                << s.day << " "
                << minutesToHHMM12(s.startTime)
                << " - "
                << minutesToHHMM12(s.endTime)
                << "  Room: " << s.roomId
                << "  Lecturer: " << s.lecturerId
                << "\n";
        }
    }
}
