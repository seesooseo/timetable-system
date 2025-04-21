#ifndef SESSIONTYPEMANAGER_HPP
#define SESSIONTYPEMANAGER_HPP

#include "SessionType.hpp"
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <iostream>

class SessionTypeManager {
private:
    std::vector<SessionType> sessionTypes;
public:
    SessionTypeManager();

    // CRUD functions for SessionType
    void addSessionType(const SessionType& sessionType);
    void updateSessionType(const std::string& typeId, const SessionType& updatedSessionType);
    void deleteSessionType(const std::string& typeId);
    void listSessionTypes() const;
};

#endif // SESSIONTYPEMANAGER_HPP
