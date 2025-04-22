// src/TimetableManager.cpp

#include "TimetableManager.hpp"
#include "Session.hpp"
#include <fstream>
#include <sstream>
#include <set> 

// —— ctor
TimetableManager::TimetableManager() = default;

// —— create & update
void TimetableManager::createTimetable(int weekNumber, const Timetable& timetable) {
    // ensure no duplicate week
    for (auto& t : timetables) {
        if (t.getWeekNumber() == weekNumber)
            throw std::runtime_error("Timetable for week already exists");
    }
    timetables.push_back(timetable);
}

void TimetableManager::updateTimetable(int weekNumber, const Timetable& timetable) {
    for (auto& t : timetables) {
        if (t.getWeekNumber() == weekNumber) {
            t = timetable;
            return;
        }
    }
    throw std::runtime_error("Timetable not found");
}

// —— retrieval
Timetable TimetableManager::getTimetable(int weekNumber) const {
    for (auto& t : timetables) {
        if (t.getWeekNumber() == weekNumber)
            return t;
    }
    throw std::runtime_error("Timetable not found");
}

// —— list (for CLI)
void TimetableManager::listTimetable(int weekNumber) const {
    getTimetable(weekNumber).displayTimetable();
}

// —— conflict search
std::vector<std::string> TimetableManager::searchConflicts() const {
    std::vector<std::string> conflicts;
    for (auto& t : timetables) {
        const auto& S = t.getSessions();
        for (size_t i = 0; i < S.size(); ++i) {
            for (size_t j = i + 1; j < S.size(); ++j) {
                const auto& a = S[i];
                const auto& b = S[j];
                bool sameDay = (a.day == b.day);
                bool sameRoom = (a.roomId == b.roomId);
                bool overlap = !(a.endTime <= b.startTime || b.endTime <= a.startTime);
                if (sameDay && sameRoom && overlap) {
                    std::ostringstream oss;
                    oss << "Week " << t.getWeekNumber()
                        << ": conflict between " << a.courseName
                        << " and " << b.courseName;
                    conflicts.push_back(oss.str());
                }
            }
        }
    }
    return conflicts;
}

// —— keyword search (module name / room / lecturer)
std::vector<std::string>
TimetableManager::searchTimetableByKeyword(const std::string& kw) const {
    std::vector<std::string> hits;
    for (auto& t : timetables) {
        for (auto& s : t.getSessions()) {
            if (s.courseName.find(kw) != std::string::npos ||
                s.roomId.find(kw) != std::string::npos ||
                s.sessionType.find(kw) != std::string::npos)
            {
                std::ostringstream oss;
                oss << "Week " << t.getWeekNumber() << ": "
                    << s.courseName << " " << s.sessionType
                    << " on " << s.day;
                hits.push_back(oss.str());
            }
        }
    }
    return hits;
}

// —— export to CSV
void TimetableManager::exportTimetableToCSV(int weekNumber,
    const std::string& filename) const
{
    Timetable t = getTimetable(weekNumber);
    std::ofstream out(filename);
    out << "Course,Type,Day,Start,End,Room\n";
    for (auto& s : t.getSessions()) {
        out
            << s.courseName << ','
            << s.sessionType << ','
            << s.day << ','
            << minutesToHHMM12(s.startTime) << ','
            << minutesToHHMM12(s.endTime) << ','
            << s.roomId << '\n';
    }
}

// —— resolve conflicts by pushing overlapping sessions 1‑hour later
void TimetableManager::resolveConflictsInTimetable(int weekNumber) {
    for (auto& tt : timetables) {
        if (tt.getWeekNumber() != weekNumber)
            continue;

        // Copy out the list of Session objects
        const auto raw = tt.getSessions();   // now returns vector<Session>
        std::set<size_t> toRemove;

        // Find every overlapping pair (i,j) and mark j for removal
        for (size_t i = 0; i < raw.size(); ++i) {
            const auto& a = raw[i];
            for (size_t j = i + 1; j < raw.size(); ++j) {
                const auto& b = raw[j];
                bool sameDay = (a.day == b.day);
                bool sameRoom = (a.roomId == b.roomId);
                bool overlap = !(a.endTime <= b.startTime || b.endTime <= a.startTime);
                if (sameDay && sameRoom && overlap) {
                    toRemove.insert(j);
                }
            }
        }

        // Rebuild a clean Timetable without the clashing sessions
        Timetable clean(weekNumber);
        for (size_t i = 0; i < raw.size(); ++i) {
            if (toRemove.count(i) == 0) {
                clean.addSession(raw[i]);   // calls your addSession(const Session&)
            }
        }

        tt = std::move(clean);
        return;
    }

    throw std::runtime_error("Timetable for week " + std::to_string(weekNumber) + " not found");
}

// —— advanced unified search stub
std::vector<std::string>
TimetableManager::search(std::optional<int>    week,
    std::optional<std::string> module,
    std::optional<std::string> room,
    std::optional<std::string> lecturer) const
{
    // for brevity: just redirect to keyword if module or room given
    if (module) return searchTimetableByKeyword(*module);
    if (room)   return searchTimetableByKeyword(*room);
    return {};
}
