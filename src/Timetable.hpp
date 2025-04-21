#ifndef TIMETABLE_HPP
#define TIMETABLE_HPP

#include <vector>
#include <string>
#include <iostream>

class Timetable {
private:
    int weekNumber;
    std::vector<std::string> sessions; // Each session as a formatted string.
public:
    Timetable();
    Timetable(int week);
    int getWeekNumber() const;
    void addSession(const std::string& sessionDetail);
    void displayTimetable() const;
    const std::vector<std::string>& getSessions() const; // <-- New getter
};

#endif // TIMETABLE_HPP
