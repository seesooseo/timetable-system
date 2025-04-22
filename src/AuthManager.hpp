#ifndef AUTH_MANAGER_HPP
#define AUTH_MANAGER_HPP

#include "UserManager.hpp"
#include "User.hpp"

/*
 * Class: AuthManager
 * -------------------------------
 * Handles user authentication.
 */
class AuthManager {
private:
    UserManager& userManager; // Reference to the UserManager instance.
public:
    explicit AuthManager(UserManager& um);
    // Login function only declared here.
    User* loginUser(const std::string& userId);
};

#endif // AUTH_MANAGER_HPP
