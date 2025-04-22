#ifndef TIMETABLE_HPP
#define TIMETABLE_HPP

#include <vector>
#include <iostream>
#include "Session.hpp"
#include "TimeUtility.hpp"
#include "TimeValidation.hpp"

class Timetable {
private:
    int weekNumber;
    std::vector<Session> sessions;  // now store Session objects directly

public:
    Timetable();
    explicit Timetable(int week);

    int getWeekNumber() const;

    // parse a single line, validate, detect clashes, then store
    void addSession(const std::string& sessionDetail);

    // overload: directly append a Session
    void addSession(const Session& s) {
        validateTimes(s.startTime, s.endTime);
        // clash‐detection
        for (auto& other : sessions) {
            bool sameDay = (other.day == s.day);
            bool sameRoom = (other.roomId == s.roomId);
            bool overlap = !(s.endTime <= other.startTime || other.endTime <= s.startTime);
            if (sameDay && sameRoom && overlap) {
                std::cerr << "⚠ warning: room clash detected\n";
            }
        }
        sessions.push_back(s);
    }

    const std::vector<Session>& getSessions() const {
        return sessions;
    }

    void displayTimetable() const;
};

#endif // TIMETABLE_HPP
