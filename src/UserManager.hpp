#ifndef USERMANAGER_HPP
#define USERMANAGER_HPP

#include "User.hpp"          // Ensure that this file exists and is correct.
#include <vector>
#include <memory>          // For std::unique_ptr
#include <algorithm>       // For std::find_if and std::remove_if
#include <stdexcept>       // For std::runtime_error
#include <iostream>
#include <string>

class Student;

class UserManager {
private:
    std::vector<std::unique_ptr<User>> users;
public:
    UserManager();
    void addUser(std::unique_ptr<User> user);
    User* findUserById(const std::string& userId) const;
    void removeUser(const std::string& userId);
    void listUsers() const;
    Student* findStudent(const std::string& id) const;
    void assignStudentToGroup(const std::string& studentId,
        const std::string& groupId);
};


#endif // USERMANAGER_HPP
