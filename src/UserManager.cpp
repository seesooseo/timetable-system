#include "UserManager.hpp"
#include <Student.hpp>
#include <algorithm>
#include<stdexcept>
#include <Admin.hpp>
#include <GroupManager.hpp>

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
std::vector<Admin*> UserManager::getAdmins() const {
    std::vector<Admin*> admins;
    for (auto& u : users) {
        if (auto a = dynamic_cast<Admin*>(u.get()))
            admins.push_back(a);
    }
    return admins;
}

std::vector<Student*> UserManager::getStudents() const {
    std::vector<Student*> students;
    for (auto& u : users) {
        if (auto s = dynamic_cast<Student*>(u.get()))
            students.push_back(s);
    }
    return students;
}

Student* UserManager::findStudent(const std::string& id) const {
    User* u = findUserById(id);
    return dynamic_cast<Student*>(u);
}

void UserManager::assignStudentToGroup(
    const std::string& studentId,
    const std::string& groupId,
    const GroupManager& groupManager
) {
    // 1) ensure the group actually exists
        // find the student
        User * u = findUserById(studentId);
        if (!u) throw std::runtime_error("Student ID " + studentId + " not found");
            auto * s = dynamic_cast<Student*>(u);
            if (!s) throw std::runtime_error("User " + studentId + " is not a student");
    
            // verify that group exists
                if (!groupManager.findGroup(groupId)) {
                throw std::runtime_error("Group ID " + groupId + " does not exist");
           
                }
    
                // assign
            s->setGroup(groupId);
    }
