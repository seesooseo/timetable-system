#ifndef TIMETABLEMANAGER_HPP
#define TIMETABLEMANAGER_HPP

#include "Timetable.hpp"
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <optional>

class TimetableManager {
private:
    std::vector<Timetable> timetables;
public:
    TimetableManager();

    // Function to create a new timetable for a given week
    void createTimetable(int weekNumber, const Timetable& timetable);

    // Function to update an existing timetable
    void updateTimetable(int weekNumber, const Timetable& timetable);

    // Retrieve timetable for a given week; throws error if not found.
    Timetable getTimetable(int weekNumber) const;

    // Display timetable for a given week
    void listTimetable(int weekNumber) const;

    // Advanced function: search for scheduling conflicts
    // (For simplicity, returns a vector of strings describing conflicts.)
    std::vector<std::string> searchConflicts() const;

    // Advanced: search through all timetables by a keyword.
    std::vector<std::string> searchTimetableByKeyword(const std::string& keyword) const;

    // Advanced: export a timetable to a CSV file.
    void exportTimetableToCSV(int weekNumber, const std::string& filename) const;

    // Resolves conflicts in a timetable for the specified week by moving the later session.
    void resolveConflictsInTimetable(int weekNumber);

    std::vector<std::string> search(std::optional<int> week,
        std::optional<std::string> module,
        std::optional<std::string> room,
        std::optional<std::string> lecturer) const;

    const std::vector<Timetable>& getTimetables() const { return timetables; }

    bool isRoomAvailable(int weekNumber,
        const std::string & day,
        const std::string & roomId,
        int startTime,
        int endTime) const;
};



#endif // TIMETABLEMANAGER_HPP