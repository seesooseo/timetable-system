#include "UserManager.hpp"
#include <Student.hpp>
#include <algorithm>
#include<stdexcept>

UserManager::UserManager() : users() {}

// Adds a new user to the system.
void UserManager::addUser(std::unique_ptr<User> user) {
    auto it = std::find_if(users.begin(), users.end(),
        [&user](const std::unique_ptr<User>& u) {
            return u->getUserID() == user->getUserID();
        });
    if (it != users.end()) {
        throw std::runtime_error("User with ID " + user->getUserID() + " already exists.");
    }
    users.push_back(std::move(user));
}

// Searches for a user by user ID.
User* UserManager::findUserById(const std::string& userId) const {
    auto it = std::find_if(users.begin(), users.end(),
        [&userId](const std::unique_ptr<User>& u) {
            return u->getUserID() == userId;
        });
    return (it != users.end()) ? it->get() : nullptr;
}

// Removes a user by their user ID.
void UserManager::removeUser(const std::string& userId) {
    auto it = std::remove_if(users.begin(), users.end(),
        [&userId](const std::unique_ptr<User>& u) {
            return u->getUserID() == userId;
        });
    if (it != users.end()) {
        users.erase(it, users.end());
    }
    else {
        throw std::runtime_error("User with ID " + userId + " not found. Cannot delete.");
    }
}

// Lists all registered users.
void UserManager::listUsers() const {
    if (users.empty()) {
        std::cout << "No users available." << std::endl;
        return;
    }
    for (const auto& u : users) {
        u->displayUserInfo();
        std::cout << "-----------------" << std::endl;
    }
}

Student* UserManager::findStudent(const std::string& id) const {
    User* u = findUserById(id);
    return dynamic_cast<Student*>(u);
}

void UserManager::assignStudentToGroup(const std::string& sid,
    const std::string& gid) {
    Student* s = findStudent(sid);
    if (!s) throw std::runtime_error("Student not found");
    s->setGroup(gid);
}
