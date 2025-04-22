#ifndef SESSION_HPP
#define SESSION_HPP

#include <string>
#include <sstream>
#include <json.hpp>                  // ← pull in nlohmann::json
using json = nlohmann::json;        // ← alias

// A structure to represent a single session in a timetable.
struct Session {
    std::string courseName;
    std::string sessionType;
    std::string day;
    int startTime; // minutes since midnight.
    int endTime;
    std::string roomId;
    std::string lecturerId;
    std::string groupId;
};

// Helper: parse a space‑delimited string into a Session.
inline Session parseSession(const std::string& sessionStr) {
    std::istringstream iss(sessionStr);
    Session s;
    iss >> s.courseName
        >> s.sessionType
        >> s.day
        >> s.startTime
        >> s.endTime
        >> s.roomId
        >> s.lecturerId
        >> s.groupId;
    return s;
}

// JSON (de)serialization overloads for nlohmann::json:
inline void to_json(json& j, Session const& s) {
    j = {
        {"courseName",  s.courseName},
        {"sessionType", s.sessionType},
        {"day",         s.day},
        {"startTime",   s.startTime},
        {"endTime",     s.endTime},
        {"roomId",      s.roomId},
        {"lecturerId",  s.lecturerId},
        {"groupId",     s.groupId}
    };
}

inline void from_json(json const& j, Session& s) {
    j.at("courseName").get_to(s.courseName);
    j.at("sessionType").get_to(s.sessionType);
    j.at("day").get_to(s.day);
    j.at("startTime").get_to(s.startTime);
    j.at("endTime").get_to(s.endTime);
    j.at("roomId").get_to(s.roomId);
    j.at("lecturerId").get_to(s.lecturerId);
    j.at("groupId").get_to(s.groupId);
}

#endif // SESSION_HPP
