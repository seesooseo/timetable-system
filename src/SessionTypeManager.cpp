#include "SessionTypeManager.hpp"

SessionTypeManager::SessionTypeManager() : sessionTypes() {}

void SessionTypeManager::addSessionType(const SessionType& sessionType) {
    auto it = std::find_if(sessionTypes.begin(), sessionTypes.end(),
        [&sessionType](const SessionType& st) { return st.getTypeId() == sessionType.getTypeId(); });
    if (it != sessionTypes.end()) {
        throw std::runtime_error("Session type with ID " + sessionType.getTypeId() + " already exists.");
    }
    sessionTypes.push_back(sessionType);
}

void SessionTypeManager::updateSessionType(const std::string& typeId, const SessionType& updatedSessionType) {
    auto it = std::find_if(sessionTypes.begin(), sessionTypes.end(),
        [&typeId](const SessionType& st) { return st.getTypeId() == typeId; });
    if (it != sessionTypes.end()) {
        *it = updatedSessionType;
    }
    else {
        throw std::runtime_error("Session type with ID " + typeId + " not found.");
    }
}

void SessionTypeManager::deleteSessionType(const std::string& typeId) {
    auto it = std::remove_if(sessionTypes.begin(), sessionTypes.end(),
        [&typeId](const SessionType& st) { return st.getTypeId() == typeId; });
    if (it != sessionTypes.end()) {
        sessionTypes.erase(it, sessionTypes.end());
    }
    else {
        throw std::runtime_error("Session type with ID " + typeId + " not found.");
    }
}

void SessionTypeManager::listSessionTypes() const {
    if (sessionTypes.empty()) {
        std::cout << "No session types available." << std::endl;
        return;
    }
    for (const auto& st : sessionTypes) {
        st.displaySessionType();
        std::cout << "-----------------" << std::endl;
    }
}
