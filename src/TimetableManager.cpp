#include "TimetableManager.hpp"
#include "Session.hpp"
#include "TimeUtility.hpp"
#include <sstream>
#include <fstream>
#include <optional>
#include <TimeValidation.hpp>

TimetableManager::TimetableManager() : timetables() {}

void TimetableManager::createTimetable(int weekNumber, const Timetable& timetable) {
    auto it = std::find_if(timetables.begin(), timetables.end(),
        [weekNumber](const Timetable& t) { return t.getWeekNumber() == weekNumber; });
    if (it != timetables.end()) {
        throw std::runtime_error("Timetable for week " + std::to_string(weekNumber) + " already exists.");
    }
    timetables.push_back(timetable);
}

void TimetableManager::updateTimetable(int weekNumber, const Timetable& timetable) {
    auto it = std::find_if(timetables.begin(), timetables.end(),
        [weekNumber](const Timetable& t) { return t.getWeekNumber() == weekNumber; });
    if (it != timetables.end()) {
        *it = timetable;
    }
    else {
        throw std::runtime_error("Timetable for week " + std::to_string(weekNumber) + " not found.");
    }
}

Timetable TimetableManager::getTimetable(int weekNumber) const {
    auto it = std::find_if(timetables.begin(), timetables.end(),
        [weekNumber](const Timetable& t) { return t.getWeekNumber() == weekNumber; });
    if (it != timetables.end()) {
        return *it;
    }
    else {
        throw std::runtime_error("Timetable for week " + std::to_string(weekNumber) + " not found.");
    }
}

void TimetableManager::listTimetable(int weekNumber) const {
    try {
        Timetable t = getTimetable(weekNumber);
        t.displayTimetable();
    }
    catch (const std::runtime_error& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}

std::vector<std::string> TimetableManager::searchConflicts() const {
    std::vector<std::string> conflicts;

    // For each timetable in the collection.
    for (const auto& timetable : timetables) {
        // Retrieve the sessions in this timetable.
        const std::vector<std::string>& sessList = timetable.getSessions();

        // Compare every pair of sessions.
        for (size_t i = 0; i < sessList.size(); ++i) {
            Session s1 = parseSession(sessList[i]);
            validateTimes(s1.startTime, s1.endTime);
            for (size_t j = i + 1; j < sessList.size(); ++j) {
                Session s2 = parseSession(sessList[j]);
                validateTimes(s2.startTime, s2.endTime);
                // If sessions are on the same day and in the same room...
                if (s1.day == s2.day && s1.roomId == s2.roomId) {
                    // ...and their time intervals overlap:
                    if (s1.startTime < s2.endTime && s2.startTime < s1.endTime) {
                        std::ostringstream oss;
                        oss << "Conflict between " << s1.courseName << " and " << s2.courseName
                            << " in room " << s1.roomId << " on " << s1.day
                            << ". Times: [" << minutesToHHMM12(s1.startTime) << ", " << minutesToHHMM12(s1.endTime)
                            << "] vs [" << minutesToHHMM12(s2.startTime) << ", " << minutesToHHMM12(s2.endTime) << "].";
                        conflicts.push_back(oss.str());
                    }
                }
            }
        }
    }

    return conflicts;
}

std::vector<std::string> TimetableManager::searchTimetableByKeyword(const std::string& keyword) const {
    std::vector<std::string> results;
    // Loop through every timetable
    for (const auto &timetable : timetables) {
        int week = timetable.getWeekNumber();
        const auto &sessions = timetable.getSessions();
        for (const auto &sessionStr : sessions) {
            // A simple search: if the session string contains the keyword.
            if (sessionStr.find(keyword) != std::string::npos) {
                results.push_back("Week " + std::to_string(week) + ": " + sessionStr);
            }
        }
    }
    return results;
}

// E3. Export timetable of a specified week to CSV.
void TimetableManager::exportTimetableToCSV(int weekNumber, const std::string& filename) const {
    // Create a local copy of the filename so we can modify it if needed.
    std::string actualFilename = filename;
    // Check if the filename ends with ".csv"
    if (actualFilename.size() < 4 || actualFilename.substr(actualFilename.size() - 4) != ".csv") {
        actualFilename += ".csv";
    }

    std::ofstream outFile;
    outFile.open(actualFilename.c_str());
    if (!outFile.is_open()) {
        throw std::runtime_error("Could not open file " + actualFilename + " for writing.");
    }

    // (Optional) Print the filename for confirmation.
    std::cout << "Exporting CSV to: " << actualFilename << std::endl;

    // Write CSV header.
    outFile << "Week,Session" << std::endl;

    try {
        Timetable t = getTimetable(weekNumber);
        for (const auto& session : t.getSessions()) {
            outFile << weekNumber << "," << session << std::endl;
        }
    }
    catch (const std::exception& e) {
        outFile.close();
        throw;
    }
    outFile.close();
}


// Helper: formats a Session struct back into the standard session string format.
static std::string formatSession(const Session& s) {
    std::ostringstream oss;
    oss << s.courseName << " " << s.sessionType << " " << s.day << " "
        << s.startTime << " " << s.endTime << " " << s.roomId << " " << s.lecturerId << " " << s.groupId;
    return oss.str();
}

void TimetableManager::resolveConflictsInTimetable(int weekNumber) {
    // Retrieve the timetable for the week.
    Timetable t = getTimetable(weekNumber);
    // Access the session list. Since getSessions() returns a const reference,
    // we use const_cast for simple in-place modification. (Ideally, design for modifiability.)
    std::vector<std::string>& sessList = const_cast<std::vector<std::string>&>(t.getSessions());
    bool conflictResolved = false;

    // Iterate over each pair of sessions.
    for (size_t i = 0; i < sessList.size(); ++i) {
        Session s1 = parseSession(sessList[i]);
        validateTimes(s1.startTime, s1.endTime);
        for (size_t j = i + 1; j < sessList.size(); ++j) {
            Session s2 = parseSession(sessList[j]);
            validateTimes(s2.startTime, s2.endTime);
            // Check if sessions occur on the same day in the same room.
            if (s1.day == s2.day && s1.roomId == s2.roomId) {
                // Check for overlapping times:
                if (s1.startTime < s2.endTime && s2.startTime < s1.endTime) {
                    // Assume s2 is the later session (if its startTime is greater) and adjust it.
                    if (s2.startTime >= s1.startTime) {
                        int duration = s2.endTime - s2.startTime;
                        // Move s2 so that its start time becomes s1.endTime.
                        s2.startTime = s1.endTime;
                        s2.endTime = s2.startTime + duration;
                        // Update session string.
                        sessList[j] = formatSession(s2);
                        conflictResolved = true;
                        std::cout << "Resolved conflict: "
                            << s2.courseName << " moved to start at "
                            << minutesToHHMM12(s2.startTime)
                            << " (ends at " << minutesToHHMM12(s2.endTime)
                            << ")." << std::endl;
                    }
                }
            }
        }
    }

    if (!conflictResolved) {
        std::cout << "No conflicts required resolution." << std::endl;
    }
    else {
        // Update the internal timetable.
        // Remove any existing timetable with the same weekNumber.
        auto it = std::remove_if(timetables.begin(), timetables.end(),
            [weekNumber](const Timetable& tt) { return tt.getWeekNumber() == weekNumber; });
        if (it != timetables.end()) {
            timetables.erase(it, timetables.end());
        }
        // Add the updated timetable.
        timetables.push_back(t);
    }
}
std::vector<std::string> TimetableManager::search(
    std::optional<int> week,
    std::optional<std::string> module,
    std::optional<std::string> room,
    std::optional<std::string> lecturer) const {

    std::vector<std::string> out;
    for (const auto& t : timetables) {
        if (week && t.getWeekNumber() != *week) continue;
        for (const auto& line : t.getSessions()) {
            Session s = parseSession(line);
            validateTimes(s.startTime, s.endTime);
            if (module && s.courseName != *module)   continue;
            if (room && s.roomId != *room)     continue;
            if (lecturer && s.lecturerId != *lecturer) continue;
            out.push_back("Week " + std::to_string(t.getWeekNumber()) + ": " + line);
        }
    }
    return out;
}
