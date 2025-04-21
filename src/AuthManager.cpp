#include "AuthManager.hpp"
#include "Admin.hpp"
#include "Student.hpp"
#include <iostream>
#include <memory>

AuthManager::AuthManager(UserManager& um) : userManager(um) {}

// Define loginUser here – this is the only definition.
User* AuthManager::loginUser(const std::string& userId) {
    User* user = userManager.findUserById(userId);
    if (!user) {
        std::cout << "Login failed. User with ID " << userId << " does not exist." << std::endl;
    }
    else {
        std::cout << "Login successful! Welcome, " << user->getName() << "." << std::endl;
    }
    return user;
}
