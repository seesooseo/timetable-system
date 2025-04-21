#ifndef SESSION_HPP
#define SESSION_HPP

#include <string>
#include <sstream>

// A structure to represent a single session in a timetable.
struct Session {
    std::string courseName;
    std::string sessionType;
    std::string day;
    int startTime; // Represent time as minutes since midnight.
    int endTime;
    std::string roomId;
    std::string lecturerId;
    std::string groupId;
};

// Helper function: Parses a formatted session string into a Session struct.
// Expected format: "COMP101 Lecture Monday 600 660 R001"
inline Session parseSession(const std::string& sessionStr) {
    std::istringstream iss(sessionStr);
    Session s;
    iss >> s.courseName >> s.sessionType >> s.day >> s.startTime >> s.endTime >> s.roomId >> s.lecturerId >> s.groupId;
    return s;
}

#endif // SESSION_HPP
