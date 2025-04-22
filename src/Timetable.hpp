#ifndef TIMETABLE_HPP
#define TIMETABLE_HPP

#include <vector>
#include <string>
#include "Session.hpp"

// forward‑declare the helper you use in addSession:
void getValidTimeInput(int& start, int& end);
std::string minutesToHHMM12(int m);

// Your Timetable class
class Timetable {
private:
    int weekNumber;
    std::vector<Session> sessions;

public:
    Timetable();
    explicit Timetable(int week);

    int getWeekNumber() const;

    // parse a space-delimited string, then prompt & clash-check
    void addSession(const std::string& sessionDetail);

    // same but directly from a Session
    void addSession(const Session& s);

    // if you ever need to bypass clash-check
    void forceAddSession(const Session& s);

    const std::vector<Session>& getSessions() const;

    // display everything
    void displayTimetable() const;

    // display only one group's sessions
    void displayForGroup(const std::string& groupId) const;
};

#endif // TIMETABLE_HPP
